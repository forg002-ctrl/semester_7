#include <iostream>
#include <ctime>
#include <fstream>

#include "libs/rsa.h"

void writeKeysIntoFile();

int main(void){
    writeKeysIntoFile();
}

void writeKeysIntoFile(){
    std::ofstream publicKeyFile;
    publicKeyFile.open(PATH_TO_FOLDER + "keys/publicKey.txt");
    if (publicKeyFile.fail()) {
        throw std::runtime_error("File for writing the public key failed to open");
    }

    std::ofstream privateKeyFile;
    privateKeyFile.open(PATH_TO_FOLDER + "keys/privateKey.txt");
    if (privateKeyFile.fail()) {
        throw std::runtime_error("File for writing the private key failed to open");
    }

    RSAKeys keys = generateKeys();

    publicKeyFile<<keys.publicKey.N<<std::endl;
    publicKeyFile<<keys.publicKey.E;
    publicKeyFile.close();

    privateKeyFile<<keys.privateKey.D<<std::endl;
    privateKeyFile<<keys.privateKey.N;
    privateKeyFile.close();
}
