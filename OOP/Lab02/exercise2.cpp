#include <iostream>

using namespace std;

class Toothbrush
{
public:
    string owner;
    double original_price;
    double discount;
    string colour;
    string manufacturer;
    Toothbrush()
    {
        this->owner="me";
        this->original_price=1.6;
        this->discount=0;
        this->colour="red";
        this->manufacturer="teect";
    }
    Toothbrush(double p, double d)
    {
        this->original_price=p;
        this->discount=d;
    }
    Toothbrush(string own, string c)
    {
        this->owner=own;
        this->colour=c;
    }
    double priceAfterReduction()
    {
        return this->original_price*(1-this->discount);
    }
    string opinion()
    {
        if(this->colour=="blue" && this->manufacturer.size()<6 && this->priceAfterReduction()<5)return "good product";
        else return "bad product";
    }
    bool comp(Toothbrush &One, Toothbrush &Two)
    {
        return One.discount>Two.discount;
    }
};

int main()
{
    Toothbrush MyFriend;
    Toothbrush Bought(3.24, 0.5);
    cout<<Bought.priceAfterReduction()<<endl;
    Toothbrush Mine;
    cout<<Mine.opinion()<<endl;
}
