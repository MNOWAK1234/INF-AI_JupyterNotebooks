page 50002 Seminar
{
    Caption = 'Seminar';
    PageType = Card;
    SourceTable = Seminar;

    layout
    {
        area(content)
        {
            group(General)
            {
                Caption = 'General';

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
                field("Search Name"; Rec."Search Name")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Search Name field.';
                }
                field("Seminar Duration"; Rec."Seminar Duration")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Seminar Duration field.';
                }
                field("Seminar Price"; Rec."Seminar Price")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Seminar Price field.';
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
                field("Last Date Modified"; Rec."Last Date Modified")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Last Date Modified field.';
                }
            }
        }
    }
}
