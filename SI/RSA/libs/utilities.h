#ifndef RSA_UTILITIES_H
#define RSA_UTILITIES_H

#include <iostream>
#include <vector>
#include <openssl/sha.h>

typedef unsigned long long int t_ullong;

static const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* ================================= Signatures ================================= */

bool isPrime(t_ullong n);
t_ullong getPrime(t_ullong N);
t_ullong gcd(t_ullong a, t_ullong b);
t_ullong mod(t_ullong a, t_ullong b, t_ullong c);
std::string sha256(const std::string& data);

/* ================================= Implementation ================================= */

bool isPrime(t_ullong n){
    if (n <= 1)  return false;
    if (n <= 3)  return true;
    if (n%2 == 0 || n%3 == 0) return false;
   
    for(int i = 5; i*i <= n; i = i+6){
        if(n%i == 0 || n%(i+2) == 0){
           return false;
        }
    }
    return true;
}

t_ullong getPrime(t_ullong N){
    if(N <= 1){
        return 2;
    }

    t_ullong prime = N;
    bool found = false;
 
    while(!found){
        prime++;
        if(isPrime(prime)){
            found = true;
        }
    }
 
    return prime;
}

t_ullong gcd(t_ullong a, t_ullong b){
    if(b == 0){
        return a;
    }
    return gcd(b, a % b); 
}

t_ullong mod(t_ullong a, t_ullong b, t_ullong c){
	if (b == 0){
		return 1;
	}
	else if (b % 2 == 0){
		t_ullong d = mod(a, b / 2, c);
		return (d * d) % c;
	}
	else{
		return ((a % c) * mod(a, b - 1, c)) % c;
	}
}

std::string sha256(const std::string& data){
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.c_str(), data.size());
    SHA256_Final(hash, &sha256);

    std::string hashStr;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hashStr += hash[i];
    }

    return hashStr;
}

#endif