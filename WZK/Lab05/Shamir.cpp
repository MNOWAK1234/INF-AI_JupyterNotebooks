#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

bool Prime[100007]; // Information if number is prime

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

long long InverseModuloEuklides(long long a, long long mod) // Finding Inverse Modulo
{
    long long InitialModulo = mod;
    // prepare first iteration
    long long t1 = 0;
    long long t2 = 1;
    long long q, r, t;
    while (a != 0)
    {
        q = mod / a;
        r = mod % a;
        t = t1 - (t2 * q);
        // calculate next
        mod = a;
        a = r;
        t1 = t2;
        t2 = t;
    }
    // assure that inverse modulo is positive
    t1 += InitialModulo;
    t1 = t1 % InitialModulo;
    return t1;
}

long long secret;
long long prime;
long long total_shares;
long long required_shares;

vector<long long> a;
vector<pair<long long, long long>> shares;
vector<long long> Langrange;

int main()
{
    srand(time(0));
    Sieve(100000);
    // 1. Specify the parameters
    cout << "Enter the number of shares: ";
    cin >> total_shares;
    cout << "Enter the required number of shares to get the secret: ";
    cin >> required_shares;
    cout << "Specify secret: ";
    cin >> secret;
    cout << "Specify a prime number: ";
    cin >> prime;
    while (true)
    {
        if (Prime[prime] == false)
            cout << "This number is not prime! Specify a different prime number: ";
        else if (prime <= secret || prime <= total_shares)
            cout << "The prime number must be greater than both the secret and the number of total shares. \n \
            Specify a different prime number: ";
        else
            break;
        cin >> prime;
    }
    // 2. Create the polynomial
    a.push_back(secret);
    for (int i = 1; i < required_shares; i++)
    {
        long long random = rand() % secret;
        a.push_back(random);
    }
    // cout << "Obejscie losowania" << endl;
    // cin >> a[1] >> a[2];
    cout << "Polynomial: ";
    cout << secret;
    for (int i = 1; i < a.size(); i++)
    {
        cout << " + " << a[i] << "x^" << i;
    }
    cout << endl;
    // 3. Calculate shares
    for (int i = 1; i <= total_shares; i++)
    {
        long long share = 0;
        long long x = 1;
        for (int j = 0; j < required_shares; j++)
        {
            share += (x * a[j]) % prime;
            x *= i;
            x = x % prime;
            share = share % prime;
        }
        shares.push_back(make_pair(i, share));
    }
    // 4. Print shares
    cout << "Shares:" << endl;
    for (int i = 0; i < shares.size(); i++)
    {
        cout << shares[i].first << " " << shares[i].second << endl;
    }
    // 5. Calculate sum of Langrange's Polynomials
    long long sum = 0;
    for (int i = 1; i <= required_shares; i++)
    {
        long long denominator = 1;
        long long numerator = 1;
        for (int j = 1; j <= required_shares; j++)
        {
            if (j == i)
                continue;
            denominator *= (shares[i].first - shares[j].first);
            denominator = denominator % prime;
            denominator += prime;
            denominator = denominator % prime;
            numerator *= (0 - shares[j].first);
            numerator = numerator % prime;
            numerator += prime;
            numerator = numerator % prime;
        }
        //  Calculate inverse modulo of denominator
        long long inverse_denominator = InverseModuloEuklides(denominator, prime);
        sum += (shares[i].second * numerator * inverse_denominator);
        sum += prime;
        sum = sum % prime;
        sum += prime;
        sum = sum % prime;
    }
    sum += prime;
    sum = sum % prime;
    cout << "Retrieved secret: " << sum << endl;
}