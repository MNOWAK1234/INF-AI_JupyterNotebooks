page 50008 SeminarRegistrationSubpage
{
    Caption = 'Seminar Registration Subpage';
    PageType = ListPart;
    SourceTable = "Seminar Registration Line";
    AutoSplitKey = true;

    layout
    {
        area(content)
        {
            repeater(General)
            {
                field("Bill-to Customer No."; Rec."Bill-to Customer No.")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Bill-to Customer No. field.';
                }
                field("Participant Contact No."; Rec."Participant Contact No.")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Participant Contact No. field.';
                }
                field("Participant Name"; Rec."Participant Name")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Participant Name field.';
                }
                field("Register Date"; Rec."Register Date")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Register Date field.';
                }
                field("Confirmation Date"; Rec."Confirmation Date")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Confirmation Date field.';
                }
                field("Seminar Price"; Rec."Seminar Price")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Seminar Price field.';
                }
                field("Line Discount %"; Rec."Line Discount %")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Line Discount % field.';
                }
                field("Line Discount Amount"; Rec."Line Discount Amount")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Line Discount Amount field.';
                }
                field(Amount; Rec.Amount)
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Amount field.';
                }
                field("To Invoice"; Rec."To Invoice")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the To Invoice field.';
                }
                field("Invoice No."; Rec."Invoice No.")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Invoice No. field.';
                }
                field(Participated; Rec.Participated)
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Participated field.';
                }
                field(Registered; Rec.Registered)
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Registered field.';
                }
            }
        }
    }
}
