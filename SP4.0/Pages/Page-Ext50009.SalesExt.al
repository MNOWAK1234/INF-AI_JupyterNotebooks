pageextension 50009 SalesPageExt extends "Sales & Receivables Setup"
{
    layout
    {
        addlast(content)
        {
            group(SeminarModule)
            {
                Caption = 'Seminar Module';

                field("G/L Account No."; Rec."G/L Account No.")
                {
                    Caption = 'G/L Account No.';
                    TableRelation = "G/L Account";
                }
            }
        }
    }
}