#pragma once

#include <iostream>
#include <map>
#include <string>

void AddWordToMap(std::map<std::string, int>& wordCountMap, const std::string& word);

void PrintWordMap(const std::map<std::string, int>& wordCountMap, std::ostream& output);

std::string ToLowerCase(const std::string& text);
