#include <iostream>

using namespace std;

class Glass
{
public:
    string shape;
    double xmax,ymax,zmax;
    int level_of_hardness;
    string usage;
    //default constructor
    Glass()
    {
        this->shape="irregular";
        this->ymax=10.4;
        this->xmax=7.6;
        this->zmax=8.9;
        this->level_of_hardness=9;
        this->usage="window";
        cout<<"Created default glass"<<endl;
    }
    //parametrised
    Glass(double x, double y, double z)
    {
        this->xmax=x;
        this->ymax=y;
        this->zmax=z;
    }
    Glass(string s, double param)
    {
        this->shape=s;
        this->xmax=this->ymax=this->zmax=param;
    }
    Glass(Glass &obj)
    {
        this->shape=obj.shape;
        this->level_of_hardness=obj.level_of_hardness;
    }
    Glass(Glass &obj, int magnify)
    {
        this->shape=obj.shape;
        this->xmax=obj.xmax*magnify;
        this->ymax=obj.ymax*magnify;
        this->zmax=obj.zmax*magnify;
    }
    double volume()
    {
        if(shape=="sphere") return this->xmax*this->ymax*this->zmax*3.14/8;
        else if(shape=="cube")return this->xmax*this->xmax*this->xmax;
        else return this->xmax*this->ymax*this->zmax;
    }
};

int main()
{
    Glass Random;
    Glass One(10,10,10);
    cout<<One.volume()<<endl;
    Glass Two("cube", 10);
    cout<<Two.volume()<<endl;
    Glass Copied(Random);
    cout<<Copied.level_of_hardness<<endl;
    Glass Sphere("sphere", 10);
    cout<<Sphere.volume()<<endl;
    Glass Magnified(Sphere,2);
    cout<<Magnified.volume()<<endl;
}
