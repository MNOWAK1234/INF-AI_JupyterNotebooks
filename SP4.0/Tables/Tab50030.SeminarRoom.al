Table 50030 "Seminar Room"
{
    Caption = 'Seminar Room';
    DataCaptionFields = Code, Name;
    fields
    {
        field(1; "Code"; Code[20])
        {
            Caption = 'Code';
        }
        field(2; Name; Text[50])
        {
            Caption = 'Name';
        }
        field(3; Address; Text[50])
        {
            Caption = 'Address';
        }
        field(4; "Address 2"; Text[50])
        {
            Caption = 'Address 2';
        }
        field(5; City; Text[30])
        {
            Caption = 'City';
        }
        field(6; "Post Code"; Code[20])
        {
            Caption = 'Post Code';
            TableRelation = "Post Code";

            trigger onValidate() //4.3
            var
                DummyCounty: Text[30];
            begin
                PostCode.ValidatePostCode(City, "Post Code", DummyCounty, "Country/Region Code", (CurrFieldNo <> 0) and GuiAllowed());
            end;
        }
        field(7; "Country/Region Code"; Code[10])
        {
            Caption = 'Country/Region Code';
            TableRelation = "Country/Region";
        }
        field(8; "Phone No."; Text[30])
        {
            Caption = 'Phone No.';
        }
        field(9; "Fax No."; Text[30])
        {
            Caption = 'Fax No.';
        }
        field(10; "Name 2"; Text[50])
        {
            Caption = 'Name 2';
        }
        field(11; "Maximum Participants"; Integer)
        {
            Caption = 'Maximum Participants';
        }
        field(12; "Internal/External"; Option)
        {
            Caption = 'Internal/External';
            OptionMembers = "Internal","External";
            OptionCaption = 'Internal,External';
        }
    }

    keys
    {
        key(Key1; "Code")
        {
            Clustered = true;
        }
    }
    var
        PostCode: Record "Post Code";
}

