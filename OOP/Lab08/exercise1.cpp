#include <iostream>

using namespace std;

class Game{
public:
    virtual string result()=0;
};

class FakeFootballGame : public Game {
public:
    virtual string result()
    {
        return "1 - 1";
    }
};

int main(){
    FakeFootballGame fakeGame;
    if(fakeGame.result()=="1 - 1") cout<<"Correct"<<endl;
    else cout<<"Incorrect"<<endl;
}
