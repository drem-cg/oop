#include "CDate.h"
#include "Logger/Logger.h"

#include <charconv>
#include <iomanip>
#include <stdexcept>

namespace
{

constexpr unsigned MIN_YEAR = 1970;
constexpr unsigned MAX_YEAR = 9999;
constexpr int DAYS_PER_400_YEARS = 146097;
constexpr int UNIX_EPOCH_OFFSET = 719468;
constexpr int MONTH_CYCLE_FACTOR = 153;
constexpr int MONTH_CYCLE_DIVISOR = 5;
constexpr int MONTH_CYCLE_OFFSET = 2;

constexpr unsigned char DAYS_IN_MONTH[] = {
	31, 28, 31, 30, 31, 30,
	31, 31, 30, 31, 30, 31
};

struct DateParts
{
	unsigned day;
	Month month;
	unsigned year;
};

constexpr int DaysSinceEpochFromYMD(const int year, const unsigned month, const unsigned day) noexcept
{
	const int yearShifted = year - (month <= 2 ? 1 : 0);
	const int era = (yearShifted >= 0 ? yearShifted : yearShifted - 399) / 400;
	const auto yearOfEra = static_cast<unsigned>(yearShifted - era * 400);

	const unsigned monthIndex = (month > 2) ? (month - 3) : (month + 9);
	const unsigned dayOfYear = (MONTH_CYCLE_FACTOR * monthIndex + MONTH_CYCLE_OFFSET) / MONTH_CYCLE_DIVISOR + day - 1;
	const unsigned dayOfEra = yearOfEra * 365 + yearOfEra / 4 - yearOfEra / 100 + dayOfYear;

	return era * DAYS_PER_400_YEARS + static_cast<int>(dayOfEra) - UNIX_EPOCH_OFFSET;
}

constexpr DateParts YMDFromDaysSinceEpoch(const int daysSinceEpoch) noexcept
{
	const int adjustedDays = daysSinceEpoch + UNIX_EPOCH_OFFSET;

	const int era = (adjustedDays >= 0 ? adjustedDays : adjustedDays - DAYS_PER_400_YEARS + 1) / DAYS_PER_400_YEARS;
	const auto dayOfEra = static_cast<unsigned>(adjustedDays - era * DAYS_PER_400_YEARS);

	const unsigned yearOfEra = (dayOfEra - dayOfEra / 1460 + dayOfEra / 36524 - dayOfEra / 146096) / 365;
	const int year = static_cast<int>(yearOfEra) + era * 400;
	const unsigned dayOfYear = dayOfEra - (365 * yearOfEra + yearOfEra / 4 - yearOfEra / 100);

	constexpr int MONTH_NUMERATOR = 5;
	constexpr int MONTH_DENOMINATOR = 153;

	const unsigned monthIndex = (MONTH_NUMERATOR * dayOfYear + 2) / MONTH_DENOMINATOR;
	const unsigned day = dayOfYear - (MONTH_DENOMINATOR * monthIndex + 2) / MONTH_NUMERATOR + 1;
	const unsigned month = monthIndex < 10 ? monthIndex + 3 : monthIndex - 9;

	return {
		day,
		static_cast<Month>(month), static_cast<unsigned>(year + (month <= 2))
	};
}

constexpr bool IsLeapYear(const int year) noexcept
{
	return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

constexpr unsigned DaysInMonth(Month month, const int year) noexcept
{
	if (month == Month::February && IsLeapYear(year))
		return 29;
	return DAYS_IN_MONTH[static_cast<unsigned>(month) - 1];
}

bool IsValidDate(const unsigned day, Month month, const unsigned year) noexcept
{
	if (year < MIN_YEAR || year > MAX_YEAR)
		return false;
	const auto m = static_cast<unsigned>(month);
	return m >= 1 && m <= 12 && day >= 1 && day <= DaysInMonth(month, static_cast<int>(year));
}

DateParts TimestampToDateParts(const unsigned timestamp) noexcept
{
	return YMDFromDaysSinceEpoch(static_cast<int>(timestamp));
}

} // namespace

CDate::CDate(const unsigned day, Month month, const unsigned year)
{
	if (!IsValidDate(day, month, year))
	{
		LogError("invalid date components");
		throw std::invalid_argument("day/month/year out of valid range");
	}
	m_timestamp = static_cast<unsigned>(
		DaysSinceEpochFromYMD(static_cast<int>(year), static_cast<unsigned>(month), day));
}

CDate::CDate(const unsigned timestamp)
{
	if (timestamp > MAX_TIMESTAMP)
	{
		LogWarn("timestamp exceeds maximum allowed value");
		m_timestamp = INVALID_TIMESTAMP;
	}
	else
	{
		m_timestamp = timestamp;
	}
}

bool CDate::IsValid() const
{
	return m_timestamp != INVALID_TIMESTAMP;
}

unsigned CDate::GetDay() const
{
	if (IsValid())
	{
		return TimestampToDateParts(m_timestamp).day;
	}
	return 0;
}

Month CDate::GetMonth() const
{
	if (IsValid())
	{
		return TimestampToDateParts(m_timestamp).month;
	}
	return Month::January;
}

unsigned CDate::GetYear() const
{
	if (IsValid())
	{
		return TimestampToDateParts(m_timestamp).year;
	}
	return 0;
}

WeekDay CDate::GetWeekDay() const
{
	if (!IsValid())
	{
		return WeekDay::Sunday;
	}
	// + 4 т.к. четверг
	return static_cast<WeekDay>((static_cast<int>(m_timestamp) + 4) % 7);
}

CDate& CDate::operator++()
{
	return *this += 1;
}

CDate CDate::operator++(int)
{
	const CDate copy = *this;
	++(*this);
	return copy;
}

CDate& CDate::operator--()
{
	return *this -= 1;
}

CDate CDate::operator--(int)
{
	const CDate copy = *this;
	--(*this);
	return copy;
}

CDate& CDate::operator+=(const int days)
{
	if (!IsValid())
		return *this;

	const long long newTimestamp = static_cast<long long>(m_timestamp) + days;
	if (newTimestamp < 0 || newTimestamp > static_cast<long long>(MAX_TIMESTAMP))
	{
		LogWarn("operation out of valid date range");
		m_timestamp = INVALID_TIMESTAMP;
	}
	else
	{
		m_timestamp = static_cast<unsigned>(newTimestamp);
	}
	return *this;
}

CDate& CDate::operator-=(const int days)
{
	return *this += -days;
}

CDate operator+(CDate date, const int days)
{
	date += days;
	return date;
}

CDate operator+(const int days, const CDate date)
{
	return date + days;
}

CDate operator-(CDate date, const int days)
{
	date -= days;
	return date;
}

int operator-(const CDate& lhs, const CDate& rhs)
{
	if (!lhs.IsValid() || !rhs.IsValid())
	{
		return 0;
	}
	return static_cast<int>(lhs.m_timestamp) - static_cast<int>(rhs.m_timestamp);
}

bool operator==(const CDate& lhs, const CDate& rhs)
{
	return lhs.m_timestamp == rhs.m_timestamp;
}

bool operator!=(const CDate& lhs, const CDate& rhs)
{
	return !(lhs == rhs);
}

bool operator<(const CDate& lhs, const CDate& rhs)
{
	return lhs.IsValid() && rhs.IsValid() && lhs.m_timestamp < rhs.m_timestamp;
}

bool operator>(const CDate& lhs, const CDate& rhs)
{
	return lhs.IsValid() && rhs.IsValid() && lhs.m_timestamp > rhs.m_timestamp;
}

bool operator<=(const CDate& lhs, const CDate& rhs)
{
	return lhs.IsValid() && rhs.IsValid() && lhs.m_timestamp <= rhs.m_timestamp;
}

bool operator>=(const CDate& lhs, const CDate& rhs)
{
	return lhs.IsValid() && rhs.IsValid() && lhs.m_timestamp >= rhs.m_timestamp;
}

std::ostream& operator<<(std::ostream& out, const CDate& date)
{
	if (!date.IsValid())
	{
		out << "INVALID";
		return out;
	}

	const auto parts = TimestampToDateParts(date.m_timestamp);
	out << std::setfill('0')
		<< std::setw(2) << parts.day << '.'
		<< std::setw(2) << static_cast<unsigned>(parts.month) << '.'
		<< std::setw(4) << parts.year;
	return out;
}

std::istream& operator>>(std::istream& iss, CDate& date)
{
	std::string token;
	if (!(iss >> token))
	{
		return iss;
	}

	if (token == "INVALID")
	{
		date.m_timestamp = CDate::INVALID_TIMESTAMP;
		return iss;
	}

	unsigned day{}, month{}, year{};
	const char* ptr = token.data();
	const char* end = ptr + token.size();

	auto parseNumAndDot = [&ptr, end](unsigned& out) -> bool {
		const auto res = std::from_chars(ptr, end, out);
		if (res.ec != std::errc{} || res.ptr == ptr || res.ptr == end || *res.ptr != '.')
		{
			return false;
		}
		ptr = res.ptr + 1;
		return true;
	};

	if (!parseNumAndDot(day) || !parseNumAndDot(month))
	{
		LogWarn("invalid input format");
		iss.setstate(std::ios::failbit);
		return iss;
	}

	const auto res = std::from_chars(ptr, end, year);
	if (res.ec != std::errc{} || res.ptr != end)
	{
		LogWarn("invalid input format");
		iss.setstate(std::ios::failbit);
		return iss;
	}

	try
	{
		date = CDate(day, static_cast<Month>(month), year);
	}
	catch (const std::invalid_argument&)
	{
		LogWarn("date out of valid range");
		iss.setstate(std::ios::failbit);
		date.m_timestamp = CDate::INVALID_TIMESTAMP;
	}
	return iss;
}