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

int gcd(int a, int b) // Greatest Common Divisor
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

// This method doesn't require mod to be a prime number
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
    long long p = 0; // assert that the while loop runs at least 1 time
    long long q = 0;
    long long n = 0;
    long long phi;
    int size = Primes.size();
    cout << "Possible prime set size: " << size << endl;
    // 3. Find p, q, n and phi
    while (p == q)
    {
        int indexP = rand() % size;
        p = Primes[indexP];
        int indexQ = rand() % size;
        q = Primes[indexQ];
    }
    cout << "p: " << p << endl;
    cout << "q: " << q << endl;
    n = p * q;
    cout << "n: " << n << endl;
    phi = (p - 1) * (q - 1);
    cout << "phi: " << phi << endl;
    // 4. Choose e
    // gcd(e, phi) = 1
    long long e = phi;
    while (gcd(e, phi) != 1)
    {
        e = rand() % 9000 + 1000;
    }
    cout << "e: " << e << endl;
    // 5. Calculate d
    // d * e is congruent to 1 mod phi
    // Therefore d is congruent (or even better: equal) to the inverse modulo of e^(-1)
    // We can safely use the Extended Euclidean Algorithm because gcd(e, phi) = 1
    // Proof by Bezout identity:
    // If a and p are coprime (i.e., their greatest common divisor is 1), then there exist integers
    // x and y such that ax + py = 1
    cout << "Safety check of gcd(e, phi): " << gcd(e, phi) << endl;
    long long d = InverseModuloEuklides(e, phi);
    cout << "d: " << d << endl;
    // 6. Give information about public and private key
    cout << "Public key:" << endl;
    cout << "e: " << e << endl;
    cout << "n: " << n << endl;
    cout << "Private key:" << endl;
    cout << "d: " << d << endl;
    cout << "n: " << n << endl;
    // 7. Create a message
    string message = "If you can read this, then you must have the correct private key!";
    vector<int> messageINT;
    for (int i = 0; i < message.size(); i++)
    {
        messageINT.push_back((int)message[i]);
    }
    cout << "Message: " << message << endl;
    // 8. Encrypt the message
    vector<int> encryptedINT;
    string encryptedSTRING = "";
    for (int i = 0; i < messageINT.size(); i++)
    {
        int encrypted = fastexp(messageINT[i], e, n);
        encryptedINT.push_back(encrypted);
        encryptedSTRING += (char)encrypted;
    }
    cout << "Encrypted message: " << encryptedSTRING << endl;
    // 8. Decrypt the message
    vector<int> decryptedINT;
    string decryptedSTRING = "";
    for (int i = 0; i < encryptedINT.size(); i++)
    {
        int decrypted = fastexp(encryptedINT[i], d, n);
        decryptedINT.push_back(decrypted);
        decryptedSTRING += (char)decrypted;
    }
    cout << "Decrypted message: " << decryptedSTRING << endl;
    cout << endl;
    cout << "Summary:" << endl;
    cout << "1. Generating secure keys, securely managing key pairs, and implementing defenses against side-channel attacks can be challenging aspects of RSA implementation." << endl;
    cout << "2. Knowing only the public key, it is very difficult to recover the private key without factoring n." << endl;
    cout << "The resilience of RSA to attacks relies on the difficulty of generating the private key based on the public key without knowledge of the prime factors of n." << endl;
}
