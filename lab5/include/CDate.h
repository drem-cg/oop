#pragma once

#include <climits>
#include <iostream>

enum class Month
{
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
};

enum class WeekDay
{
	Sunday = 0,
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday
};

class CDate
{
public:
	CDate() = default;
	CDate(unsigned day, Month month, unsigned year);
	explicit CDate(unsigned timestamp);

	[[nodiscard]] unsigned GetDay() const;
	[[nodiscard]] Month GetMonth() const;
	[[nodiscard]] unsigned GetYear() const;
	[[nodiscard]] WeekDay GetWeekDay() const;
	[[nodiscard]] bool IsValid() const;

	CDate& operator++();
	CDate operator++(int);
	CDate& operator--();
	CDate operator--(int);

	CDate& operator+=(int days);
	CDate& operator-=(int days);

	friend CDate operator+(CDate date, int days);
	friend CDate operator+(int days, CDate date);
	friend CDate operator-(CDate date, int days);
	friend int operator-(const CDate& lhs, const CDate& rhs);

	friend bool operator==(const CDate& lhs, const CDate& rhs);
	friend bool operator!=(const CDate& lhs, const CDate& rhs);
	friend bool operator<(const CDate& lhs, const CDate& rhs);
	friend bool operator>(const CDate& lhs, const CDate& rhs);
	friend bool operator<=(const CDate& lhs, const CDate& rhs);
	friend bool operator>=(const CDate& lhs, const CDate& rhs);

	friend std::ostream& operator<<(std::ostream& out, const CDate& date);
	friend std::istream& operator>>(std::istream& iss, CDate& date);

private:
	static constexpr unsigned INVALID_TIMESTAMP = UINT_MAX;
	static constexpr unsigned MAX_TIMESTAMP = 2932896;

	unsigned m_timestamp = 0;
};