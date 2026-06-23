#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>

std::string ToLower(std::string str);
std::vector<std::string> camelcase(const std::string & str);
std::vector<std::string> Tokenize(const std::string& str);
std::string input(std::string placeholder);
