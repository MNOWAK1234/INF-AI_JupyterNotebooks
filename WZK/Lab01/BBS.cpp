#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

bool Prime[10007];     // Information if number is prime
vector<int> Primes;    // List of 4-digit primes
int consecutive0[7];   // Consecutive zeros in the array
int consecutive1[7];   // Consecutive ones in the array
int poker[16];         // Count number of occurences of subarrays of length 4
int amount = 20000;    // Bits array length
vector<int> bits;      // Bits array
int comparision[7][2]; // Helping array for comparing expected string values

void Sieve(int n) // Sieve of Eratostenes
{
    for (int i = 0; i < 10007; i++)
    {
        Prime[i] = true;
    }
    Prime[0] = false;
    Prime[1] = false;
    Prime[2] = true;
    for (int i = 4; i <= n; i += 2)
    {
        Prime[i] = false;
    }
    for (int i = 3; i <= n; i += 2)
    {
        if (Prime[i] == true)
        {
            for (int j = 3 * i; j <= n; j += 2 * i)
            {
                Prime[j] = false;
            }
        }
    }
}

int gcd(int a, int b) // Greatest Common Divisor
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

void Test1()
{
    int count0 = 0;
    for (int i = 0; i < amount; i++)
    {
        if (bits[i] == 0)
        {
            count0++;
        }
    }
    cout << "There are " << count0 << " zeros and " << amount - count0 << " ones" << endl;
    if (count0 >= 10275 || count0 <= 9725)
    {
        cout << "Test 1 failed!" << endl;
    }
    else
    {
        cout << "Test 1 passed!" << endl;
    }
}

void Test2and3()
{
    int ones = 0;  // current 0 streak
    int zeros = 0; // current 1 streak
    bool failed3rd = false;
    bool failed2nd = false;
    for (int i = 0; i < amount; i++)
    {
        // Count consecutive 0s or 1s. Reset the counter after each change
        if (bits[i] == 0)
        {
            zeros++;
            if (ones > 26)
            {
                cout << "Test 3 failed!" << endl;
                cout << "There were " << ones << " '1's in a row" << endl;
                failed3rd = true;
                break;
            }
            else if (ones > 6) // Treat streaks of length greater than 6 as one sum
            {
                ones = 6;
            }
            consecutive1[ones]++;
            ones = 0;
        }
        if (bits[i] == 1)
        {
            ones++;
            if (zeros > 26)
            {
                cout << "Test 3 failed!" << endl;
                cout << "There were " << zeros << " '0's in a row" << endl;
                failed3rd = true;
                break;
            }
            if (zeros > 6) // Treat streaks of length greater than 6 as one sum
            {
                zeros = 6;
            }
            consecutive0[zeros]++;
            zeros = 0;
        }
    }
    // Handle the end of the array
    if (ones > 26)
    {
        cout << "Test 3 failed!" << endl;
        cout << "There were " << ones << " '1's in a row" << endl;
        failed3rd = true;
    }
    else
    {
        if (ones > 6) // Treat streaks of length greater than 6 as one sum
        {
            ones = 6;
        }
        consecutive1[ones]++;
    }
    if (zeros > 26)
    {
        cout << "Test 3 failed!" << endl;
        cout << "There were " << ones << " '1's in a row" << endl;
        failed3rd = true;
    }
    else
    {
        if (zeros > 6) // Treat streaks of length greater than 6 as one sum
        {
            zeros = 6;
        }
        consecutive1[zeros]++;
    }
    if (failed3rd == true)
    {
        return;
    }
    // Test 2 final part
    comparision[1][0] = 2315;
    comparision[1][1] = 2685;
    comparision[2][0] = 1114;
    comparision[2][1] = 1386;
    comparision[3][0] = 527;
    comparision[3][1] = 723;
    comparision[4][0] = 240;
    comparision[4][1] = 384;
    comparision[5][0] = 103;
    comparision[5][1] = 209;
    comparision[6][0] = 103;
    comparision[6][1] = 209;
    for (int i = 1; i <= 6; i++)
    {
        cout << "Consecutive '0's " << i << " times: " << consecutive0[i] << endl;
        if (consecutive0[i] <= comparision[i][0] || consecutive0[i] >= comparision[i][1])
        {
            cout << "Test 2 failed!" << endl;
            failed2nd = true;
        }
        cout << "Consecutive '1's " << i << " times: " << consecutive1[i] << endl;
        if (consecutive1[i] <= comparision[i][0] || consecutive1[i] >= comparision[i][1])
        {
            cout << "Test 2 failed!" << endl;
            failed2nd = true;
        }
    }
    if (failed2nd == false)
    {
        cout << "Test 2 passed!" << endl;
    }
    if (failed3rd == false)
    {
        cout << "Test 3 passed!" << endl;
    }
}

int decimal(int a, int b, int c, int d) // helping function for calculating decimal value
{
    return 8 * a + 4 * b + 2 * c + d;
}

void Test4()
{
    for (int i = 0; i < amount; i += 4)
    {
        int value = decimal(bits[i], bits[i + 1], bits[i + 2], bits[i + 3]);
        poker[value]++;
    }
    for (int i = 0; i < 16; i++)
    {
        cout << "Number of " << i << "s: " << poker[i] << endl;
    }
    double chitest = 0;
    for (int i = 0; i < 16; i++)
    {
        chitest += ((poker[i] - 5000 / 16) * (poker[i] - 5000 / 16)) / (5000 / 16);
    }
    cout << "Chi value: " << chitest << endl;
    if (chitest < 2.16 || chitest > 46.17)
    {
        cout << "Test 4 failed!" << endl;
    }
    else
    {
        cout << "Test 4 passed!" << endl;
    }
}

int main()
{
    srand(time(0));
    // 1. Perform Sieve of Eratostenes
    Sieve(10000);
    // 2. Find all 4-digit primes (congruent to 3 mod 4)
    // Its good when p = 2p' + 1 where p' is also prime
    for (int i = 1000; i < 10000; i++)
    {
        if (Prime[i] == true && i % 4 == 3 && Prime[(i - 1) / 2] == true)
        {
            Primes.push_back(i);
        }
    }
    long long p = 55; // assert that the while loop runs at least 1 time
    long long q = 55;
    long long N = 0;
    int size = Primes.size();
    cout << "Possible prime set size: " << size << endl;
    // 3. Find p and q
    // They should be safe primes with a small gcd((p-3)/2, (q-3)/2) (this makes the cycle length large).
    while (gcd((p - 3) / 2, (q - 3) / 2) > 2)
    {
        int indexP = rand() % size;
        p = Primes[indexP];
        int indexQ = rand() % size;
        q = Primes[indexQ];
    }
    cout << "p: " << p << endl;
    cout << "q: " << q << endl;
    cout << "gcd((p-3)/2, (q-3)/2): " << gcd((p - 3) / 2, (q - 3) / 2) << endl;
    N = p * q;
    cout << "N: " << N << endl;
    // 4. Choose x
    // Since N = p*q, therefore gcd(N, x) = 1 iff x isn't divisible both by p and q
    long long x = 0;
    while (x % p == 0 || x % q == 0)
    {
        x = rand() % 9000 + 1000;
    }
    cout << "x: " << x << endl;
    // 5. Generate bits
    for (int i = 0; i < amount; i++)
    {
        x = (x * x) % N;
        int lsb = x % 2;
        bits.push_back(lsb);
    }
    // 6. Perform Test 1 (about half '0's and half '1's)
    Test1();
    // 7. Perform Test 2 (series and long series)
    Test2and3();
    // 8. Poker Test
    Test4();
}
