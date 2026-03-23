#include "vector.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

bool ReadNumbers(std::vector<double>& numbers)
{
	std::string line;
	while (std::getline(std::cin, line))
	{
		std::istringstream iss(line);
		double number;
		while (iss >> number)
		{
			numbers.push_back(number);
		}

		if (!iss.eof())
		{
			return false;
		}
	}
	return true;
}

void ProcessNumbers(std::vector<double>& numbers)
{
	if (numbers.empty())
	{
		return;
	}
	// сделал при помощи std::accumulate
	const double positiveSum = std::accumulate(numbers.begin(), numbers.end(), 0.0,
		[](const double sum, const double num) {
			return num > 0 ? sum + num : sum;
		});

	const auto positiveCount = std::count_if(numbers.begin(), numbers.end(),
		[](const double num) { return num > 0; });

	double average = 0.0;
	if (positiveCount > 0)
	{
		average = positiveSum / static_cast<double>(positiveCount);
	}

	std::transform(numbers.begin(), numbers.end(), numbers.begin(),
		[average](const double num) {
			return num + average;
		});

	std::sort(numbers.begin(), numbers.end());
}

void PrintSortedNumbers(std::vector<double>& numbers)
{
	std::sort(numbers.begin(), numbers.end());
	for (size_t i = 0; i < numbers.size(); ++i)
	{
		std::cout << std::fixed << std::setprecision(3) << numbers[i];
		if (i < numbers.size() - 1)
		{
			std::cout << " ";
		}
	}
	std::cout << std::endl;
}
