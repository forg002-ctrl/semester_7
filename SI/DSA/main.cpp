#include <iostream>
#include <random>
#include <cmath>
#include <vector>

// DSA parameters (should be generated securely)
// const uint64_t p = 982451653;  // A prime number
// const uint64_t q = 982449707;  // A prime number (q | (p-1))
// const uint64_t g = 456789123;  // A generator

// A function to check if a number is prime
bool isPrime(uint64_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }

    return true;
}

// Custom modular exponentiation function
uint64_t modular_pow(uint64_t base, uint64_t exponent, uint64_t modulus) {
    uint64_t result = 1;
    base = base % modulus;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

// Generate prime numbers p and q
std::pair<uint64_t, uint64_t> generatePrimes() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(1e12, 1e13);  // Adjust the range

    while (true) {
        uint64_t p_candidate = dist(gen);
        if ((p_candidate & 1) && p_candidate % 10 == 5) {
            uint64_t q_candidate = (p_candidate - 1) / 2;

            std::cout<<p_candidate<<std::endl;
            std::cout<<q_candidate<<std::endl;
            std::cout<<std::endl;

            if (isPrime(p_candidate) && isPrime(q_candidate)) {
                return std::make_pair(p_candidate, q_candidate);
            }
        }
    }
}

// Generate generator g
uint64_t generateGenerator(uint64_t p, uint64_t q) {
    for (uint64_t g = 2; g < p; ++g) {
        uint64_t mod_result = modular_pow(g, q, p);
        if (mod_result != 1) {
            return g;
        }
    }
    return 0;
}

// User-specific key generation
struct KeyPair {
    uint64_t privateKey;  // x
    uint64_t publicKey;   // y
};

KeyPair generateUserKeyPair(uint64_t p, uint64_t q, uint64_t g) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(1, q - 1);

    KeyPair keyPair;
    keyPair.privateKey = dist(gen);
    keyPair.publicKey = modular_pow(g, keyPair.privateKey, p);

    return keyPair;
}

// Signature generation
struct Signature {
    uint64_t r;
    uint64_t s;
};

Signature generateSignature(const uint64_t privateKey, const std::string& message, uint64_t p, uint64_t q, uint64_t g) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> k_dist(1, q - 1);

    uint64_t k;
    Signature signature;

    do {
        k = k_dist(gen);
        signature.r = modular_pow(g, k, p) % q;
    } while (signature.r == 0);

    uint64_t h = std::hash<std::string>{}(message);
    uint64_t inverse_k = modular_pow(k, q - 2, q);  // Corrected to use q
    signature.s = (inverse_k * (h + privateKey * signature.r)) % q;

    return signature;
}

// Signature verification
bool verifySignature(const uint64_t publicKey, const std::string& message, const Signature& signature, uint64_t p, uint64_t q, uint64_t g) {
    if (signature.r < 1 || signature.r >= q || signature.s < 1 || signature.s >= q) {
        return false;
    }

    uint64_t h = std::hash<std::string>{}(message);
    uint64_t w = modular_pow(signature.s, q - 2, q);
    uint64_t u1 = (h * w) % q;
    uint64_t u2 = (signature.r * w) % q;
    uint64_t v = ((modular_pow(g, u1, p) * modular_pow(publicKey, u2, p)) % p) % q;

    return v == signature.r;
}

int main() {
    // std::pair<uint64_t, uint64_t> primes = generatePrimes();
    uint64_t p = 23;
    uint64_t q = 11;

    uint64_t g = generateGenerator(p, q);

    KeyPair userKeyPair = generateUserKeyPair(p, q, g);
    std::string message = "Hello, DSA!";
    Signature signature = generateSignature(userKeyPair.privateKey, message, p, q, g);

    bool isVerified = verifySignature(userKeyPair.publicKey, message, signature, p, q, g);

    if (isVerified) {
        std::cout << "Signature is valid." << std::endl;
    } else {
        std::cout << "Signature is not valid." << std::endl;
    }

    return 0;
}