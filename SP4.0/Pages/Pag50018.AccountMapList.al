page 50018 AccountMapList
{
    ApplicationArea = All;
    Caption = 'AccountMapList';
    PageType = List;
    SourceTable = AccountMapSettings;
    UsageCategory = Lists;

    layout
    {
        area(content)
        {
            repeater(General)
            {
                Caption = 'General';
                field("No. (Symfonia)"; Rec."No. (Symfonia)")
                {
                    ApplicationArea = All;
                }
                field("No. (BC)"; Rec."No. (BC)")
                {
                    ApplicationArea = All;
                }
            }
        }
    }
}
