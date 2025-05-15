#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

long long gcd(long long a, long long b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

// You should specify the number of possible TestValues
long long PollardRho(long long tests, long long composite)
{
    long long x, y;
    long long c;
    long long d;
    for (long long i = 0; i < tests; i++)
    {
        // 1. Set initial values
        x = 2 + rand() % (composite - 2);
        c = 1 + rand() % (composite - 1);
        y = (x * x) % composite;
        y = (y + c) % composite;
        // 2.Repeat the loop until a divisor is found
        d = gcd(abs(x - y), composite);
        while (d == 1)
        {
            // Update x
            x = (x * x) % composite;
            x = (x + c) % composite;
            // Update y
            y = (y * y) % composite;
            y = (y + c) % composite;
            y = (y * y) % composite;
            y = (y + c) % composite;
            // Calculate gcd
            d = gcd(abs(x - y), composite);
        }
        // 3.Return divisor
        if (d == composite)
            continue;
        else
            return d;
    }
    // 4.Return -1 if no divisor is found
    return -1;
}

// You should specify b for checking b-smoothness
long long QuadraticSieve(long long b, long long composite)
{
    long long res;
    long long exponent;
    long long modulo;
    long long a;
    long long DataAmount;
    long long trial, squared;
    vector<long long> smooth;
    vector<long long> congurence;
    vector<vector<long long>> matrix;
    long long padic;
    vector<pair<long long, long long>> tests;
    long long memory;
    // 1. Calculate smaller primes than b
    vector<long long> SmallPrimes;
    vector<bool> Prime;
    Prime.push_back(false);
    Prime.push_back(false);
    Prime.push_back(true);
    for (int i = 3; i <= b; i++)
    {
        if (i % 2 == 1)
            Prime.push_back(true);
        else
            Prime.push_back(false);
    }
    SmallPrimes.push_back(2);
    for (long long i = 3; i <= b; i += 2)
    {
        if (Prime[i] == true)
        {
            // 2. Check the Euler Criterium
            res = 1;
            modulo = i;
            exponent = (i - 1) / 2;
            a = composite;
            while (exponent > 0)
            {
                if (exponent % 2 == 1)
                {
                    res = res * a;
                    res = res % modulo;
                }
                a = a * a;
                a = a % modulo;
                exponent = exponent / 2;
            }
            if (res == 1)
                SmallPrimes.push_back(i);
            for (int j = 3 * i; j <= b; j += 2 * i)
            {
                Prime[j] = false;
            }
        }
    }
    // 3. Choose data amount
    DataAmount = SmallPrimes.size() + 5;
    // 4. Find B-smooth squares
    long long completed = 0;
    trial = sqrt(composite) + 1;
    while (completed < DataAmount && trial < composite)
    {
        congurence.clear();
        squared = trial * trial;
        squared = squared % composite;
        memory = squared;
        for (long long i = 0; i < SmallPrimes.size(); i++)
        {
            padic = 0;
            while (squared % SmallPrimes[i] == 0)
            {
                squared = squared / SmallPrimes[i];
                padic++;
            }
            padic = padic % 2;
            congurence.push_back(padic);
        }
        if (squared == 1)
        {
            // 5. Prepare right hand side
            for (long long i = 0; i < DataAmount; i++)
            {
                if (i == completed)
                    congurence.push_back(1);
                else
                    congurence.push_back(0);
            }
            matrix.push_back(congurence);
            tests.push_back(make_pair(trial, memory));
            completed++;
        }
        trial++;
    }
    // 6. Solve matrix
    long long FindOne;
    long long row = 0;
    for (long long i = 0; i < SmallPrimes.size(); i++)
    {
        // find row that has a 1 in the ith column
        FindOne = -1;
        for (long long j = row; j < matrix.size(); j++)
        {
            if (matrix[j][i] == 1)
            {
                FindOne = j;
                break;
            }
        }
        // check if there is a one
        if (FindOne != -1)
        {
            swap(matrix[row], matrix[FindOne]);
            // matrix operations
            for (long long j = row + 1; j < matrix.size(); j++)
            {
                if (matrix[j][i] == 1)
                {
                    for (long long k = i; k < matrix[0].size(); k++)
                    {
                        if (matrix[row][k] == 1)
                        {
                            // negate cell
                            if (matrix[j][k] == 1)
                                matrix[j][k] = 0;
                            else
                                matrix[j][k] = 1;
                        }
                    }
                }
            }
            row++;
        }
    }
    // 7. Test all zero rows
    long long x, y;
    for (long long i = row; i < completed; i++)
    {
        x = 1;
        y = 1;
        for (long long j = 0; j < completed; j++)
        {
            // check the right hand side
            if (matrix[i][j + SmallPrimes.size()] == 1)
            {
                x *= tests[j].first;
                y *= tests[j].second;
            }
        }
        y = sqrt(y);
        long long divisor = gcd(x - y, composite);
        if (divisor != 1 && divisor % composite != 0)
        {
            return divisor;
        }
    }
    // no divisors found
    return -1;
}

int main()
{
    srand(time(0));
    int runs = 0;
    long long composite;
    cout << "Enter a large composite number: " << endl;
    cin >> composite;
    cout << "Enter the number of tests to run for Pollard-Rho: " << endl;
    cin >> runs;
    // long long pr = PollardRho(2, 10967535067);
    long long pr = PollardRho(runs, composite);
    if (pr == -1)
        cout << "Divisor not found by Pollard-rho." << endl;
    else
        cout << "I found, that " << pr << " divides " << composite << endl;
    cout << "Specify B for checking B-smoothness in Quadratic Sieve: " << endl;
    int B;
    cin >> B;
    // long long qs = QuadraticSieve(30, 10967535067);
    long long qs = QuadraticSieve(B, composite);
    if (qs == -1)
        cout << "Divisor not found by Quadratic Sieve." << endl;
    else
        cout << "I found, that " << qs << " divides " << composite << endl;
}
