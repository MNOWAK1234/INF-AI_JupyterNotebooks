#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int length, probeSize;
string start;
string trash;
string probe;
int spectrum;
vector<string> probes;
vector<bool> taken;

int similarity[400][400];
int depth, mn;
int maxdepth;
int bestIndex;

void calcSimilarity()
{
    for (int i = 0; i < (int)probes.size(); i++)
    {
        for (int j = 0; j < (int)probes.size(); j++)
        {
            if (i == j)
                continue;
            string first = probes[i];
            string second = probes[j];
            for (int k = 1; k < probeSize - 1; k++)
            {
                bool match = true;
                for (int l = k; l < probeSize; l++)
                {
                    if (first[l] != second[l - k])
                    {
                        match = false;
                        break;
                    }
                }
                if (match == true)
                {
                    similarity[i][j] = k;
                    break;
                }
            }
            if (similarity[i][j] == 0)
                similarity[i][j] = probeSize;
        }
    }
}

// DFS with specified depth
int ReducedBB(int prevIndex, int depth, int score)
{
    if (depth == 0 || score > mn)
        return score;
    depth--;
    int passedScore;
    int retreivedScore;
    int bestScore = 10000;
    for (int i = 0; i < (int)probes.size(); i++)
    {
        if (taken[i] == true)
            continue;
        passedScore = score + similarity[prevIndex][i];
        taken[i] = true;
        retreivedScore = ReducedBB(i, depth, passedScore);
        taken[i] = false;
        if (retreivedScore < bestScore)
        {
            bestScore = retreivedScore;
            if (score == 0)
            {
                bestIndex = i;
                mn = bestScore;
            }
        }
    }
    return bestScore;
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
    string current = start;
    int previndex = -1;
    for (int i = 0; i < (int)probes.size(); i++)
    {
        if (probes[i] != start)
            taken.push_back(false);
        else
        {
            previndex = i;
            taken.push_back(true);
        }
    }
    // Add starting probe to the spectrum if it isn't there yet
    if (previndex == -1)
    {
        previndex = (int)probes.size();
        probes.push_back(start);
        taken.push_back(true);
    }
    calcSimilarity();
    // Adjust depth to avoid TLE
    if (length <= 100)
        maxdepth = 4;
    else
        maxdepth = 1;
    // Construct the answer one by one
    while ((int)current.size() < length)
    {
        bestIndex = -1;
        mn = 100000;
        depth = min(maxdepth, length - (int)current.size());
        int score = ReducedBB(previndex, depth, 0);
        // Add padding if necessaru
        if (bestIndex == -1)
        {
            while ((int)current.size() < length)
                current += "G";
            break;
        }
        int offset = similarity[previndex][bestIndex];
        current += probes[bestIndex].substr(probeSize - offset, offset);
        previndex = bestIndex;
        taken[bestIndex] = true;
    }
    // Trim if necessary
    current = current.substr(0, length);
    cout << current << endl;
}
