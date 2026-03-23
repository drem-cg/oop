#include "word_counter.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(WordCounterTest, CaseInsensitiveCounting)
{
	std::map<std::string, int> counts;
	AddWordToMap(counts, "Hello");
	AddWordToMap(counts, "hello");
	AddWordToMap(counts, "HELLO");

	EXPECT_EQ(counts["hello"], 3);
}

TEST(WordCounterTest, MultipleWordsSorted)
{
	std::map<std::string, int> counts;
	AddWordToMap(counts, "apple");
	AddWordToMap(counts, "banana");
	AddWordToMap(counts, "apple");

	std::ostringstream output;
	PrintWordMap(counts, output);

	const std::string expected = "apple: 2\nbanana: 1\n";
	EXPECT_EQ(output.str(), expected);
}

TEST(WordCounterTest, EmptyInput)
{
	std::map<std::string, int> counts;
	std::ostringstream output;
	PrintWordMap(counts, output);
	EXPECT_TRUE(output.str().empty());
}

TEST(WordCounterTest, SingleWord)
{
	std::map<std::string, int> counts;
	AddWordToMap(counts, "test");
	std::ostringstream output;
	PrintWordMap(counts, output);
	EXPECT_EQ(output.str(), "test: 1\n");
}

// TODO: русский язык тесты

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}