table 50005 AccountMapSettings
{
    Caption = 'AccountMapSettings';
    DataClassification = ToBeClassified;

    fields
    {
        field(1; "No. (Symfonia)"; Code[20])
        {
            Caption = 'No. (Symfonia)';
        }
        field(2; "No. (BC)"; Code[20])
        {
            Caption = 'No. (BC)';
            TableRelation = "G/L Account";
        }
    }
    keys
    {
        key(PK; "No. (Symfonia)", "No. (BC)")
        {
            Clustered = true;
        }
    }
}
