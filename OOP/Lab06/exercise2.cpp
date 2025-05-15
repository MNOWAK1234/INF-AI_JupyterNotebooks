#include <iostream>
#include <cmath>

using namespace std;

class Number
{
public:
    virtual double doubleValue()=0;
};

class Logarithm : public Number
{
private:
    double base,argument;
public:
    parameters(double base, double argument)
    {
        this->base=base;
        this->argument=argument;
    }
    double doubleValue()
    {
        if(this->base<=0)throw invalid_argument("Base must be positive.");
        else if(this->base==1)throw invalid_argument("Base must not be equal to 1.");
        else if(this->argument<=0)throw invalid_argument("Argument must be positive.");
        else return log(this->argument)/log(this->base);
    }
};

int main()
{
    double testBase,testArgument;
    cin>>testBase>>testArgument;
    try
    {
        Logarithm numbersOneAndTwo;
        numbersOneAndTwo.parameters(testBase,testArgument);
        cout<<numbersOneAndTwo.doubleValue()<<endl;
    }
    catch(const invalid_argument &e)
    {
        cout<<"Logarithm of a positive number x to the positive base b!=1 is the exponent to which b must be raised, to produce x."<<endl;
        cout<<endl;
        cout<<"Error: ";
        cout<<e.what()<<endl;
    }
}
