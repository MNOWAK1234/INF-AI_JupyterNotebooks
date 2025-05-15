#include <iostream>
#include <vector>

using namespace std;

class Human
{
public:
    string firstName;
    string lastName;
    int age;
    void saySomething(string what)
    {
        cout<<what<<endl;
    }
    string fullname()
    {
        return (this->firstName + " " + this->lastName);
    }
};

class Device
{
public:
    string type;
    bool electrical;
    string ID;
    Human owner;
    void buy()
    {
        cout<<this->owner.fullname()<<" got device "<<this->ID<<endl;
    }
    void sell()
    {
        cout<<this->owner.fullname()<<" sold device "<<this->ID<<endl;
    }
};

class Store
{
public:
    string address;
    vector<Human> owners;
    void newClient(Human person)
    {
        owners.push_back(person);
    }
    void clients()
    {
        cout<<"List of current clients:"<<endl;
        for(int i=0; i<this->owners.size(); i++)
        {
            cout<<this->owners[i].fullname()<<endl;
        }
    }
};

int main()
{
    Human Me;
    Me.firstName="Mikolaj";
    Me.lastName="Nowak";
    Me.age=20;
    Me.saySomething("I was born");

    Human Friend;
    Friend.firstName="Jan";
    Friend.lastName="Kowalski";
    Friend.age=25;

    Device Microwave;
    Microwave.type="machine";
    Microwave.electrical=1;
    Microwave.ID="234.235.0.67";
    Microwave.owner=Me;
    Microwave.buy();

    Store Lidl;
    Lidl.address="White St. 87/6";
    Lidl.newClient(Me);
    Lidl.clients();
    Lidl.newClient(Friend);
    Lidl.clients();
}
