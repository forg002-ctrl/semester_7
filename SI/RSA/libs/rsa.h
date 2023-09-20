#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <vector>

#include "utilities.h"

/* ================================= Constants ================================= */

#define RANGE_MIN 100
#define RANGE_MAX 1000
std::string PATH_TO_FOLDER = "/home/kosai/Desktop/7_semester/SI/RSA/";

/* ================================= Signatures ================================= */

struct PublicKey
{
    t_ullong N;
    t_ullong E;
};

struct PrivateKey
{
    t_ullong D;
    t_ullong N;
};

struct RSAKeys
{
    PublicKey publicKey;
    PrivateKey privateKey;

    RSAKeys(PublicKey publicKey, PrivateKey privateKey)
    {
        this->publicKey = publicKey;
        this->privateKey = privateKey;
    };
};

t_ullong calculate_E(t_ullong Phi_N);
t_ullong calculate_D(t_ullong Phi_N, t_ullong E);
RSAKeys generateKeys();
std::vector<t_ullong> encryptMessage(PublicKey publicKey, std::string message);
std::string decryptMessage(PrivateKey privateKey, std::string message);

/* ================================= Implementation ================================= */

t_ullong calculate_E(t_ullong Phi_N) {
    t_ullong i = 2;
    while(gcd(i,Phi_N) != 1){
        i++;
    }
    if(i >= Phi_N){
        exit(-1);
    }
    else{
        return i;
    }
}

t_ullong calculate_D(t_ullong Phi_N, t_ullong E) {
    t_ullong k = 0;
    while(((k*Phi_N)+1)%E != 0){
        k++;
    }
    return (((k*Phi_N)+1)/E);
}

RSAKeys generateKeys() {
    srand((int)time(0));
    t_ullong randNumber1 = RANGE_MIN + (rand() % (RANGE_MAX - RANGE_MIN + 1));
    t_ullong randNumber2 = RANGE_MIN + (rand() % (RANGE_MAX - RANGE_MIN + 1));

    t_ullong P = getPrime(randNumber1);
    t_ullong Q = getPrime(randNumber2);
    t_ullong N = P * Q;
    t_ullong Phi_N = (P - 1) * (Q - 1);
    t_ullong E = calculate_E(Phi_N);
    t_ullong D = calculate_D(Phi_N, E);

    PublicKey publicKey;
    publicKey.N = N;
    publicKey.E = E;

    PrivateKey privateKey;
    privateKey.D = D;
    privateKey.N = N;

    return RSAKeys(publicKey, privateKey);
}

std::vector<t_ullong> encryptMessage(PublicKey publicKey, std::string message){
    std::vector<t_ullong> encryptedMessage;
    std::vector<t_ullong> ASCIMessage = convertMessageIntoASCI(message);

    for(int i = 0; i < ASCIMessage.size(); ++i){
        encryptedMessage.push_back(mod(ASCIMessage[i], publicKey.E, publicKey.N));
    }

    return encryptedMessage;
}

std::string decryptMessage(PrivateKey privateKey, std::string message){
    std::vector<t_ullong> segmentedMessage = convertASCIIntoSegments(message);

    std::vector<std::string> decryptedSegmentedMessage;
    for(int i = 0; i < segmentedMessage.size(); ++i){
        decryptedSegmentedMessage.push_back(std::to_string(mod(segmentedMessage[i], privateKey.D, privateKey.N)));
    }

    return convertSegmentsIntoMessage(decryptedSegmentedMessage);
}

#endif