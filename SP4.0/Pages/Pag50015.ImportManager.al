page 50015 "Import Manager"
{
    Caption = 'Import Manager';
    PageType = RoleCenter;
    RefreshOnActivate = false;
    UsageCategory = Administration;

    actions
    {
        area(Sections)
        {
            group(Lists)
            {
                Caption = 'Lists';
                action("Import settings")
                {
                    Caption = 'Import settings';
                    ApplicationArea = All;
                    RunPageMode = View;
                    RunObject = page "Settings List";
                }
                action("Map accounts")
                {
                    Caption = 'Map accounts';
                    ApplicationArea = All;
                    RunPageMode = View;
                    RunObject = page AccountMapList;
                }
                action("Import buffer")
                {
                    Caption = 'Import buffer';
                    ApplicationArea = All;
                    RunPageMode = View;
                    RunObject = page ImportBufferList;
                }
            }
        }
    }
}

profile "Import Manager"
{
    Caption = 'Import Manager';
    RoleCenter = "Import Manager";
}
