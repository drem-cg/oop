#include "../src/prime_generator.h"
#include <gtest/gtest.h>
#include <vector>

bool CompareSetWithVector(const std::set<int>& primes, const std::vector<int>& expected)
{
	if (primes.size() != expected.size())
	{
		return false;
	}
	auto it = primes.begin();
	for (const int value : expected)
	{
		if (it == primes.end() || *it != value)
		{
			return false;
		}
		++it;
	}
	return true;
}

TEST(PrimeGeneratorTest, SmallNumbers)
{
	const std::set<int> result = GeneratePrimeNumbersSet(10);
	const std::vector<int> expected = { 2, 3, 5, 7 };
	EXPECT_TRUE(CompareSetWithVector(result, expected));
}

TEST(PrimeGeneratorTest, EdgeCase_ZeroAndOne)
{
	EXPECT_TRUE(GeneratePrimeNumbersSet(0).empty());
	EXPECT_TRUE(GeneratePrimeNumbersSet(1).empty());
}

TEST(PrimeGeneratorTest, EdgeCase_Two)
{
	const std::set<int> result = GeneratePrimeNumbersSet(2);
	const std::vector<int> expected = { 2 };
	EXPECT_TRUE(CompareSetWithVector(result, expected));
}

TEST(PrimeGeneratorTest, EdgeCase_Three)
{
	const std::set<int> result = GeneratePrimeNumbersSet(3);
	const std::vector<int> expected = { 2, 3 };
	EXPECT_TRUE(CompareSetWithVector(result, expected));
}

TEST(PrimeGeneratorTest, MediumRange)
{
	const std::set<int> result = GeneratePrimeNumbersSet(20);
	const std::vector<int> expected = { 2, 3, 5, 7, 11, 13, 17, 19 };
	EXPECT_TRUE(CompareSetWithVector(result, expected));
}

TEST(PrimeGeneratorTest, ParseArgument_Valid)
{
	int result = 0;
	EXPECT_TRUE(ParseArgument("100", result));
	EXPECT_EQ(result, 100);
}

TEST(PrimeGeneratorTest, ParseArgument_Invalid_Negative)
{
	int result = 0;
	EXPECT_FALSE(ParseArgument("-5", result));
}

TEST(PrimeGeneratorTest, ParseArgument_Invalid_Text)
{
	int result = 0;
	EXPECT_FALSE(ParseArgument("abc", result));
	EXPECT_FALSE(ParseArgument("12a", result));
}

TEST(PrimeGeneratorTest, ParseArgument_Invalid_Empty)
{
	int result = 0;
	EXPECT_FALSE(ParseArgument("", result));
}

TEST(PrimeGeneratorTest, ParseArgument_TooLarge)
{
	int result = 0;
	EXPECT_FALSE(ParseArgument("100000000000", result));
}

TEST(PrimeGeneratorTest, ParseArgument_MaxBound)
{
	int result = 0;
	EXPECT_TRUE(ParseArgument("100000000", result));
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}