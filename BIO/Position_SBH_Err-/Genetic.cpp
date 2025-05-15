#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstdlib>
#include <time.h>
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>
#include <fstream>

using namespace std;

#define CROSSES 2500
#define POPULATION 5000
#define BEST 5000
#define STABLE 400
#define ITERATIONS1 3000
#define ITERATIONS2 1000
#define MAXDEPTH 2

vector<int> visited1;
vector<int> visited2;
vector<int> child1;
vector<int> child2;

string start;
int length;
int amount;
vector<string> probes;
vector<int> lows;
vector<int> highs;
vector<vector<int>> similarity;
vector<vector<int>> similaritySquared;
int range;
vector<vector<int>> similarityRanged;
int probeSize;
int startIndex;
vector<int> permutation;
int permutationSize;
vector<bool> taken;
int bestIndex;
int minimum;
vector<int> HungarianNext;
vector<int> visited;

class Solution
{
public:
    vector<int> points;
    double sum;
    Solution() // default constructor is necessary for resize to work
    {
        this->points = vector<int>();
        this->sum = 0;
    }
    Solution(vector<int> points, double sum)
    {
        this->points = points;
        this->sum = sum;
    }
    void display()
    {
        cout << sum << endl;
    }
    void CalcValue()
    {
        this->sum = 0.0;
        int position = 2;
        for (int i = 1; i < permutationSize; i++)
        {
            // Matching error
            int matchingError = similarity[this->points[i - 1]][this->points[i]];
            this->sum += (matchingError * matchingError);
            // Add position error
            position += matchingError;
            if (position < lows[points[i]])
            {
                this->sum += probeSize * probeSize;
            }
            if (position > highs[points[i]])
            {
                this->sum += probeSize * probeSize;
            }
            // this->sum += max(0, lows[points[i]] - position);
            // this->sum += max(0, position - highs[points[i]]);
        }
        // this->sum += (position - length) * (position - length);
    }
    bool operator<(const Solution &a) const
    {
        return sum < a.sum;
    }
    bool operator==(const Solution &a) const
    {
        if (sum != a.sum)
            return false;
        else
        {
            for (int i = 0; i < (int)this->points.size(); i++)
            {
                if (points[i] != a.points[i])
                    return false;
            }
        }
        return true;
    }
};

vector<Solution> solutions;
queue<Solution> best_of_generation;
vector<Solution> extinct;

void checkSolution(Solution a)
{
    vector<int> used;
    for (int i = 0; i < amount; i++)
        used.push_back(0);
    for (int i = 0; i < a.points.size(); i++)
    {
        used[a.points[i]]++;
        if (used[a.points[i]] > 1)
        {
            cout << "zle jest " << a.points[i] << " a ja juz przetworzylem " << i << endl;
        }
    }
}

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
            similaritySquared[i][j] = similarity[i][j] * similarity[i][j];
            similarityRanged[i][j] = similarity[i][j] + min(max(0, range - abs(lows[i] - lows[j])), probeSize);
        }
        similarity[i][i] = 10000;
        similaritySquared[i][i] = 10000;
        similarityRanged[i][i] = 10000;
    }
}

void addRandom()
{
    for (int i = 0; i < POPULATION; i++)
    {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        mt19937 g(seed);
        shuffle(permutation.begin(), permutation.end(), g);
        Solution randomized(permutation, 0);
        randomized.CalcValue();
        solutions.push_back(randomized);
    }
}

int ReducedBB(int prevIndex, int depth, int score)
{
    if (depth == 0 || score > minimum)
        return score;
    depth--;
    int passedScore;
    int retreivedScore;
    int localBestScore = 10000;
    for (int i = 0; i < (int)probes.size(); i++)
    {
        if (taken[i] == true)
            continue;
        passedScore = score + similarityRanged[prevIndex][i];
        taken[i] = true;
        retreivedScore = ReducedBB(i, depth, passedScore);
        taken[i] = false;
        if (retreivedScore < localBestScore)
        {
            localBestScore = retreivedScore;
            // If we are choosing the first next probe
            // Remember its index
            if (score == 0)
            {
                bestIndex = i;
                minimum = localBestScore;
            }
        }
    }
    return localBestScore;
}

void addReducedBB()
{
    minimum = 10000000;
    vector<int> path;
    path.push_back(startIndex);
    int prevIndex = startIndex;
    int depth;
    while ((int)path.size() < amount)
    {
        minimum = 100000;
        // Avoid searching after the path is finished
        depth = min(MAXDEPTH, amount - (int)path.size());
        int score = ReducedBB(prevIndex, depth, 0);
        prevIndex = bestIndex;
        path.push_back(prevIndex);
        taken[bestIndex] = true;
    }
    Solution fromBB(path, 0);
    fromBB.CalcValue();
    solutions.push_back(fromBB);
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
        HungarianNext.push_back(markedZeroRow[i]);
        // cout << probes[i] << " " << probes[markedZeroRow[i]] << endl;
    }
    return result;
}

