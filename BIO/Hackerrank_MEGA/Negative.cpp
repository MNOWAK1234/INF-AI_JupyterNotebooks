#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int length, probeSize;
string start;
string trash;
string probe;
int spectrum;
vector<string> probes;
vector<bool> taken;
vector<int> visited;

vector<vector<int>> similarity;
int previndex = 0;
vector<int> nextProbe;

void calcSimilarity()
{
    for (int i = 0; i < (int)probes.size(); i++)
    {
        for (int j = 0; j < (int)probes.size(); j++)
        {
            if (i == j)
            {
                similarity[i][j] = 4000;
                continue;
            }
            if (j == previndex)
            {
                similarity[i][j] = 2000;
                continue;
            }
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
                    similarity[i][j] = k * k;
                    break;
                }
            }
            if (similarity[i][j] == 0)
                similarity[i][j] = probeSize;
        }
    }
}

long long Hungarian(int sizeGroup1, int sizeGroup2, vector<vector<int>> cost)
{
    long long infinity = 1000000000000000000;
    long long minimum;
    // 1. Prepare matrix structure
    //---By making it square
    if (sizeGroup1 < sizeGroup2)
    {
        vector<int> temporal;
        for (int i = 0; i < sizeGroup2; i++)
        {
            temporal.push_back(2);
        }
        int difference = sizeGroup2 - sizeGroup1;
        for (int i = 0; i < difference; i++)
        {
            cost.push_back(temporal);
        }
    }
    if (sizeGroup1 > sizeGroup2)
    {
        int difference = sizeGroup1 - sizeGroup2;
        for (int i = 0; i < sizeGroup1; i++)
        {
            for (int j = 0; j < difference; j++)
            {
                cost[i].push_back(2);
            }
        }
    }
    int size = max(sizeGroup1, sizeGroup2);
    vector<vector<int>> matrix = cost;
    // 2. Subtract smallest element from each row
    for (int i = 0; i < size; i++)
    {
        minimum = infinity;
        for (int j = 0; j < size; j++)
        {
            if (matrix[i][j] < minimum)
            {
                minimum = matrix[i][j];
            }
        }
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] -= minimum;
        }
    }
    // 3. Subtract smallest element from each column
    for (int i = 0; i < size; i++)
    {
        minimum = infinity;
        for (int j = 0; j < size; j++)
        {
            if (matrix[j][i] < minimum)
            {
                minimum = matrix[j][i];
            }
        }
        for (int j = 0; j < size; j++)
        {
            matrix[j][i] -= minimum;
        }
    }
    // 4. Prepare checking for the optimal solution
    vector<int> column;
    vector<int> row;
    vector<int> markedZeroColumn;
    vector<int> markedZeroRow;
    vector<int> primedZero;
    for (int i = 0; i < size; i++)
    {
        column.push_back(0);
        row.push_back(0);
        markedZeroColumn.push_back(-1);
        markedZeroRow.push_back(-1);
        primedZero.push_back(-1);
    }
    int lines = 0;
    // 5. Start checking
    //---Cross all the 0s in the matrix
    //---If the number of lines used is equal to the size of the group
    //---Then the marked zeros are the optimal solution
    while (lines < size)
    {
        // 6. Empty helping arrays
        //---This must be done, because after augmenting we need to delete all lines
        for (int i = 0; i < size; i++)
        {
            column[i] = 0;
            row[i] = 0;
            primedZero[i] = -1;
        }
        // 7. Find the marked zeros in each row
        //---Those are assignments in the solution
        for (int i = 0; i < size; i++)
        {
            // Check if there is an already marked zero in this row
            if (markedZeroRow[i] != -1)
            {
                // Cross the appropriate column
                column[markedZeroRow[i]] = 1;
            }
            else
            {
                // Try to find an unmarked zero in this row
                for (int j = 0; j < size; j++)
                {
                    if (matrix[i][j] == 0 && column[j] != 1)
                    {
                        lines++;
                        column[j] = 1;
                        markedZeroRow[i] = j;
                        markedZeroColumn[j] = i;
                        // There is no need to check the other elements in this row
                        break;
                    }
                }
            }
        }
        // 8. Start finding uncovered zeros
        bool foundZero = true;
        while (foundZero == true)
        {
            foundZero = false;
            // This variable is used to break the loop
            // After an augmenting path was found
            // This variable is needed to exit the nested loops
            bool startAgain = false;
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    if (matrix[i][j] == 0 && row[i] != 1 && column[j] != 1)
                    {
                        foundZero = true;
                        // 9. Classify this zero
                        if (markedZeroRow[i] != -1)
                        {
                            // 10. Handle the first case
                            //----If the zero is on the same row as a marked zero
                            //----Then cover the corresponding row
                            //----And uncover the column of the marked zero
                            //----Denote this zero as 'primed'
                            //----That means it may be used in augmenting paths
                            column[markedZeroRow[i]] = 0;
                            row[i] = 1;
                            primedZero[i] = j;
                            // There is no need to check other elements in this row
                            break;
                        }
                        else
                        {
                            // 11. Handle the second case
                            //----Find augmenting path from this zero
                            //----Alternating between marked and primed zeros
                            int foundRow = i;
                            int foundColumn = j;
                            while (markedZeroColumn[foundColumn] != -1)
                            {
                                int primedRow = markedZeroColumn[foundColumn];
                                int primedColumn = primedZero[primedRow];
                                // Make found zero the marked zero
                                markedZeroColumn[foundColumn] = foundRow;
                                markedZeroRow[foundRow] = foundColumn;
                                // Augument further from the primed zero
                                foundRow = primedRow;
                                foundColumn = primedColumn;
                            }
                            // Make the last primed zero the marked zero
                            markedZeroColumn[foundColumn] = foundRow;
                            markedZeroRow[foundRow] = foundColumn;
                            // 12. Update line count
                            lines++;
                            // 13. Exit the loop
                            startAgain = true;
                            break;
                        }
                    }
                }
                // 14. Continue exiting
                if (startAgain == true)
                    break;
            }
            // 15. Continue exiting
            if (startAgain == true)
                break;
        }
        // 16. Check if the optimal solution was found
        if (lines == size)
        {
            break;
        }
        // 17. Else reweight the values
        //----Find the minimum uncovered value
        //----Then subtract it from the uncovered elements
        //----And add it to all the elements at the intersections
        minimum = infinity;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (row[i] == 0 && column[j] == 0)
                {
                    if (matrix[i][j] < minimum)
                    {
                        minimum = matrix[i][j];
                    }
                }
            }
        }
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (row[i] == 0 && column[j] == 0)
                {
                    matrix[i][j] -= minimum;
                }
                else if (row[i] == 1 && column[j] == 1)
                {
                    matrix[i][j] += minimum;
                }
            }
        }
    }
    // 18. Calculate the result
    long long result = 0;
    for (int i = 0; i < size; i++)
    {
        result += cost[i][markedZeroRow[i]];
        // cout << i << " " << markedZeroRow[i] << endl;
        nextProbe.push_back(markedZeroRow[i]);
        // cout << probes[i] << " " << probes[markedZeroRow[i]] << endl;
    }
    return result;
}

