#ifndef DES_H
#define DES_H

#include "utilities.h"
#include "schemas.h"

// GLOBAL VARIABLES
std::string desStringKey = "DESkey64";

// FUNCTIONS DICLARATION
std::string encryptMessage(std::string message, std::string DES64bKey);

std::bitset <56> key64bToKey56b(std::string key64b);
std::vector<std::bitset <48>> keyExpansionTo48bits(std::bitset <28> L, std::bitset <28> R);
std::bitset <48> keyContractionPermutation(std::bitset <56> block56b);

std::bitset <64> initialPermutation(std::bitset <64> block64b);
std::bitset <64> finalPermutation(std::bitset <64> block64b);

void feistelCipherEncode(std::bitset <32> *L, std::bitset<32> *R, std::vector<std::bitset <48>> keys48b);
void feistelCipherDecode(std::bitset <32> *L, std::bitset<32> *R, std::vector<std::bitset <48>> keys48b);
void round_feistel_cipher(std::bitset <32> *L, std::bitset<32> *R, std::bitset <48> key48b);

std::bitset <32> funcF(std::bitset <32> block32b, std::bitset <48> key48b);
std::bitset <48> expansionPermutation(std::bitset <32> block32b);
std::bitset <32> substitutions(std::bitset <48> block48b);
void substitution6bitsTo4bits(std::bitset <6> * blocks6b, std::bitset <4> * blocks4b);
std::bitset <32> permutation(std::bitset <32> block32b);

// FUNCTIONS DEFINITION
std::string encryptMessage(std::string message, std::string DES64bKey) {
    // KEYS CREATION
    std::bitset <56> key56b = key64bToKey56b(DES64bKey);
    std::bitset <28> rightKey = std::bitset<28> ((key56b & divider56).to_ulong());
    std::bitset <28> leftKey = std::bitset<28> (((key56b >> 28) & divider56).to_ulong());


    std::vector<std::bitset <48>> keys48b = keyExpansionTo48bits(leftKey, rightKey);

    // ENCODE MESSAGE BY 64bit blocks
    std::string encryptedMessage = "";
    size_t messageLength = message.length() % 8 == 0 ? message.length() : message.length() + (8 - (message.length() % 8));
    for (size_t i = 0; i < messageLength; i += 8) {
        std::bitset <64> messagePart = convert_string_into_bits(message.substr(i, 8));
        messagePart = initialPermutation(messagePart);
        std::bitset <32> L = std::bitset<32> ((messagePart & divider64).to_ulong());
        std::bitset <32> R = std::bitset<32> (((messagePart >> 32) & divider64).to_ulong());
        feistelCipherEncode(&L, &R, keys48b);
        std::bitset <64> encryptPart64b = finalPermutation(concatBitsets(L, R));
        encryptedMessage += encryptPart64b.to_string<char,std::string::traits_type,std::string::allocator_type>();
    }

    return encryptedMessage;
}

std::string decryptMessage(std::string encryptedMessage, std::string DES64bKey) {
    // KEYS CREATION
    std::bitset <56> key56b = key64bToKey56b(DES64bKey);
    std::bitset <28> rightKey = std::bitset<28> ((key56b & divider56).to_ulong());
    std::bitset <28> leftKey = std::bitset<28> (((key56b >> 28) & divider56).to_ulong());
    std::vector<std::bitset <48>> keys48b = keyExpansionTo48bits(leftKey, rightKey);

    // ENCODE MESSAGE BY 64bit blocks
    std::string decryptedMessage = "";
    for (size_t i = 0; i < encryptedMessage.length(); i += 64) {
        std::bitset <64> messagePart = convert_string_into_bits(encryptedMessage.substr(i, 64));
        messagePart = initialPermutation(messagePart);
        std::bitset <32> L = std::bitset<32> ((messagePart & divider64).to_ulong());
        std::bitset <32> R = std::bitset<32> (((messagePart >> 32) & divider64).to_ulong());
        feistelCipherDecode(&L, &R, keys48b);
        std::bitset <64> decryptPart64b = finalPermutation(concatBitsets(L, R));
        decryptedMessage += decryptPart64b.to_string<char,std::string::traits_type,std::string::allocator_type>();
    }

    return decryptedMessage;
}

