#ifndef RSA_UTILITIES_H
#define RSA_UTILITIES_H

#include <iostream>
#include <vector>

typedef unsigned long long int t_ullong;

static const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* ================================= Signatures ================================= */

bool isPrime(t_ullong n);
t_ullong getPrime(t_ullong N);
t_ullong gcd(t_ullong a, t_ullong b);
t_ullong mod(t_ullong a, t_ullong b, t_ullong c);
std::vector<t_ullong> convertMessageIntoASCI(std::string message);
std::vector<t_ullong> convertASCIIntoSegments(std::string message);
std::string convertSegmentsIntoMessage(std::vector<t_ullong> message);

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

std::vector<t_ullong> convertMessageIntoASCI(std::string message){
    std::vector<t_ullong> ASCIMessage;
    std::string two_numbers = "";
    
    for(int i = 0; i < message.length(); ++i){
        if(i%2 == 0 && i != 0){
            ASCIMessage.push_back(std::stoll(two_numbers));
            two_numbers = "";
        }
        std::string newNumber = std::to_string((int)message[i] - 31);
        if(newNumber.length() == 1 && i%2 != 0){
            newNumber.insert(newNumber.begin(), '0');
        }
        two_numbers += newNumber;
    }

    if(message.length()%2 != 0){
        two_numbers += "00";
    }
    ASCIMessage.push_back(std::stoll(two_numbers));

    return ASCIMessage;
}

std::vector<t_ullong> convertASCIIntoSegments(std::string message){
    std::vector<t_ullong> segmentedMessage;
    std::string segment = "";

    for(int i = 0; i < message.length(); ++i){
        if(message[i] != ' '){
            segment += message[i];
        }
        else{
            segmentedMessage.push_back(std::stoll(segment));
            segment = "";
        }
    }

    return segmentedMessage;
}

std::string convertSegmentsIntoMessage(std::vector<std::string> message){
    std::string decryptedMessage;
    for(int i = 0; i < message.size(); ++i){
        if(message[i].length() == 4){
            std::string asciCode = "";
            asciCode += message[i][0];
            asciCode += message[i][1];
            decryptedMessage += (char)(std::stod(asciCode) + 31);
            asciCode = "";
            asciCode += message[i][2];
            asciCode += message[i][3];
            decryptedMessage += (char)(std::stod(asciCode) + 31);
        }
        else{
            std::string asciCode = "";
            asciCode += message[i][0];
            decryptedMessage += (char)(std::stod(asciCode) + 31);
            asciCode = "";
            asciCode += message[i][1];
            asciCode += message[i][2];
            decryptedMessage += (char)(std::stod(asciCode) + 31);
        }
    }

    return decryptedMessage;
}

#endif