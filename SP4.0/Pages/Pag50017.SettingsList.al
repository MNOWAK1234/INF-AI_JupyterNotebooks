page 50017 "Settings List"
{
    ApplicationArea = All;
    Caption = 'Settings List';
    PageType = Card;
    SourceTable = "Settings Table";
    UsageCategory = Documents;
    Editable = true;

    layout
    {
        area(content)
        {
            group(General)
            {
                Caption = 'General';
                field("Journal Template"; Rec."Journal Template")
                {
                    ApplicationArea = All;
                    Lookup = true;
                    Editable = true;
                }
                field(Instance; Rec.Instance)
                {
                    ApplicationArea = All;
                    Lookup = true;
                    Editable = true;
                }
            }
        }
    }
}
