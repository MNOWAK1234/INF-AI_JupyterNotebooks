Table 50040 "Seminar Registration Header"
{
    Caption = 'Seminar Registration Header';
    fields
    {
        field(1; "No."; Code[20])
        {
            Caption = 'No.';
        }
        field(2; "Starting Date"; Date)
        {
            Caption = 'Starting Date';

            trigger OnValidate() //4.4.5
            begin
                if Status <> Status::Planning then
                    Error(StartingDateChangeErr);
            end;
        }
        field(3; "Seminar Code"; Code[20])
        {
            Caption = 'Seminar Code';
            TableRelation = "Seminar";

            trigger onValidate() //4.4.1 and 4.4.6
            var
                Seminar: Record Seminar;
                SeminarRegLine: Record "Seminar Registration Line";
            begin
                SeminarRegLine.Reset();
                SeminarRegLine.SetRange(SeminarRegLine."Seminar Registration No.", "No.");
                if SeminarRegLine.FindSet() then
                    repeat
                        if SeminarRegLine.Registered then //4.4.6
                            Error(SeminarWithRegisteredLinesModifyErr);
                    until SeminarRegLine.Next() = 0;

                if Seminar.Get("Seminar Code") then begin //4.4.1
                    Seminar.TestField(Seminar.Blocked, false);
                    "Seminar Name" := Seminar.Name;
                    "Seminar Duration" := Seminar."Seminar Duration";
                    "Minimum Participants" := Seminar."Minimum Participants";
                    "Maximum Participants" := Seminar."Maximum Participants";
                    Validate("Seminar Price", Seminar."Seminar Price");
                end else begin
                    "Seminar Name" := '';
                    "Seminar Duration" := 0;
                    "Minimum Participants" := 0;
                    "Maximum Participants" := 0;
                    "Seminar Price" := 0;
                    SeminarRegLine."Line Discount %" := 0;
                    //SeminarRegLine."Line Discount %" := 0;
                end;
            end;
        }
        field(4; "Seminar Name"; Text[50])
        {
            Caption = 'Seminar Name';
        }
        field(5; "Instructor Code"; Code[20])
        {
            Caption = 'Instructor Code';
            TableRelation = "Instructor";

            trigger OnValidate() //4.4.7
            begin
                CalcFields("Instructor Name");
            end;
        }
        field(6; "Instructor Name"; Text[100])
        {
            Caption = 'Instructor Name';
            FieldClass = FlowField;
            CalcFormula = lookup(Instructor.Name where(Code = field("Instructor Code")));

        }
        field(7; Status; Option)
        {
            Caption = 'Status';
            OptionMembers = Planning,Registration,Finished,Cancelled;
            OptionCaption = 'Planning,Registration,Finished,Cancelled';
        }
        field(8; "Seminar Duration"; Decimal)
        {
            Caption = 'Seminar Duration';
        }
        field(9; "Minimum Participants"; Integer)
        {
            Caption = 'Minimum Participants';
        }
        field(10; "Maximum Participants"; Integer)
        {
            Caption = 'Maximum Participants';
        }
        field(11; "Seminar Room Code"; Code[20])
        {
            Caption = 'Seminar Room Code';
            TableRelation = "Seminar Room";

            trigger onValidate() //4.4.2
            var
                Seminar: Record "Seminar Room";
            begin
                if Seminar.Get("Seminar Room Code") then begin
                    if "Maximum Participants" > Seminar."Maximum Participants" then
                        Message(MaximumParticipantsWarning);
                    "Seminar Room Name" := Seminar.Name;
                    "Seminar Room Address" := Seminar.Address;
                    "Seminar Room Address 2" := Seminar."Address 2";
                    "Seminar Room Post Code" := Seminar."Post Code";
                    "Seminar Room City" := Seminar.City;
                    "Seminar Room Phone No." := Seminar."Phone No."
                end else begin
                    "Seminar Room Name" := '';
                    "Seminar Room Address" := '';
                    "Seminar Room Address 2" := '';
                    "Seminar Room Post Code" := '';
                    "Seminar Room City" := '';
                    "Seminar Room Phone No." := '';
                end;
            end;
        }
        field(12; "Seminar Room Name"; Text[50])
        {
            Caption = 'Seminar Room Name';
        }
        field(13; "Seminar Room Address"; Text[50])
        {
            Caption = 'Seminar Room Address';
        }
        field(14; "Seminar Room Address 2"; Text[50])
        {
            Caption = 'Seminar Room Address 2';
        }
        field(15; "Seminar Room Post Code"; Code[20])
        {
            Caption = 'Seminar Room Post Code';
            TableRelation = "Post Code";
        }
        field(16; "Seminar Room City"; Text[30])
        {
            Caption = 'Seminar Room City';
        }
        field(17; "Seminar Room Phone No."; Text[30])
        {
            Caption = 'Seminar Room Phone No';
        }
        field(18; "Posting Date"; Date)
        {
            Caption = 'Posting Date';
        }
        field(19; "Seminar Price"; Decimal)
        {
            Caption = 'Seminar Price';

            trigger OnValidate() //4.4.8
            var
                SeminarRegLine: Record "Seminar Registration Line";
            begin
                if (Status <> Status::Cancelled) then begin
                    if (Dialog.Confirm(SeminarPriceUpdateMess)) then begin

                        SeminarRegLine.Reset();
                        SeminarRegLine.SetRange(SeminarRegLine."Seminar Registration No.", "No.");
                        if SeminarRegLine.FindSet(true) then
                            repeat
                                if not SeminarRegLine.Registered then begin
                                    SeminarRegLine."Seminar Price" := "Seminar Price";
                                    //SeminarRegLine."Line Discount Amount" := SeminarRegLine."Line Discount %" / 100 * "Seminar Price";
                                    //SeminarRegLine.Amount := "Seminar Price" - SeminarRegLine."Line Discount Amount";
                                    SeminarRegLine."Line Discount Amount" := 0;
                                    SeminarRegLine."Line Discount %" := 0;
                                    SeminarRegLine.Amount := "Seminar Price";
                                    SeminarRegLine.Modify();
                                end;
                            until SeminarRegLine.Next() = 0;

                        CalcFields(Amount);
                    end else
                        Error(SeminarPriceChangeErr);
                end;
            end;
        }
        field(20; Amount; Decimal)
        {
            Caption = 'Amount';
            FieldClass = FlowField;
            CalcFormula = sum("Seminar Registration Line"."Amount" where("Seminar Registration No." = field("No.")));
        }
    }

    keys
    {
        key(Key1; "No.")
        {
            Clustered = true;
        }
    }

    var
        SeminarWithRegisteredLinesModifyErr: label 'Seminars with registered lines cannot be modified';
        StartingDateChangeErr: label 'Starting date can only be modified for planned seminars';
        MaximumParticipantsWarning: label 'The room is too small';
        DeleteRecordErr: label 'Only planned or cancelled courses can be deleted';
        PrimaryKeyChangeErr: label 'No. cannot be changed';
        SeminarPriceChangeErr: label 'Seminar Price was not changed';
        SeminarPriceUpdateMess: label 'Do you want to update the seminar price?';

    trigger OnInsert() //4.4.2
    begin
        "Posting Date" := WorkDate;
    end;

    trigger OnDelete() //4.4.3
    begin
        if (Status <> Status::Planning) and (Status <> Status::Cancelled) then
            Error(DeleteRecordErr);
    end;

    trigger OnRename() //4.4.4
    begin
        Error(PrimaryKeyChangeErr);
    end;
}