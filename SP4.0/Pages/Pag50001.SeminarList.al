page 50001 "Seminar List"
{
    ApplicationArea = All;
    UsageCategory = Lists;
    Caption = 'Seminar List';
    PageType = List;
    SourceTable = Seminar;
    CardPageID = "Seminar";

    layout
    {
        area(content)
        {
            repeater(General)
            {
                field("Code"; Rec."Code")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Code field.';
                }
                field(Name; Rec.Name)
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Name field.';
                }
                field("Seminar Duration"; Rec."Seminar Duration")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Seminar Duration field.';
                }
                field("Search Name"; Rec."Search Name")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Search Name field.';
                }
                field("Minimum Participants"; Rec."Minimum Participants")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Minimum Participants field.';
                }
                field("Maximum Participants"; Rec."Maximum Participants")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Maximum Participants field.';
                }
                field(Blocked; Rec.Blocked)
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Blocked field.';
                }
                field("Seminar Price"; Rec."Seminar Price")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Seminar Price field.';
                }
            }
        }
    }
}
