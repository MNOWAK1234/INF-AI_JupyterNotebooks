table 50004 "Settings Table"
{
    Caption = 'Settings Table';
    DataClassification = ToBeClassified;

    fields
    {
        field(1; "Journal Template"; Code[10])
        {
            Caption = 'Journal Template';
            TableRelation = "Gen. Journal Template";

        }
        field(2; Instance; Code[20])
        {
            Caption = 'Instance';
            TableRelation = "Gen. Journal Batch".Name where("Journal Template Name" = field("Journal Template"));
        }

    }
    keys
    {
        key(PK; "Journal Template", Instance)
        {
            Clustered = true;
        }
    }
}
