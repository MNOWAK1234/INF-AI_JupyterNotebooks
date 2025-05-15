codeunit 50020 CreateJournalLines
{
    procedure CreateJournalLines(importBuffer: Record ImportBuffer)
    var
        id: Integer;
        generalJournalLine: Record "Gen. Journal Line";
        tempFileLine: Code[100];
        position: Integer;
        documentName: Code[70];
        lineNo: Integer;
        accountNoWn: Code[20];
        descriptionWn: Text[100];
        accountNoMa: Code[20];
        descriptionMa: Text[100];
        debitAmount: Decimal;
        creditAmount: Decimal;
        amountWn: Decimal;
        amountMa: Decimal;
        side: Code[10];
        parallel: Code[10];
        second: Boolean;
        dateOper: Date;
        GenJnlBatch: Record "Gen. Journal Batch";
        NoSeriesMgt: Codeunit NoSeriesManagement;
        seriesNo: Code[20];
        importSettings: Record "Settings Table";
        generalJournalLink: Page "General Journal";
        url: Text;
        decision: Integer;
    begin
        importSettings.FindFirst();
        Clear(NoSeriesMgt);
        GenJnlBatch.Get(importSettings."Journal Template", importSettings.Instance);
        seriesNo := NoSeriesMgt.TryGetNextNo(GenJnlBatch."No. Series", WorkDate());
        if importBuffer.FindSet() then begin
            repeat
                second := false;
                //MESSAGE('Przetwarzanie linii pierwsze: ' + importBuffer."Line Content");
                if importBuffer."Line Content" = 'DOKUMENT{' then begin
                    repeat
                        //MESSAGE('Przetwarzanie linii w dokumencie: ' + importBuffer."Line Content");
                        tempFileLine := importBuffer."Line Content";
                        position := StrPos(tempFileLine, 'NAZWA =');
                        if position > 0 then begin
                            documentName := DelStr(tempFileLine, position, 7);
                        end;
                        position := StrPos(tempFileLine, 'DATAOPER =');
                        if position > 0 then begin
                            Evaluate(dateOper, DelStr(tempFileLine, position, 10));
                        end;
                    until (importBuffer.Next() = 0) or (StrPos(importBuffer."Line Content", 'OBSŁUGUJJAK') > 0);
                    seriesNo := NoSeriesMgt.TryGetNextNo(GenJnlBatch."No. Series", dateOper);
                end;
                //MESSAGE('Przetwarzanie linii wpierw: ' + importBuffer."Line Content");
                if importBuffer."Line Content" = 'ZAPIS{' then begin
                    repeat
                        //MESSAGE('Przetwarzanie linii: ' + importBuffer."Line Content");
                        tempFileLine := importBuffer."Line Content";
                        position := StrPos(tempFileLine, 'WN/MA =');
                        if position > 0 then begin
                            side := DelStr(tempFileLine, position, 7);
                        end;
                        if side = 'WN' then begin
                            position := StrPos(tempFileLine, 'OPIS =');
                            if position > 0 then begin
                                descriptionWn := DelStr(tempFileLine, position, 6);
                            end;
                            position := StrPos(tempFileLine, 'KWOTA =');
                            if position > 0 then begin
                                Evaluate(amountWn, DelStr(ConvertStr(tempFileLine, '.', ','), position, 7));
                            end;
                            position := StrPos(tempFileLine, 'KONTO =');
                            if position > 0 then begin

                                accountNoWn := DelStr(tempFileLine, position, 7);
                            end;
                        end else
                            if side = 'MA' then begin
                                tempFileLine := importBuffer."Line Content";
                                position := StrPos(tempFileLine, 'OPIS =');
                                if position > 0 then begin
                                    descriptionMa := DelStr(tempFileLine, position, 6);
                                end;
                                position := StrPos(tempFileLine, 'KWOTA =');
                                if position > 0 then begin
                                    Evaluate(amountMa, DelStr(ConvertStr(tempFileLine, '.', ','), position, 7));
                                end;
                                position := StrPos(tempFileLine, 'KONTO =');
                                if position > 0 then begin
                                    accountNoMa := DelStr(tempFileLine, position, 7);
                                end;
                            end;
                        position := StrPos(tempFileLine, 'RÓWNOLEGŁY =');
                        if position > 0 then begin
                            parallel := DelStr(tempFileLine, position, 12);
                        end;
                        if (not second) and (StrPos(importBuffer."Line Content", 'RÓWNOLEGŁY =') > 0) then begin
                            importBuffer.Next(2);
                            second := true;
                        end;

                    until (importBuffer.Next() = 0) or (StrPos(importBuffer."Line Content", '}') > 0);
                    generalJournalLine.Reset();
                    generalJournalLine.SetRange("Journal Template Name", importSettings."Journal Template");
                    generalJournalLine.SetRange("Journal Batch Name", importSettings.Instance);
                    if generalJournalLine.FindLast() then begin
                        lineNo := generalJournalLine."Line No.";
                    end;
                    lineNo := lineNo + 1;
                    generalJournalLine."Line No." := lineNo;
                    generalJournalLine."Journal Template Name" := importSettings."Journal Template";
                    generalJournalLine."Journal Batch Name" := importSettings.Instance;
                    generalJournalLine."Document No." := seriesNo;
                    generalJournalLine.Description := descriptionWn;
                    //MESSAGE('amountWn value: %1', amountWn);
                    //MESSAGE('amountMa value: %1', amountMa);
                    generalJournalLine.Amount := amountWn;
                    //generalJournalLine."Debit Amount" := amountWn;
                    //generalJournalLine."Credit Amount" := amountMa;
                    generalJournalLine."Account No." := accountNoWn;
                    generalJournalLine."Bal. Account No." := accountNoMa;
                    generalJournalLine."Posting Date" := WorkDate();
                    generalJournalLine.Insert();
                end;
            until importBuffer.Next() = 0;
            Dialog.Message(messageLbl, seriesNo);
        end;


    end;

    var
        messageLbl: Label 'Journal lines created, document No. %1';



}
