#pragma once

#include <set>
#include <string>

constexpr int maxBound = 100000000;

std::set<int> GeneratePrimeNumbersSet(int upperBound);

bool ParseArgument(const std::string& arg, int& result);