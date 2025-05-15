page 50011 SeminarManagerRoleCenter
{
    Caption = 'Seminar Manager';
    PageType = RoleCenter;

    layout
    {
    }

    actions
    {
        area(Sections)
        {
            group(Lists)
            {
                Caption = 'Lists';
                action(Seminars)
                {
                    ApplicationArea = All;
                    Caption = 'Seminars';
                    RunObject = page "Seminar List";
                }
                action(Instructors)
                {
                    ApplicationArea = All;
                    Caption = 'Instructors';
                    RunObject = page InstructorList;
                }
                action(SeminarRooms)
                {
                    ApplicationArea = All;
                    Caption = 'Seminar Rooms';
                    RunObject = page SeminarRoomList;
                }
                action(SeminarRegistration)
                {
                    ApplicationArea = All;
                    Caption = 'Seminar Registration';
                    RunObject = page SeminarRegistrationList;
                }
            }
            group(Tasks)
            {
                Caption = 'Tasks';
                action(Export)
                {
                    ApplicationArea = All;
                    Caption = 'Export seminar participant list';
                    Image = XMLFile;
                    RunObject = xmlport "SeminarRegList Export XML";
                }
            }
        }
    }
}

profile "Seminar Manager"
{
    Caption = 'Seminar Manager';
    RoleCenter = "SeminarManagerRoleCenter";
}