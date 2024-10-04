#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#include <string>
#include <vector>


bool isDate(const char *date);

std::vector<std::string> splitString(const std::string& input);

std::vector<std::string> splitStringJson(const std::string& input);

std::string replaseString(const std::string& json);

std::string intToString(int value);

std::string doubleToString(double value);

std::string boolToString(bool value);

#endif
