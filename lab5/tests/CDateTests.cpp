#include "CDate.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(CDateTest, DefaultConstructorIsEpoch)
{
	constexpr CDate date;
	EXPECT_TRUE(date.IsValid());
	EXPECT_EQ(date.GetDay(), 1u);
	EXPECT_EQ(date.GetMonth(), Month::January);
	EXPECT_EQ(date.GetYear(), 1970u);
}

TEST(CDateTest, ValidDateConstruction)
{
	const CDate date(15, Month::March, 2024);
	EXPECT_TRUE(date.IsValid());
	EXPECT_EQ(date.GetDay(), 15u);
	EXPECT_EQ(date.GetMonth(), Month::March);
	EXPECT_EQ(date.GetYear(), 2024u);
}

TEST(CDateTest, InvalidDayThrows)
{
	EXPECT_THROW(CDate(32, Month::January, 2024), std::invalid_argument);
}

TEST(CDateTest, InvalidMonthThrows)
{
	EXPECT_THROW(CDate(1, Month(13), 2024), std::invalid_argument);
}

TEST(CDateTest, YearBelow1970Throws)
{
	EXPECT_THROW(CDate(1, Month::January, 1969), std::invalid_argument);
}

TEST(CDateTest, TimestampConstructorOutOfRange)
{
	const CDate date(3000000u);
	EXPECT_FALSE(date.IsValid());
}

TEST(CDateTest, LeapYear2000IsValid)
{
	const CDate date(29, Month::February, 2000);
	EXPECT_TRUE(date.IsValid());
}

TEST(CDateTest, NonLeapYear1900IsInvalid)
{
	EXPECT_THROW(CDate(29, Month::February, 1900), std::invalid_argument);
}

TEST(CDateTest, LeapYear2004IsValid)
{
	const CDate date(29, Month::February, 2004);
	EXPECT_TRUE(date.IsValid());
}

TEST(CDateTest, NonLeapYear2100IsInvalid)
{
	EXPECT_THROW(CDate(29, Month::February, 2100), std::invalid_argument);
}

TEST(CDateTest, MinBoundary1970)
{
	const CDate minDate(1, Month::January, 1970);
	EXPECT_TRUE(minDate.IsValid());
	EXPECT_EQ(minDate.GetDay(), 1u);
}

TEST(CDateTest, MaxBoundary9999)
{
	const CDate maxDate(31, Month::December, 9999);
	EXPECT_TRUE(maxDate.IsValid());
	EXPECT_EQ(maxDate.GetYear(), 9999u);
}

TEST(CDateTest, DayBeforeMaxIsValid)
{
	CDate date(30, Month::December, 9999);
	date += 1;
	EXPECT_TRUE(date.IsValid());
	EXPECT_EQ(date.GetDay(), 31u);
}

TEST(CDateTest, DayAfterMaxBecomesInvalid)
{
	CDate date(31, Month::December, 9999);
	date += 1;
	EXPECT_FALSE(date.IsValid());
}

TEST(CDateTest, DayBeforeMinBecomesInvalid)
{
	CDate date(1, Month::January, 1970);
	date -= 1;
	EXPECT_FALSE(date.IsValid());
}

TEST(CDateTest, AddDaysCrossesMonthBoundary)
{
	CDate date(28, Month::February, 2024);
	date += 1;
	EXPECT_EQ(date.GetDay(), 29u);
	EXPECT_EQ(date.GetMonth(), Month::February);
}

TEST(CDateTest, AddDaysCrossesYearBoundary)
{
	CDate date(31, Month::December, 2023);
	date += 1;
	EXPECT_EQ(date.GetDay(), 1u);
	EXPECT_EQ(date.GetMonth(), Month::January);
	EXPECT_EQ(date.GetYear(), 2024u);
}

TEST(CDateTest, SubtractDaysCrossesYearBoundary)
{
	CDate date(1, Month::January, 2024);
	date -= 1;
	EXPECT_EQ(date.GetDay(), 31u);
	EXPECT_EQ(date.GetMonth(), Month::December);
	EXPECT_EQ(date.GetYear(), 2023u);
}

TEST(CDateTest, PostfixAndPrefixIncrement)
{
	CDate d1(10, Month::January, 2024);
	const CDate d2 = d1++;
	EXPECT_EQ(d2.GetDay(), 10u);
	EXPECT_EQ(d1.GetDay(), 11u);

	CDate d3(12, Month::January, 2024);
	++d3;
	EXPECT_EQ(d3.GetDay(), 13u);
}

TEST(CDateTest, DateDifferenceOperator)
{
	const CDate early(1, Month::January, 2024);
	const CDate late(31, Month::December, 2024);
	EXPECT_EQ(late - early, 365);
	EXPECT_EQ(early - late, -365);
}

TEST(CDateTest, EqualityOperators)
{
	const CDate a(15, Month::March, 2024);
	const CDate b(15, Month::March, 2024);
	const CDate c(16, Month::March, 2024);
	EXPECT_TRUE(a == b);
	EXPECT_TRUE(a != c);
}

TEST(CDateTest, OrderingOperators)
{
	const CDate jan(1, Month::January, 2024);
	const CDate dec(31, Month::December, 2024);
	EXPECT_TRUE(jan < dec);
	EXPECT_TRUE(dec > jan);
	EXPECT_TRUE(jan <= dec);
	EXPECT_TRUE(dec >= jan);
}

TEST(CDateTest, EpochWeekdayIsThursday)
{
	const CDate epoch(1, Month::January, 1970);
	EXPECT_EQ(epoch.GetWeekDay(), WeekDay::Thursday);
}

TEST(CDateTest, KnownDateWeekday)
{
	const CDate date(24, Month::April, 2026);
	EXPECT_EQ(date.GetWeekDay(), WeekDay::Friday);
}

TEST(CDateTest, OutputStreamFormat)
{
	const CDate date(5, Month::July, 2023);
	std::ostringstream oss;
	oss << date;
	EXPECT_EQ(oss.str(), "05.07.2023");
}

TEST(CDateTest, OutputStreamInvalid)
{
	const CDate date(3000000u);
	std::ostringstream oss;
	oss << date;
	EXPECT_EQ(oss.str(), "INVALID");
}

TEST(CDateTest, InputStreamValid)
{
	CDate date;
	std::istringstream iss("15.03.2024");
	iss >> date;
	EXPECT_TRUE(date.IsValid());
	EXPECT_EQ(date.GetDay(), 15u);
	EXPECT_EQ(date.GetMonth(), Month::March);
}

TEST(CDateTest, InputStreamInvalidString)
{
	CDate date(1, Month::January, 2020);
	std::istringstream iss("INVALID");
	iss >> date;
	EXPECT_FALSE(date.IsValid());
}

TEST(CDateTest, InputStreamBadFormat)
{
	CDate date;
	std::istringstream iss("abc");
	iss >> date;
	EXPECT_TRUE(iss.fail());
}

TEST(CDateTest, InvalidDatePlusDaysStaysInvalid)
{
	CDate date(3000000u);
	date += 100;
	EXPECT_FALSE(date.IsValid());
}

TEST(CDateTest, InvalidDateMinusDaysStaysInvalid)
{
	CDate date(3000000u);
	date -= 50;
	EXPECT_FALSE(date.IsValid());
}

TEST(CDateTest, ComparisonWithInvalidReturnsFalse)
{
	const CDate valid(1, Month::January, 2024);
	const CDate invalid(3000000u);
	EXPECT_FALSE(valid < invalid);
	EXPECT_FALSE(valid > invalid);
	EXPECT_FALSE(valid == invalid);
}