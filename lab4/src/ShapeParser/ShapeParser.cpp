#include "../ShapeParser/ShapeParser.h"
#include "../Circle/CCircle.h"
#include "../LineSegment/CLineSegment.h"
#include "../Logger/Logger.h"
#include "../Rectangle/CRectangle.h"
#include "../Triangle/CTriangle.h"

#include <charconv>
#include <istream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
uint32_t ParseHexColor(const std::string& hex)
{
	if (hex.length() != 6)
	{
		throw std::invalid_argument("Invalid hex color length");
	}

	uint32_t color = 0;
	auto [ptr, ec] = std::from_chars(hex.data(), hex.data() + 6, color, 16);

	if (ec != std::errc{} || ptr != hex.data() + 6)
	{
		throw std::invalid_argument("Invalid hex color format");
	}
	return color;
}

std::pair<uint32_t, uint32_t> ParseSolidColors(std::istringstream& iss)
{
	std::string outlineHex, fillHex;
	if (!(iss >> outlineHex >> fillHex))
	{
		throw std::invalid_argument("Missing solid shape colors");
	}
	return { ParseHexColor(outlineHex), ParseHexColor(fillHex) };
}

uint32_t ParseOutlineOnly(std::istringstream& iss)
{
	std::string outlineHex;
	if (!(iss >> outlineHex))
	{
		throw std::invalid_argument("Missing outline color");
	}
	return ParseHexColor(outlineHex);
}

std::shared_ptr<IShape> ParseLine(std::istringstream& iss)
{
	double x1, y1, x2, y2;
	if (!(iss >> x1 >> y1 >> x2 >> y2))
	{
		throw std::invalid_argument("Missing line parameters");
	}
	return std::make_shared<CLineSegment>(CPoint{ x1, y1 }, CPoint{ x2, y2 }, ParseOutlineOnly(iss));
}

std::shared_ptr<IShape> ParseRectangle(std::istringstream& iss)
{
	double x1, y1, x2, y2;
	if (!(iss >> x1 >> y1 >> x2 >> y2))
	{
		throw std::invalid_argument("Missing rectangle parameters");
	}
	auto [outline, fill] = ParseSolidColors(iss);
	return std::make_shared<CRectangle>(CPoint{ x1, y1 }, CPoint{ x2, y2 }, outline, fill);
}

std::shared_ptr<IShape> ParseCircle(std::istringstream& iss)
{
	double cx, cy, r;
	if (!(iss >> cx >> cy >> r))
	{
		throw std::invalid_argument("Missing circle parameters");
	}
	auto [outline, fill] = ParseSolidColors(iss);
	return std::make_shared<CCircle>(CPoint{ cx, cy }, r, outline, fill);
}

std::shared_ptr<IShape> ParseTriangle(std::istringstream& iss)
{
	double x1, y1, x2, y2, x3, y3;
	if (!(iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3))
	{
		throw std::invalid_argument("Missing triangle parameters");
	}
	auto [outline, fill] = ParseSolidColors(iss);
	return std::make_shared<CTriangle>(CPoint{ x1, y1 }, CPoint{ x2, y2 }, CPoint{ x3, y3 }, outline, fill);
}

std::shared_ptr<IShape> ParseShape(const std::string& type, std::istringstream& iss)
{
	if (type == "line")
		return ParseLine(iss);
	if (type == "rectangle")
		return ParseRectangle(iss);
	if (type == "circle")
		return ParseCircle(iss);
	if (type == "triangle")
		return ParseTriangle(iss);
	return nullptr;
}
} // namespace

std::vector<std::shared_ptr<IShape>> ShapeParser::Parse(std::istream& input)
{
	std::vector<std::shared_ptr<IShape>> shapes;
	std::string line;
	int lineNum = 0;

	while (std::getline(input, line))
	{
		lineNum++;
		if (line.empty() || line[0] == '#')
			continue;

		std::istringstream iss(line);
		std::string type;
		if (!(iss >> type))
		{
			continue;
		}

		try
		{
			auto shape = ParseShape(type, iss);
			if (!shape)
			{
				LogWarn("Unknown shape type at line " + std::to_string(lineNum));
				continue;
			}
			shapes.emplace_back(shape);
		}
		catch (const std::exception& e)
		{
			LogError("Failed to parse line " + std::to_string(lineNum) + ": " + e.what());
		}
	}
	return shapes;
}