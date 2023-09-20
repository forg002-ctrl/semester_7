#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "libs/rsa.h"

PublicKey getPublicKey();
std::string getMessage();
void writeEncryptedMessageIntoFile();

int main(void){
    writeEncryptedMessageIntoFile();
}

PublicKey getPublicKey(){
    t_ullong N;
    t_ullong E;

    std::ifstream publicKeyFile;
    publicKeyFile.open(PATH_TO_FOLDER + "keys/publicKey.txt");
    if (publicKeyFile.fail()) {
        throw std::runtime_error("File with public key failed to open");
    }
    publicKeyFile>>N;
    publicKeyFile>>E;
    publicKeyFile.close();

    if (!N || !E) {
        throw std::runtime_error("Public Key wasn't found");
    }

    PublicKey publicKey;
    publicKey.N = N;
    publicKey.E = E;

    return publicKey;
}

std::string getMessage(){
    std::string messageToEncrypt;
    std::cout<<"Enter the message, you want to encrypt: ";
    std::getline(std::cin, messageToEncrypt);

    return messageToEncrypt;
}

void writeEncryptedMessageIntoFile(){
    std::vector<t_ullong> encryptedMessage = encryptMessage(getPublicKey(), getMessage());

    std::ofstream encryptedMessageFile;
    encryptedMessageFile.open(PATH_TO_FOLDER + "message/encryptedMessage.txt");
    if (encryptedMessageFile.fail()) {
        throw std::runtime_error("File for writing the encrypted message failed to open");
    }

    for (const auto &e : encryptedMessage) encryptedMessageFile << e << " ";
    encryptedMessageFile.close();    
}