// returns the number of joint components
int components(int vertices, vector<int> next)
{
    for (int i = 0; i < amount; i++)
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

void addHungarian()
{
    long long ans = Hungarian(amount, amount, similarity);
    int component = components(amount, HungarianNext);
    for (int i = 0; i < amount; i++)
        visited[i] = 0;
    visited[startIndex] = 1;
    vector<int> path;
    int currNode = startIndex;
    int nextNode = HungarianNext[currNode];
    while (visited[nextNode] == 0)
    {
        nextNode = HungarianNext[currNode];
        path.push_back(currNode);
        visited[currNode] = 1;
        currNode = nextNode;
    }
    vector<int> nextComponent;
    minimum = 1000;
    int after, before;
    for (int k = 1; k < component; k++)
    {
        nextComponent.clear();
        minimum = 1000000;
        for (int i = 0; i < amount; i++)
        {
            if (visited[i] == 0)
            {
                for (int j = 0; j < amount; j++)
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
        visited[after] = 1;
        currNode = after;
        nextNode = HungarianNext[currNode];
        while (visited[nextNode] == 0)
        {
            nextNode = HungarianNext[currNode];
            nextComponent.push_back(currNode);
            visited[currNode] = 1;
            currNode = nextNode;
        }
        int target = before;
        auto it = find(path.begin(), path.end(), target);
        if (it != path.end())
        {
            path.insert(it + 1, nextComponent.begin(), nextComponent.end());
        }
    }
    Solution assignment(path, 0);
    checkSolution(assignment);
    assignment.CalcValue();
    solutions.push_back(assignment);
}

void prepareCross()
{
    for (int j = 0; j < permutationSize; j++)
    {
        visited1.push_back(0);
        child1.push_back(0);
        visited2.push_back(0);
        child2.push_back(0);
    }
}

void Cross()
{
    for (int i = 0; i < CROSSES; i++)
    {
        // Select parents
        int parent1 = rand() % BEST;
        int parent2 = rand() % POPULATION;
        for (int j = 0; j < permutationSize; j++)
        {
            visited1[j] = 0;
            visited2[j] = 0;
        }
        // Select 2 points
        int a = rand() % permutationSize;
        int b = rand() % permutationSize;
        int index = a;
        while (index != b)
        {
            // Mark visited sequences for both children
            visited1[solutions[parent1].points[index]] = 1;
            visited2[solutions[parent2].points[index]] = 1;
            // Duplicate a part of a parent
            child1[index] = solutions[parent1].points[index];
            child2[index] = solutions[parent2].points[index];
            // Move forward
            index = (index + 1) % permutationSize;
        }
        visited1[solutions[parent1].points[index]] = 1;
        visited2[solutions[parent2].points[index]] = 1;
        // Duplicate a part of a parent
        child1[index] = solutions[parent1].points[index];
        child2[index] = solutions[parent2].points[index];
        // Move forward
        index = (index + 1) % permutationSize;
        int index1 = index;
        int index2 = index;
        // Add unused points
        for (int j = 0; j < permutationSize; j++)
        {
            if (visited1[solutions[parent2].points[index]] == 0)
            {
                child1[index1] = solutions[parent2].points[index];
                index1 = (index1 + 1) % permutationSize;
            }
            if (visited2[solutions[parent1].points[index]] == 0)
            {
                child2[index2] = solutions[parent1].points[index];
                index2 = (index2 + 1) % permutationSize;
            }
            // Move forward
            index = (index + 1) % permutationSize;
        }
        if (child1.size() == 0)
            return;
        if (child2.size() == 0)
            return;
        Solution crossed1(child1, 0);
        Solution crossed2(child2, 0);
        crossed1.CalcValue();
        crossed2.CalcValue();
        solutions.push_back(crossed1);
        solutions.push_back(crossed2);
    }
}

void mutation(int which)
{
    int option;
    int a, b;
    vector<int> change(0);
    option = rand() % 100;
    option += 1000;
    if (option < 33) // Swap 2 points
    {
        a = rand() % permutationSize;
        b = rand() % permutationSize;
        swap(solutions[which].points[a], solutions[which].points[b]);
    }
    else if (option < 66) // Move one point into an entirely new place
    {
        a = rand() % permutationSize;
        b = rand() % permutationSize;
        change.push_back(solutions[which].points[a]);
        solutions[which].points.erase(solutions[which].points.begin() + a);
        solutions[which].points.insert(solutions[which].points.begin() + b, change[0]);
    }
    else if (option < 100) // Place a chunk of a solution into a different place
    {
        a = rand() % permutationSize;
        b = rand() % permutationSize;
        if (b < a)
            swap(a, b);
        for (int i = a; i < b; i++)
            change.push_back(solutions[which].points[i]);
        solutions[which].points.erase(solutions[which].points.begin() + a, solutions[which].points.begin() + b);
        a = rand() % solutions[which].points.size();
        solutions[which].points.insert(solutions[which].points.begin() + a, change.begin(), change.end());
    }
    solutions[which].CalcValue();
}

void massExtinction()
{
    vector<int> tmp;
    Solution help(tmp, 0);
    best_of_generation.push(solutions[0]);
    if (best_of_generation.size() == STABLE)
    {
        if (abs(solutions[0].sum - best_of_generation.front().sum) < 40)
        {
            extinct.push_back(solutions[0]);
            solutions.clear();
            addRandom();
            best_of_generation = queue<Solution>(); // clears the queue
        }
        else
        {
            best_of_generation.pop();
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
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
    probeSize = start.size();
    string chain;
    int low, high;
    for (int i = 0; i < amount; i++)
    {
        inputFile >> chain >> low >> high;
        probes.push_back(chain);
        lows.push_back(low);
        highs.push_back(high);
        taken.push_back(false);
        visited.push_back(0);
    }
    // Add starting sequence
    probes.push_back(start);
    lows.push_back(1);
    highs.push_back(1);
    taken.push_back(true);
    visited.push_back(0);
    amount++;
    startIndex = amount - 1;
    range = highs[0] - lows[0];
    inputFile.close();
    cout << "Data read successfully" << endl;
    // Calculate similarities between probes
    vector<int> tmp;
    for (int i = 0; i < amount; i++)
        tmp.push_back(0);
    for (int i = 0; i < amount; i++)
        similarity.push_back(tmp);
    similaritySquared = similarity;
    similarityRanged = similarity;
    for (int i = 0; i < probes.size(); i++)
        permutation.push_back(i);
    calcSimilarity();
    cout << "Similarity calculated successfully" << endl;
    // Prepare variables for genetic algorithm
    permutationSize = probes.size();
    addRandom();
    cout << "Random solutions added successfully" << endl;
    addReducedBB();
    cout << "RBB added successfully" << endl;
    addHungarian();
    cout << "Hungarian added successfully" << endl;
    prepareCross();

    sort(solutions.begin(), solutions.end());

    for (int k = 0; k < ITERATIONS1; k++)
    {
        if (k % 10 == 0)
        {
            cout << k << ": ";
            solutions[0].display();
            checkSolution(solutions[0]);
        }
        for (int i = solutions.size() - 1; i >= 1; i--)
        {
            if (solutions[i - 1].sum == solutions[i].sum)
            {
                mutation(i);
            }
        }
        Cross();
        sort(solutions.begin(), solutions.end());
        solutions.erase(solutions.begin() + POPULATION, solutions.end());
        massExtinction();
    }
    cout << "Extinct:" << endl; // adding best results to population
    for (int i = 0; i < (int)extinct.size(); i++)
    {
        extinct[i].display();
        for (int j = 0; j < 50; j++)
            solutions.push_back(extinct[i]);
    }
    sort(solutions.begin(), solutions.end());
    solutions.erase(solutions.begin() + POPULATION, solutions.end());

    for (int k = 0; k < ITERATIONS2; k++)
    {
        checkSolution(solutions[0]);
        if (k % 10 == 0)
        {
            cout << k << endl;
            solutions[0].display();
        }
        for (int i = solutions.size() - 1; i >= 1; i--)
        {
            if (solutions[i - 1].sum == solutions[i].sum)
                mutation(i);
        }
        Cross();
        sort(solutions.begin(), solutions.end());
        solutions.erase(solutions.begin() + POPULATION, solutions.end());
    }

    cout << "Final:" << endl;
    cout << solutions[0].sum << endl;
    int position = 2;
    cout << "1: " << start << endl;
    for (int i = 1; i < permutationSize; i++)
    {
        cout << position << ": " << solutions[0].points[i] << " " << probes[solutions[0].points[i]] << " low: " << lows[solutions[0].points[i]] << " high: " << highs[solutions[0].points[i]] << " similarity ";
        int error = similarity[solutions[0].points[i - 1]][solutions[0].points[i]];
        position += error;
        cout << error << endl;
    }
    cout << endl;
}
