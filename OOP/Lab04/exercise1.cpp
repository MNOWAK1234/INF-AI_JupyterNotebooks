///updated version: 18.11.22
///compiler couldn't handle to_string() function, therefore it is removed and Car.state() returns now pair<double,string>

#include <iostream>
#include <cmath>
#include <string>

using namespace std;

class Car
{
private:
    double recommendedPerformance;
public:
    virtual pair<double,string> state()=0;
    //Car tuning(double addedPerformance)return Car(this.recommendedPerformance+addedPerformance)
};
class Steering: public Car{
private:
    double angle;
    double velocity;
    double thresholdPerformance;
public:
    Steering brake(double distanceToObject)
    {
        return Steering(0,velocity*distanceToObject/10.0, thresholdPerformance);
    }
    pair<double,string> state()
    {
        string relation;
        if(velocity/angle>thresholdPerformance)relation=" above ";
        else relation=" below ";
        relation+="threshold performance.";
        return make_pair(abs(velocity/angle-thresholdPerformance),relation);
    }
    Steering(double angle, double velocity, double thresholdPerformance)
    {
        this->angle=angle;
        this->velocity=velocity;
        this->thresholdPerformance=thresholdPerformance;
    }

};

int main()
{
    Steering Tesla(2.67,56,10);
    pair<double,string> info=Tesla.state();
    cout<<info.first<<info.second<<endl;
    //Steering Mercedes(3.43,66,14);
    //Car MercedesUpgrade=Mercedes.tuning(4);
}
