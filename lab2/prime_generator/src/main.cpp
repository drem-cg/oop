#include "prime_generator.h"
#include <iostream>
#include <string>

int main(const int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "ERROR" << std::endl;
		return 0;
	}

	int upperBound = 0;
	const std::string arg = argv[1];

	if (!ParseArgument(arg, upperBound))
	{
		std::cout << "ERROR 1" << std::endl;
		return 0;
	}

	const std::set<int> primes = GeneratePrimeNumbersSet(upperBound);

	bool isFirst = true;
	for (const int prime : primes)
	{
		if (!isFirst)
		{
			std::cout << " ";
		}
		std::cout << prime;
		isFirst = false;
	}
	std::cout << std::endl;

	return 0;
}
