#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct nucleotide
{
    string chain;
    int low, high;
};

string start;
int length;
int amount;
vector<nucleotide> probes;

// Comparison function for sorting based on high value
bool compareByHigh(const nucleotide &a, const nucleotide &b)
{
    return a.high < b.high;
}

bool match(char a, char b)
{
    if (a == 'X' || b == 'X')
        return true;
    if (a == b)
        return true;
    return false;
}

vector<bool> taken;
vector<int> order;

bool traverse(string current, int position, int finished)
{
    if (position == finished)
        return true;
    /*cout << current << endl;
    cout << current.size() << endl;
    for (int i = 0; i < finished - 1; i++)
        cout << taken[i];
    cout << endl;*/
    bool any = false;
    bool found = false;
    bool matches = true;
    for (int i = 0; i < probes.size(); i++)
    {
        if (taken[i] == true || any == true)
            continue;
        if (probes[i].low > position)
            continue; // Too early for this
        if (probes[i].high < position)
        {
            // A probe was missed
            cout << "I am at position " << position << " and I have a problem with: " << probes[i].chain << " " << probes[i].low << " " << probes[i].high << endl;
            return false;
        }
        if (i >= amount)
            any = true;
        // Check if we can match this probe
        matches = true;
        string forMatch = probes[i].chain;
        // No need for matching the last character
        for (int j = 0; j < forMatch.size() - 1; j++)
        {
            if (match(current[position + j], forMatch[j]) == false)
            {
                matches = false;
                break;
            }
        }
        if (matches == true)
        {
            order[position] = i;
            taken[i] = true;
            // Go deeper
            // Add matching probe
            string memory = current;
            current += "X";
            if (forMatch[0] != 'X')
            {
                for (int j = 0; j < forMatch.size(); j++)
                    current[position + j] = forMatch[j];
            }
            position++;
            found = traverse(current, position, finished);
            if (found == false)
            {
                // Retract
                taken[i] = false;
                position--;
                current = memory;
            }
            else
            {
                // Solution found!
                break;
            }
        }
    }
    return found;
}

int main()
{
    ifstream inputFile("TestCase.txt");
    if (!inputFile.is_open())
    {
        cout << "Failed to open the file." << endl;
        return 1;
    }

    // Read length, start, and amount from the file
    inputFile >> length;
    inputFile >> start;
    inputFile >> amount;

    nucleotide curr;
    for (int i = 0; i < amount; i++)
    {
        inputFile >> curr.chain >> curr.low >> curr.high;
        probes.push_back(curr);
    }
    inputFile.close();
    for (int i = 0; i < length; i++)
        order.push_back(-1);

    nucleotide dummy;
    dummy.chain = "";
    for (int i = 0; i < start.size(); i++)
        dummy.chain += "X";
    dummy.low = -1;
    dummy.high = length + 1;
    // Sort probes based on high value
    sort(probes.begin(), probes.end(), compareByHigh);

    /*
    // Output sorted probes
    for (int i = 0; i < amount; i++)
    {
        for (int j = 0; j < probes[i].low; j++)
            cout << " ";
        cout << probes[i].chain << " " << probes[i].low << " " << probes[i].high << endl;
    }

    cout << endl;*/

    // Add dummies
    for (int i = amount; i < length - dummy.chain.size(); i++)
        probes.push_back(dummy);

    string current = start;
    int position = 1;
    for (int i = 0; i < length - dummy.chain.size(); i++)
        taken.push_back(false);
    int finish = length - dummy.chain.size() + 1;
    cout << start << " " << position << " " << finish << endl;
    if (traverse(start, position, finish) == true)
    {
        cout << "Found solution!" << endl;
        cout << start << endl;
        for (int i = 1; i < finish; i++)
        {
            for (int j = 0; j < i; j++)
                cout << " ";
            cout << probes[order[i]].chain << " " << probes[order[i]].low << " " << probes[order[i]].high << endl;
        }
    }
}
