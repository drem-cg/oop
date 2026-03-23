#include "prime_generator.h"
#include <cctype>
#include <cmath>
#include <sstream>
#include <vector>

std::set<int> GeneratePrimeNumbersSet(const int upperBound)
{
	std::set<int> primes;

	if (upperBound < 2)
	{
		return {};
	}

	std::vector<bool> isPrime(upperBound + 1, true);

	isPrime[0] = false;
	isPrime[1] = false;
	isPrime[2] = true;

	primes.insert(2);

	// TODO: проходить по нечетным для оптимизации перебора, это сделает более оптимальным
	// поправил. жаль, что нет учебного гитлаба с ютреком для таких комментов :))
	for (int i = 3; i * i <= upperBound; i += 2)
	{
		if (isPrime[i])
		{
			for (int j = i * i; j <= upperBound; j += 2 * i)
			{
				isPrime[j] = false;
			}
		}
	}

	for (int i = 3; i <= upperBound; i += 2)
	{
		if (isPrime[i])
		{
			primes.insert(i);
		}
	}

	return primes;
}

bool ParseArgument(const std::string& arg, int& result)
{
	if (arg.empty())
	{
		return false;
	}

	std::istringstream stream(arg);
	int value = 0;
	stream >> value;
	// TODO: узнать где упадет если > int size
	if (stream.fail() || value < 0 || value > maxBound)
	{
		return false;
	}

	result = value;
	return true;
}
