#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool match(char a, char b)
{
    if (a == 'X' || b == 'X')
        return true;
    if (a == b)
        return true;
    return false;
}

int length, probeSize;
string start;
string trash;
string probe;
int spectrum;
vector<string> probes;
vector<bool> taken;
vector<int> order;

bool traverse(string current, int position, int finished)
{
    if ((int)current.size() == finished)
    {
        return true;
    }
    bool found = false;
    bool matches = true;
    for (int i = 0; i < (int)probes.size(); i++)
    {
        if (taken[i] == true)
            continue;
        // Check if we can match this probe
        matches = true;
        string forMatch = probes[i];
        // No need for matching the last character
        for (int j = 0; j < (int)forMatch.size() - 1; j++)
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
                for (int j = 0; j < (int)forMatch.size(); j++)
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
                return true;
            }
        }
    }
    return found;
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> trash >> length;
    cin >> trash >> start;
    cin >> trash >> probeSize;
    cin >> trash >> spectrum;
    int probeNumber = spectrum;
    for (int i = 0; i < probeNumber; i++)
    {
        cin >> probe;
        probes.push_back(probe);
    }

    for (int i = 0; i < length; i++)
        order.push_back(-1);

    string current = start;
    int position = 1;
    for (int i = 0; i < (int)probes.size(); i++)
        taken.push_back(false);
    int finish = length;
    int amount = length - probeSize + 1;
    if (traverse(start, position, finish) == true)
    {
        cout << start;
        int skipped = 1;
        for (int i = 1; i < amount; i++)
        {
            if (probes[order[i]][0] == 'X')
            {
                skipped++;
                continue;
            }
            else
            {
                cout << probes[order[i]].substr(probeSize - skipped, skipped);
                skipped = 1;
            }
        }
        cout << endl;
    }
    else
    {
        cout << "Solution not found!" << endl;
    }
}