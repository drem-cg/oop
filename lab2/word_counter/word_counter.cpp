#include "word_counter.h"
#include <algorithm>
#include <cctype>

void PrintWordMap(const std::map<std::string, int>& wordCountMap, std::ostream& output)
{
	for (const auto& [name, count] : wordCountMap)
	{
		output << name << ": " << count << '\n';
	}
}

void AddWordToMap(std::map<std::string, int>& wordCountMap, const std::string& word)
{
	if (word.empty())
	{
		return;
	}
	const std::string normalized = ToLowerCase(word);
	wordCountMap[normalized]++;
}

std::string ToLowerCase(const std::string& text)
{
	std::string result = text;
	std::transform(result.begin(), result.end(), result.begin(),
		[](const unsigned char c) { return std::tolower(c); });
	return result;
}
