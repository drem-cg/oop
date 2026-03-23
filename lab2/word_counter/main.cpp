#include "word_counter.cpp"
#include <iostream>
#include <string>

int main()
{
	std::map<std::string, int> wordCounts;
	std::string word;

	while (std::cin >> word)
	{
		AddWordToMap(wordCounts, word);
	}

	PrintWordMap(wordCounts, std::cout);

	return 0;
}
