#ifndef DES_UTILITIES_H
#define DES_UTILITIES_H

#include <iostream>
#include <bitset>
#include <vector>

const std::bitset<56> divider56(0b00000000000000000000000000001111111111111111111111111111);
const std::bitset<64> divider64(0b0000000000000000000000000000000011111111111111111111111111111111);

// FUNCTIONS DICLARATION
void split_48bits_to_6bits_blocks(std::bitset <48> block48b, std::bitset <6> * blocks6b);
std::bitset <32> join_4bits_to_32bits(std::bitset<4> * blocks4b);
std::bitset <64> convert_key_into_bits(std::string key);
std::bitset <64> convert_string_into_bits(std::string stringToConvert);
template <size_t N1, size_t N2 > std::bitset <N1 + N2> concatBitsets(const std::bitset <N1> &b1, const std::bitset <N2> &b2 );
static inline void swap(std::bitset<32> *L, std::bitset<32> *R);

// FUNCTIONS DEFINITION
void split_48bits_to_6bits_blocks(std::bitset <48> block48b, std::bitset <6> * blocks6b) {
    int current_array = 0;
    int i = 0;
    while (i < 48) {
        std::bitset <6> tmp6b;
        for (int j = 0; j < 6; ++j) {
            tmp6b[j] = block48b[i++];
        }
        blocks6b[current_array++] = tmp6b;
    }
}

std::bitset <32> join_4bits_to_32bits(std::bitset<4> * blocks4b) {
    std::bitset <32> block32b;
    int z = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 4; ++j) {
            block32b[z++] = blocks4b[i][j];
        }
    }

    return block32b;
}

std::bitset <64> convert_key_into_bits(std::string key) {
    if (key.length() != 8) {
        throw std::runtime_error("Key must be 8 symbols");
    }
    return convert_string_into_bits(key);
}

std::bitset <64> convert_string_into_bits(std::string stringToConvert) {
    std::string binary = "";
    for (char const &c: stringToConvert) {
        binary += std::bitset<8>(c).to_string();
    }
    std::bitset <64> returnBitset(binary);

    return returnBitset;
}

template <size_t N1, size_t N2 > std::bitset <N1 + N2> concatBitsets(const std::bitset <N1> &b1, const std::bitset <N2> &b2 ) {
    std::string s1 = b1.to_string();
    std::string s2 = b2.to_string();
    return std::bitset <N1 + N2>( s1 + s2 );
}

static inline void swap(std::bitset<32> *L, std::bitset<32> *R) {
    std::bitset<32> temp = *L;
    *L = *R;
    *R = temp;
}

#endif