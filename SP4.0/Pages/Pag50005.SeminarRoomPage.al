page 50005 SeminarRoomPage
{
    Caption = 'Seminar Room';
    PageType = Card;
    SourceTable = "Seminar Room";

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
                field(Address; Rec.Address)
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Address field.';
                }
                field("Address 2"; Rec."Address 2")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Address 2 field.';
                }
                field("Post Code"; Rec."Post Code")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Post Code field.';
                }
                field(City; Rec.City)
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the City field.';
                }
                field("Country/Region Code"; Rec."Country/Region Code")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Country/Region Code field.';
                }
                field("Maximum Participants"; Rec."Maximum Participants")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Maximum Participants field.';
                }
                field("Internal/External"; Rec."Internal/External")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Internal/External field.';
                }
            }
            group(Communication)
            {

                Caption = 'Communication';
                field("Phone No."; Rec."Phone No.")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Phone No. field.';
                }
                field("Fax No."; Rec."Fax No.")
                {
                    ApplicationArea = All;
                    // ToolTip = 'Specifies the value of the Fax No. field.';
                }
            }
        }
    }
}