std::bitset <56> key64bToKey56b(std::string string64bKey) {
    std::bitset <64> key64b = convert_key_into_bits(string64bKey);
    std::bitset <56> key56b;

    for (int i = 0; i < 56; ++i) {
        key56b[i] = key64b[KeyPermutationSchema[i] - 1];
    }

    return key56b;
}

std::vector<std::bitset <48>> keyExpansionTo48bits(std::bitset <28> L, std::bitset <28> R) {
    std::vector<std::bitset <48>> keys48b;
    size_t n = 0;

    for (size_t i = 1; i <= 16; ++i) {
        switch(i) {
            case 1: case 2: case 9: case 16: n = 1; break;
            default: n = 2; break;
        }

        L <<= n;
        R <<= n;

        std::bitset <56> block56b = concatBitsets(L, R);
        keys48b.push_back(keyContractionPermutation(block56b));
    }

    return keys48b;
}

std::bitset <48> keyContractionPermutation(std::bitset <56> block56b) {
    std::bitset <48> block48b;
    for (int i = 0; i < 48; ++i) {
        block48b[i] = block56b[KeyContractionPermutationSchema[i] - 1];
    }

    return block48b;
}

std::bitset <64> initialPermutation(std::bitset <64> block64b) {
    std::bitset <64> returnBlock64b;
    for (int i = 0 ; i < 64; ++i) {
        returnBlock64b[i] = block64b[InitialPermutationSchema[i] - 1];
    }

    return returnBlock64b;  
}

std::bitset <64> finalPermutation(std::bitset <64> block64b) {
    std::bitset <64> returnBlock64b;
    for (int i = 0 ; i < 64; ++i) {
        returnBlock64b[i] = block64b[FinalPermutaionSchema[i] - 1];
    }

    return returnBlock64b;
}

void feistelCipherEncode(std::bitset <32> *L, std::bitset<32> *R, std::vector<std::bitset <48>> keys48b) {
    for (int round = 0; round < 16; ++round) {
        round_feistel_cipher(L, R, keys48b[round]);
    }
    swap(L, R);
}

void feistelCipherDecode(std::bitset <32> *L, std::bitset<32> *R, std::vector<std::bitset <48>> keys48b) {
    for (int round = 15; round >= 0; --round) {
        round_feistel_cipher(L, R, keys48b[round]);
    }
    swap(L, R);
}

void round_feistel_cipher(std::bitset <32> *L, std::bitset<32> *R, std::bitset <48> key48b) {
    std::bitset <32> temp = *R;
    *R = funcF(*R, key48b) ^ *L;
    *L = temp;
}

std::bitset <32> funcF(std::bitset <32> block32b, std::bitset <48> key48b) {
    std::bitset <48> block48b = expansionPermutation(block32b);
    block48b ^= key48b;

    return permutation(substitutions(block48b));
}

std::bitset <48> expansionPermutation(std::bitset <32> block32b) {
    std::bitset <48> block48b;
    for (int i = 0 ; i < 48; ++i) {
        block48b[i] = block32b[ExpansionPermutationSchema[i] - 1];
    }

    return block48b;
}

void substitution6bitsTo4bits(std::bitset <6> * blocks6b, std::bitset <4> * blocks4b) {
    for (int i = 0; i < 8; ++i) {
        std::bitset <2> block2b;
        std::bitset <4> block4b;

        // fist and last bits
        block2b[0] = blocks6b[i][0];
        block2b[1] = blocks6b[i][5];

        // remain bits
        block4b[0] = blocks6b[i][1];
        block4b[1] = blocks6b[i][2];
        block4b[2] = blocks6b[i][3];
        block4b[3] = blocks6b[i][4];

        blocks4b[i] = SboxesSchemas[i][block2b.to_ulong()][block4b.to_ulong()];
    }
}

std::bitset <32> substitutions(std::bitset <48> block48b) {
    std::bitset <6> blocks6b[8];
    std::bitset <4> blocks4b[8];

    split_48bits_to_6bits_blocks(block48b, blocks6b);
    substitution6bitsTo4bits(blocks6b, blocks4b);
    return join_4bits_to_32bits(blocks4b);
}

std::bitset <32> permutation(std::bitset <32> block32b) {
    std::bitset <32> returnBlock32b;
    for (int i = 0; i < 32; ++i) {
        returnBlock32b[i] = block32b[PermutationSchema[i] - 1];
    }

    return returnBlock32b;
}

#endif