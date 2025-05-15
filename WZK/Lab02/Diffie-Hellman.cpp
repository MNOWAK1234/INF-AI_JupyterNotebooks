#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

bool Prime[10007];  // Information if number is prime
vector<int> Primes; // List of 4-digit primes

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

long long fastexp(long long a, long long exponent, long long modulo)
{
    long long res = 1;
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
    return res;
}

int main()
{
    srand(time(0));
    // 1. Perform Sieve of Eratostenes
    Sieve(10000);
    // 2. Find all 4-digit primes
    for (int i = 1000; i < 10000; i++)
    {
        if (Prime[i] == true)
        {
            Primes.push_back(i);
        }
    }
    long long n = 0;
    int size = Primes.size();
    cout << "Possible prime set size: " << size << endl;
    // 3. Choose p
    int indexN = rand() % size;
    n = Primes[indexN];
    cout << "n: " << n << endl;
    // 4. Find g
    // Unfortunately it is done by brute force
    bool gfound = false;
    long long g = 0;
    vector<bool> rest;
    for (int i = 0; i < n; i++)
    {
        rest.push_back(false);
    }
    while (gfound == false)
    {
        gfound = true;
        for (int i = 0; i < rest.size(); i++)
        {
            rest[i] = false;
        }
        g = rand() % (n - 2) + 2;
        long long power = g;
        rest[power] = true;
        // check rests for exponents from 2 to p-1
        for (int i = 2; i < n; i++)
        {
            power *= g;
            power = power % n;
            if (rest[power] == true)
            {
                gfound = false;
                break;
            }
            rest[power] == true;
        }
    }
    cout << "g: " << g << endl;
    // 5. Find x and X for person A
    long long x = rand() % 9000 + 1000;
    long long Xsent = fastexp(g, x, n);
    // 6. Find y and Y for person B
    long long y = rand() % 9000 + 1000;
    long long Ysent = fastexp(g, y, n);
    // 7. Print publicly sent numbers
    cout << "A sends: " << Xsent << endl;
    cout << "B sends: " << Ysent << endl;
    // 8. Both A and B calculate their key k
    long long kA = fastexp(Ysent, x, n);
    cout << "A knows, that the key is " << kA << endl;
    long long kB = fastexp(Xsent, y, n);
    cout << "B knows, that the key is " << kB << endl;
}
