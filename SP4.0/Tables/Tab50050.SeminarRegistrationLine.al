Table 50050 "Seminar Registration Line"
{
    Caption = 'Seminar Registration Line';
    fields
    {
        field(1; "Seminar Registration No."; Code[20])
        {
            Caption = 'Seminar Registration No.';
            TableRelation = "Seminar Registration Header";
        }
        field(2; "Line No."; Integer)
        {
            Caption = 'Line No.';
        }
        field(3; "Bill-to Customer No."; Code[20]) //4.5.4
        {
            Caption = 'Bill-to Customer No.';
            TableRelation = "Customer";

            trigger OnValidate()
            begin
                if (Registered) then Error(BillToCustomerChangeErr);
            end;
        }
        field(4; "Participant Contact No."; Code[20])
        {
            Caption = 'Participant Contact No.';
            TableRelation = "Contact";

            trigger OnLookup() //4.5.6
            var
                ContactBusinessRelation: Record "Contact Business Relation";
                Contact: Record Contact;
            begin
                ContactBusinessRelation.RESET;
                ContactBusinessRelation.SETRANGE("Link to Table", ContactBusinessRelation."Link to Table"::Customer);
                ContactBusinessRelation.SETRANGE("No.", "Bill-to Customer No.");
                if ContactBusinessRelation.FINDFIRST then begin
                    Contact.SETRANGE("Company No.", ContactBusinessRelation."Contact No.");
                    if PAGE.RUNMODAL(5052, Contact) = ACTION::LookupOk then
                        "Participant Contact No." := Contact."No.";
                    CalcFields("Participant Name");
                end;
            end;

        }
        field(5; "Participant Name"; Text[100])
        {
            Caption = 'Participant Name';
            FieldClass = FlowField;
            CalcFormula = lookup(Contact.Name where("No." = field("Participant Contact No.")));
        }
        field(6; "Register Date"; Date)
        {
            Caption = 'Register Date';
            Editable = false;
        }
        field(7; "To Invoice"; Boolean)
        {
            Caption = 'To Invoice';
            InitValue = false;
        }
        field(8; Participated; Boolean)
        {
            Caption = 'Participated';
        }
        field(9; "Confirmation Date"; Date)
        {
            Caption = 'Confirmation Date';
        }
        field(10; "Seminar Price"; Decimal)
        {
            Caption = 'Seminar Price';
            AutoFormatType = 2;
            trigger OnValidate()
            begin
                "Line Discount Amount" := 0;
                "Line Discount %" := 0;
            end;
        }
        field(11; "Line Discount %"; Decimal)
        {
            Caption = 'Line Discount %';
            DecimalPlaces = 0 : 5;
            MinValue = 0;
            MaxValue = 100;

            trigger OnValidate() //4.5.2
            begin
                "Amount" := "Seminar Price" - "Seminar Price" * "Line Discount %" / 100;
                "Line Discount Amount" := "Seminar Price" * "Line Discount %" / 100;
            end;
        }
        field(12; "Line Discount Amount"; Decimal)
        {
            Caption = 'Line Discount Amount';
            AutoFormatType = 1;

            trigger OnValidate() //4.5.2
            begin
                "Amount" := "Seminar Price" - "Line Discount Amount";
                "Line Discount %" := "Line Discount Amount" / "Seminar Price" * 100;
            end;
        }
        field(13; Amount; Decimal)
        {
            Caption = 'Amount';
            AutoFormatType = 1;

            trigger OnValidate() //4.5.2
            var
                SeminarHeader: Record "Seminar Registration Header";
            begin
                "Line Discount Amount" := "Seminar Price" - "Amount";
                "Line Discount %" := "Line Discount Amount" / "Seminar Price" * 100;
                SeminarHeader.Get("Seminar Registration No.");
            end;
        }
        field(14; Registered; Boolean)
        {
            Caption = 'Registered';

            trigger OnValidate() //4.5.5
            begin
                if (Registered) then "Register Date" := WorkDate();
            end;
        }
        field(15; "Invoice No."; Code[20])
        {
            Caption = 'Invoice No.';
            Editable = false;
            TableRelation = "Sales Header" where("Document Type" = filter(Invoice));
        }
    }

    keys
    {
        key(Key1; "Seminar Registration No.", "Line No.")
        {
            Clustered = true;
            SumIndexFields = Amount;
        }
    }

    var
        DeleteRegisteredErr: label 'Registered lines cannot be deleted';
        BillToCustomerChangeErr: label 'Bill-to-Customer No. cannot be modified for registered lines';

    trigger OnInsert() //4.5.1
    var
        SeminarHeader: Record "Seminar Registration Header";
    begin
        SeminarHeader.Get("Seminar Registration No.");
        "Seminar Price" := SeminarHeader."Seminar Price";
        "Amount" := SeminarHeader."Seminar Price";
        "Line Discount %" := 0;
        "Line Discount Amount" := 0;
    end;

    trigger OnDelete() //4.5.3
    begin
        if (Registered) then Error(DeleteRegisteredErr);
    end;
}