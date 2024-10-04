#include <cstddef>
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>

bool isDate(const char *date) {
    int countTrattini = 0;
    for ( size_t x = 0; x < std::strlen(date); ++x){
        if ( date[x] == '-' ){
            ++countTrattini;
        }
        else{
            if (date[x] < '0' && date[x] > '9'){
                return false;
            }
        }
    }
    if (countTrattini == 2){
        return true;
    }
    return false;
}

std::vector<std::string> splitString(const std::string& input) {
    std::vector<std::string> tokens;
    tokens.clear();
    std::string token;
    for (char c : input) {
        if (c != '/') {
            token += c;
        } else {
            tokens.push_back(token);
            token.clear();
        }
    }
    tokens.push_back(token); // Aggiungi l'ultimo token

    return tokens;
}

std::vector<std::string> splitStringJson(const std::string& input){
    std::vector<std::string> tokens;
    std::string token;
    for (char c : input) {
        if (c == '}'){
            tokens.push_back(token); // Aggiungi l'ultimo token
            return tokens;
        }
        else if (c != ':') {
            token += c;
        } else {
            tokens.push_back(token);
            token.clear();
        }
    }
    return tokens;
}

std::string replaseString(const std::string& json){
    std::string appoggio = "";
    bool flag = false;
    for (char carattere : json) {
        int c = static_cast<int>(carattere);
        if ( c == '}'){
            appoggio += "}";
            return appoggio;
        }
        if ( c == 34){
            flag = !flag;
        }
        if ( c < 0){
            break;
        }
        if (c == 44){
            appoggio += ":";
        }
        if (( c == 32 && flag ) || (c == '}') || (c >= 45 && c <= 58) || (c >= 60 && c <= 122) || (c >= 130 && c <= 163) || (c == 183) || (c == 181) || (c == 212) || (c == 224) || (c == 226)) {
            appoggio += carattere;
        }
    }
    return appoggio;
}

// Funzione di utilità per convertire un intero in una stringa
std::string intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string doubleToString(double value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string boolToString(bool value) {
    return value ? "true" : "false";
}