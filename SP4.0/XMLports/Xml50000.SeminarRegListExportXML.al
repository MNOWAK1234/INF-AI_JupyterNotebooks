xmlport 50000 "SeminarRegList Export XML"
{
    Caption = 'SeminarRegList Export XML';
    Direction = Export;
    Format = xml;
    UseRequestPage = true;

    schema
    {
        textelement(Seminar_Registartion_Participant_List)
        {
            tableelement(Seminar; "Seminar Registration Header")
            {
                fieldelement(Registration_No; Seminar."No.")
                {
                }
                fieldelement(Seminar_Code; Seminar."Seminar Code")
                {
                }
                fieldelement(Seminar_Name; Seminar."Seminar Name")
                {
                }
                fieldelement(Starting_Date; Seminar."Starting Date")
                {
                }
                fieldelement(Seminar_Duration; Seminar."Seminar Duration")
                {
                }
                fieldelement(Instructor_Name; Seminar."Instructor Name")
                {
                }
                fieldelement(Room_Name; Seminar."Seminar Room Name")
                {
                }
            }

            tableelement(Participant; "Seminar Registration Line")
            {
                LinkTable = Seminar;
                LinkFields = "Seminar Registration No." = FIELD("No."); // Display only registered people
                MinOccurs = Zero; // Display only if there is someone registered

                fieldelement(Customer_No; Participant."Bill-to Customer No.")
                {
                }
                textelement(Customer_Name) // Get value from the customer table
                {
                    trigger OnBeforePassVariable()
                    var
                        Customer: Record "Customer";
                    begin
                        Customer.Get(Participant."Bill-to Customer No.");
                        Customer_Name := Customer.Name;
                    end;
                }
                fieldelement(Contact_No; Participant."Participant Contact No.")
                {
                }
                fieldelement(Participant_Name; Participant."Participant Name")
                {
                }
            }
        }
    }

    requestpage
    {
    }
}