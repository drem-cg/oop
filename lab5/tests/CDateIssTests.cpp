#include "CDate.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(CDateInputTest, ValidDateFormat)
{
	CDate date;
	std::istringstream input("15.03.2024");
	input >> date;
	EXPECT_TRUE(date.IsValid());
	EXPECT_EQ(date.GetDay(), 15u);
	EXPECT_EQ(date.GetMonth(), Month::March);
	EXPECT_EQ(date.GetYear(), 2024u);
}

TEST(CDateInputTest, EpochStart)
{
	CDate date;
	std::istringstream input("01.01.1970");
	input >> date;
	EXPECT_TRUE(date.IsValid());
	EXPECT_EQ(date.GetYear(), 1970u);
}

TEST(CDateInputTest, LeapYearValid)
{
	CDate date;
	std::istringstream input("29.02.2000");
	input >> date;
	EXPECT_TRUE(date.IsValid());
}

TEST(CDateInputTest, LeapYearInvalid)
{
	CDate date;
	std::istringstream input("29.02.1999");
	input >> date;
	EXPECT_FALSE(date.IsValid());
	EXPECT_TRUE(input.fail());
}

TEST(CDateInputTest, UpperBoundary)
{
	CDate date;
	std::istringstream input("31.12.9999");
	input >> date;
	EXPECT_TRUE(date.IsValid());
	EXPECT_EQ(date.GetYear(), 9999u);
}

TEST(CDateInputTest, InvalidKeyword)
{
	CDate date(1, Month::January, 2020);
	std::istringstream input("INVALID");
	input >> date;
	EXPECT_FALSE(date.IsValid());
	EXPECT_FALSE(input.fail());
}

TEST(CDateInputTest, WrongSeparator)
{
	CDate date;
	std::istringstream input("15-03-2024");
	input >> date;
	EXPECT_TRUE(input.fail());
}

TEST(CDateInputTest, NonNumericInput)
{
	CDate date;
	std::istringstream input("abc.def.ghi");
	input >> date;
	EXPECT_TRUE(input.fail());
}

TEST(CDateInputTest, ShortYearFormat)
{
	CDate date;
	std::istringstream input("01.01.24");
	input >> date;
	EXPECT_TRUE(input.fail());
}

TEST(CDateInputTest, OutOfRangeDay)
{
	CDate date;
	std::istringstream input("32.01.2024");
	input >> date;
	EXPECT_FALSE(date.IsValid());
	EXPECT_TRUE(input.fail());
}

TEST(CDateInputTest, OutOfRangeYearBelow)
{
	CDate date;
	std::istringstream input("01.01.1969");
	input >> date;
	EXPECT_FALSE(date.IsValid());
	EXPECT_TRUE(input.fail());
}

TEST(CDateInputTest, LeadingAndTrailingSpaces)
{
	CDate date;
	std::istringstream input("  10.05.2025  ");
	input >> date;
	EXPECT_TRUE(date.IsValid());
	EXPECT_EQ(date.GetDay(), 10u);
}

TEST(CDateInputTest, StreamStateAfterFailure)
{
	CDate date;
	std::istringstream input("wrong.format.date");
	input >> date;
	EXPECT_TRUE(input.fail());

	int dummy = 0;
	input >> dummy;
	EXPECT_TRUE(input.fail());
}