#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "libs/rsa.h"

PrivateKey getPrivateKey();
std::string getEncryptedMessage();
void logDecryptedMessage();

int main(void){
    logDecryptedMessage();
}

PrivateKey getPrivateKey(){
    t_ullong D;
    t_ullong N;

    std::ifstream privateKeyFile;
    privateKeyFile.open(PATH_TO_FOLDER + "keys/privateKey.txt");
    if (privateKeyFile.fail()) {
        throw std::runtime_error("File with private key failed to open");
    }
    privateKeyFile>>D;
    privateKeyFile>>N;
    privateKeyFile.close();

    if (!D || !N) {
        throw std::runtime_error("Private Key wasn't found");
    }

    PrivateKey privateKey;
    privateKey.D = D;
    privateKey.N = N;

    return privateKey;
}

std::string getEncryptedMessage(){
    std::string encryptedMessage;

    std::ifstream encryptedMessageFile;
    encryptedMessageFile.open(PATH_TO_FOLDER + "message/encryptedMessage.txt");
    if (encryptedMessageFile.fail()) {
        throw std::runtime_error("Encrypted Message failed to open");
    }

    std::getline(encryptedMessageFile, encryptedMessage);
    encryptedMessageFile.close();

    return encryptedMessage;
}

void logDecryptedMessage(){
    std::cout << decryptMessage(getPrivateKey(), getEncryptedMessage()) << std::endl;
}