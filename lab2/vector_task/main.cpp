#include <iostream>
#include <ostream>

#include "vector.h"

int main()
{
	std::vector<double> numbers;
	if (!ReadNumbers(numbers))
	{
		std::cout << "ERROR" << std::endl;
		return 0;
	}

	ProcessNumbers(numbers);
	PrintSortedNumbers(numbers);

	return 0;
}
