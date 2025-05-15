page 50003 InstructorList
{
    Caption = 'Instructor List';
    PageType = List;
    SourceTable = Instructor;
    ApplicationArea = All;
    UsageCategory = Lists;

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
                field("Worker/Subcontractor"; Rec."Worker/Subcontractor")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Worker/Subcontractor field.';
                }
                field("Resource No."; Rec."Resource No.")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Resource No. field.';
                }
                field("Vendor No."; Rec."Vendor No.")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Vendor No. field.';
                }
            }
        }
    }
}
