#include <iostream>

using namespace std;

class Owner
{
private:
    string firstname;
    string lastname;
    int age;
    int yearlyIncome;
    int ownsSince;
public:
    Owner(string firstname, string lastname, int age, int yearlyIncome, int ownsSince)
    {
        this->firstname=firstname;
        this->lastname=lastname;
        this->age=age;
        this->yearlyIncome=yearlyIncome;
        this->ownsSince=ownsSince;
    }
    Owner();
    bool realPerson(int currentYear)
    {
        return (currentYear-ownsSince-age>=0);
    }
};

class TechnicalData
{
private:
    int yearOfProduction;
    int engineBHP;
    int serialnumber;
public:
    TechnicalData(int yearOfProduction, int engineBHP, int serialnumber)
    {
        this->yearOfProduction=yearOfProduction;
        this->engineBHP=engineBHP;
        this->serialnumber=serialnumber;
    }
    TechnicalData();
    bool technicalCheck()
    {
        int serialNumberFront=serialnumber;
        while(serialNumberFront>=100)serialNumberFront/=10;
        return (yearOfProduction%100==serialNumberFront);
    }
};
class RaceParameters
{
private:
    double safetyBubble;
    double minAcc;
    double maxAcc;
    double turnRatio;
    double fieldOfVievRatio;
public:
    RaceParameters(double safetyBubble, double minAcc, double maxAcc, double turnRatio)
    {
        this->safetyBubble=safetyBubble;
        this->minAcc=minAcc;
        this->maxAcc=maxAcc;
        while(turnRatio>1)turnRatio/=10;//must be in range (0,1)
        this->turnRatio=turnRatio;
        this->fieldOfVievRatio=(1-turnRatio);
    }
    RaceParameters();
    double minVelocity()
    {
        return minAcc*(turnRatio/0.5);
    }
    double maxVelocity()
    {
        return maxAcc*(fieldOfVievRatio/0.5);
    }
};
class LegalInformation
{
private:
    int nextInspection;
    bool approved;
    char vehicleClass;
    double taxDeduction;
    int lastRegisteredSell;
public:
    LegalInformation(int nextInspection, int currentYear, char vehicleClass, double taxDeduction, int lastRegisteredSell)
    {
        this->nextInspection=nextInspection;
        this->approved=(currentYear<=nextInspection);
        this->vehicleClass=vehicleClass;
        this->taxDeduction;
        this->lastRegisteredSell;
    }
    LegalInformation();
    int inspectionsByThisOwner()
    {
        return(nextInspection-lastRegisteredSell)/5;
    }
};

class Vehicle
{
private:
    Owner Person;
    TechnicalData Data;
    RaceParameters Parameters;
    LegalInformation LawyerInfo;
public:
    Vehicle(Owner Person, TechnicalData Data, RaceParameters Parameters, LegalInformation Lawyerinfo)
    {
        this->Person=Person;
        this->Data=Data;
        this->Parameters=Parameters;
        this->LawyerInfo=LawyerInfo;
    }
    void isOk()
    {
        if(Person.realPerson(2022) && Data.technicalCheck() && LawyerInfo.inspectionsByThisOwner()<2)
        {
            cout<<"Everything OK"<<endl;
        }
        else cout<<"Something wrong"<<endl;
    }
};

int main()
{
    /*Owner CL16("Charles", "Leclerc", 24, 9000000, 2019);
    TechnicalData F90(2023, 1128, 1655);
    RaceParameters Monza(0.3, 80, 300, 0.2);
    LegalInformation Burocracy(2024, 2023, 'S', 1000000, 2023);
    Vehicle Ferrari(CL16, F90, Monza, Burocracy);*/
}
