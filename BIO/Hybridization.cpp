#include <iostream>
#include <vector>
#include <string>
#include <set>

using namespace std;

int length, probeSize;
string start;
string trash;
string probe;
vector<string> probes;
vector<pair<string, string>> dividedProbes;
string half1, half2;
int position;
set<string> nodes;

vector<pair<int, int>> lss[1000007];
int visited[1000007];
int VisitedEdge[2000007];
vector<int> path;
vector<int> positionOfProbe[10000];
vector<string> storedSet;

void DFS(int current)
{
    for (int i = (int)lss[current].size() - 1; i >= 0; i--)
    {
        // Traverse all new edges which in fact represent probes
        if (VisitedEdge[lss[current][i].second] == 0)
        {
            VisitedEdge[lss[current][i].second] = 1;
            DFS(lss[current][i].first);
        }
    }
    path.push_back(current);
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> trash >> length;
    cin >> trash >> start;
    cin >> trash >> probeSize;
    int probeNumber = length - probeSize + 1;
    for (int i = 0; i < probeNumber; i++)
    {
        cin >> probe;
        half1 = probe.substr(0, probeSize - 1);
        half2 = probe.substr(1, probeSize - 1);
        probes.push_back(probe);
        dividedProbes.push_back(make_pair(half1, half2));
        nodes.insert(half1);
        nodes.insert(half2);
    }
    for (const auto &str : nodes)
    {
        storedSet.push_back(str);
    }
    // Construct de Bruijn graph
    for (int i = 0; i < probeNumber; i++)
    {
        auto front = nodes.find(dividedProbes[i].first);
        auto back = nodes.find(dividedProbes[i].second);
        position = distance(nodes.begin(), front);
        positionOfProbe[position].push_back(i);
        lss[position].push_back(make_pair(distance(nodes.begin(), back), i));
    }
    // Find first vertex
    half1 = start.substr(0, probeSize - 1);
    auto where = nodes.find(start.substr(0, probeSize - 1));
    position = distance(nodes.begin(), where);
    // Construct Eulerian path
    DFS(position);
    // Now print the answer
    cout << storedSet[path[path.size() - 1]];
    for (int i = (int)path.size() - 2; i >= 0; i--)
    {
        // display only last element
        cout << storedSet[path[i]][storedSet[path[i]].size() - 1];
    }
}