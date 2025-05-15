#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Shiritori
{
private:
    vector<string>words;
    bool gameOver;
public:
    Shiritori()
    {
        this->gameOver=false;
        this->words.clear();
    }
    string correct(string word)
    {
        string corrected="";
        bool upper=false;
        for(int i=0; i<word.size(); i++)
        {
            if(int(word[i])>=int('A') && int(word[i])<=int('Z'))
            {
                upper=true;
                int diff=int(word[i])-int('A');
                corrected+=char(int('a')+diff);
            }
            else corrected+=word[i];
        }
        if(upper)
        {
            cout<<"Try to use only lowercase letters."<<endl;
            cout<<"This time is fine, but please be gentle to your Shiritori."<<endl;
        }
        return corrected;
    }
    void play(string word)
    {
        bool found=false;
        string corrected=correct(word);
        int siz=this->words.size();
        if(siz==0)
        {
            if(corrected[corrected.size()-1]=='x')
            {
                this->gameOver=true;
                cout<<"You lose, because there aren't any valid words that start with an x."<<endl;
                cout<<"Game over"<<endl;
            }
            else
            {
                this->words.push_back(corrected);
            }
        }
        else
        {
            for(int i=0; i<siz; i++)
            {
                if(this->words[i]==word)
                {
                    found=true;
                }
            }
            if(!found)
            {
                string s=this->words[siz-1];
                if(s[s.size()-1]==word[0])
                {
                    if(corrected[corrected.size()-1]=='x')
                    {
                        this->gameOver=true;
                        cout<<"You lose, because there aren't any valid words that start with an x."<<endl;
                        cout<<"Game over"<<endl;
                    }
                    else
                    {
                        this->words.push_back(corrected);
                    }
                }
                else
                {
                    this->gameOver=true;
                    cout<<"Your word "<<corrected<<" doesn't match your oppenent's one."<<endl;
                    cout<<"Game over"<<endl;
                }
            }
            else
            {
                this->gameOver=true;
                cout<<"Word "<<corrected<<" already in use!"<<endl;
                cout<<"Game over"<<endl;
            }
        }
    }
    void restart()
    {
        this->words.clear();
        this->gameOver=false;
        cout<<"Game restarted."<<endl;
    }
    void printWords()
    {
        for(int i=0; i<this->words.size(); i++)
        {
            cout<<this->words[i]<<", ";
        }
        cout<<endl;
    }
    bool lost()
    {
        return this->gameOver;
    }
};

class Control
{
public:
    void Loop()
    {
        cout<<"This is a shiritari game."<<endl;
        cout<<"User's Guide:"<<endl;
        cout<<"Type a word or:"<<endl;
        cout<<"1. exit to exit the game."<<endl;
        cout<<"2. restart to exit the game."<<endl;
        cout<<"3. hint to see the words already in use."<<endl;
        cout<<"Have fun!"<<endl;
        string input="a";
        bool exit=false;
        while(1)
        {
            cout<<"Enter a word to start:"<<endl;
            vector <string> words;
            Shiritori NewGame;
            bool gameOver=0;
            string userInput;
            while(!gameOver)
            {
                cin>>userInput;
                if(userInput=="exit")
                {
                    exit=true;
                    break;
                }
                else if(userInput=="restart")
                {
                    NewGame.restart();
                }
                else if(userInput=="hint")
                {
                    NewGame.printWords();
                }
                else
                {
                    NewGame.play(userInput);
                    gameOver=NewGame.lost();
                }
            }
            if(exit)
            {
                cout<<"Game aborted."<<endl;
                break;
            }
        }
    }
};

int main()
{
    Control Play;
    Play.Loop();
}