// returns the number of joint components
int components(int vertices, vector<int> next)
{
    for (int i = 0; i < 400; i++)
        visited[i] = 0;
    int count = 0;
    for (int i = 0; i < vertices; i++)
    {
        if (visited[i] == 0)
        {
            count++;
            visited[i] = 1;
            int nextNode = next[i];
            while (visited[nextNode] == 0)
            {
                visited[nextNode] = 1;
                nextNode = next[nextNode];
            }
        }
    }
    return count;
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
    int startindex = 0;
    for (int i = 0; i < probeNumber; i++)
    {
        cin >> probe;
        probes.push_back(probe);
    }
    // Populate similarity matrix
    //(Cost of joining 2 probes in particular order)
    vector<int> temp;
    for (int i = 0; i < 400; i++)
        temp.push_back(100);
    for (int i = 0; i < 400; i++)
        similarity.push_back(temp);
    for (int i = 0; i < 400; i++)
        visited.push_back(0);
    // Find starting probe
    string current = start;
    for (int i = 0; i < (int)probes.size(); i++)
    {
        if (probes[i] != start)
            taken.push_back(false);
        else
        {
            previndex = i;
            startindex = i;
            taken.push_back(true);
        }
    }
    calcSimilarity();
    int bestindex = previndex;

    // Calculate optimal assignment
    long long ans = Hungarian(spectrum, spectrum, similarity);
    // Get the number of joint components
    int component = components(spectrum, nextProbe);
    // Create a path out of the first component
    for (int i = 0; i < 400; i++)
        visited[i] = 0;
    visited[startindex] = 1;
    vector<int> path;
    int currNode = startindex;
    int nextNode = nextProbe[currNode];
    while (visited[nextNode] == 0)
    {
        nextNode = nextProbe[currNode];
        path.push_back(currNode);
        visited[currNode] = 1;
        currNode = nextNode;
    }
    vector<int> nextComponent;
    int minimum = 1000;
    int after, before;
    string created = start;
    int offset;
    // Join other components one by one
    //  Be greedy and always join the current best
    for (int k = 1; k < component; k++)
    {
        nextComponent.clear();
        minimum = 1000;
        for (int i = 0; i < spectrum; i++)
        {
            if (visited[i] == 0)
            {
                for (int j = 0; j < spectrum; j++)
                {
                    if (visited[j] == 1)
                    {
                        if (similarity[j][i] < minimum)
                        {
                            minimum = similarity[j][i];
                            before = j;
                            after = i;
                        }
                    }
                }
            }
        }
        // Create the new path out of the second joint component
        visited[after] = 1;
        currNode = after;
        nextNode = nextProbe[currNode];
        while (visited[nextNode] == 0)
        {
            nextNode = nextProbe[currNode];
            nextComponent.push_back(currNode);
            visited[currNode] = 1;
            currNode = nextNode;
        }
        // Place this path inside
        int target = before;
        auto it = find(path.begin(), path.end(), target);
        if (it != path.end())
        {
            path.insert(it + 1, nextComponent.begin(), nextComponent.end());
        }
    }
    // Construct the answer
    for (int i = 1; i < path.size(); i++)
    {
        if (similarity[path[i - 1]][path[i]] > probeSize)
        {
            similarity[path[i - 1]][path[i]] = probeSize;
        }
        offset = sqrt(similarity[path[i - 1]][path[i]]);
        current += probes[path[i]].substr(probeSize - offset, offset);
    }
    // Trim if necessary
    current = current.substr(0, length);
    // Add padding if necessary
    while (current.size() < length)
        current += "G";
    cout << current << endl;
}
