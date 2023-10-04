#include "libs/des.h"

#include <iostream>
#include <bitset>

std::string getMessageToEncrypt();

int main(void) {
    std::string key64b = "DESkey56";
    std::string message = getMessageToEncrypt();

    std::string encrypted = encryptMessage(message, key64b);
    std::cout << "encrypted - " << encrypted << std::endl;
    std::cout << std::endl;
    std::string decrypted = decryptMessage(encrypted, key64b);
    std::cout << "decrypted - " << decrypted << std::endl;
    std::cout << std::endl;
}

std::string getMessageToEncrypt() {
    std::string message = "";
    std::getline(std::cin, message);

    size_t messageLength = message.length() % 8 == 0 ? message.length() : message.length() + (8 - (message.length() % 8));
    std::cout << convert_string_into_bits(message);
    std::cout << std::endl;
    std::cout << std::endl;

    return message;
}
