page 50007 SeminarRegistrationList
{
    Caption = 'Seminar Registration List';
    PageType = List;
    SourceTable = "Seminar Registration Header";
    ApplicationArea = All;
    UsageCategory = Lists;
    CardPageId = "SeminarRegistrationCard";

    layout
    {
        area(content)
        {
            repeater(General)
            {
                field("No."; Rec."No.")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the No. field.';
                }
                field("Starting Date"; Rec."Starting Date")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Starting Date field.';
                }
                field("Seminar Code"; Rec."Seminar Code")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Seminar Code field.';
                }
                field("Seminar Name"; Rec."Seminar Name")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Seminar Name field.';
                }
                field("Instructor Code"; Rec."Instructor Code")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Instructor Code field.';
                }
                field("Instructor Name"; Rec."Instructor Name")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Instructor Name field.';
                }
                field(Status; Rec.Status)
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Status field.';
                }
            }
        }
    }

    actions
    {
        area(Processing)
        {
            group(ExportActions)
            {
                Caption = 'Export Actions';

                action(Export)
                {
                    ApplicationArea = All;
                    Caption = 'Export seminar participant list';
                    Image = XMLFile;
                    Promoted = true;
                    PromotedCategory = Process;

                    trigger OnAction()
                    begin
                        Xmlport.Run(Xmlport::"SeminarRegList Export XML");
                    end;
                }
            }
        }
    }
}
