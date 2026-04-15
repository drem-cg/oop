#pragma once

struct CPoint
{
	double x;
	double y;

	bool operator==(const CPoint& other) const
	{
		return x == other.x && y == other.y;
	}
};