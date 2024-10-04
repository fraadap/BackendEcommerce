#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include "database.h"

char* Database::computeHash(const char* input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input, strlen(input));
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    std::string hashStr = ss.str();
    char* hashChar = (char*)malloc(hashStr.size() + 1);
    strcpy(hashChar, hashStr.c_str());

    return hashChar;
}
