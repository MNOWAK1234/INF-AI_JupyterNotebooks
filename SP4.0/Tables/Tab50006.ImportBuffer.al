table 50006 ImportBuffer
{
    Caption = 'ImportBuffer';
    DataClassification = ToBeClassified;

    fields
    {
        field(1; "Line No."; Integer)
        {
            Caption = 'Line No.';
            AutoIncrement = true;
        }
        field(2; "Line Content"; Code[70])
        {
            Caption = 'Line Content';
        }
    }
    keys
    {
        key(PK; "Line No.")
        {
            Clustered = true;
        }
    }
}
