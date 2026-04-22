#include "ShapeParser.h"

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

// TODO: про static

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

uint32_t ParseOutlineColor(std::istringstream& iss)
{
	std::string outlineHex;
	if (!(iss >> outlineHex))
	{
		throw std::invalid_argument("Missing outline color");
	}
	return ParseHexColor(outlineHex);
}

std::shared_ptr<IShape> ParseLineSegment(std::istringstream& iss)
{
	double x1, y1, x2, y2;
	if (!(iss >> x1 >> y1 >> x2 >> y2))
	{
		throw std::invalid_argument("Missing line parameters");
	}
	const uint32_t color = ParseOutlineColor(iss);
	return std::make_shared<CLineSegment>(CPoint{ x1, y1 }, CPoint{ x2, y2 }, color);
}

std::shared_ptr<IShape> ParseRectangleShape(std::istringstream& iss)
{
	double x1, y1, x2, y2;
	if (!(iss >> x1 >> y1 >> x2 >> y2))
	{
		throw std::invalid_argument("Missing rectangle parameters");
	}
	const auto [outline, fill] = ParseSolidColors(iss);
	return std::make_shared<CRectangle>(CPoint{ x1, y1 }, CPoint{ x2, y2 }, outline, fill);
}

std::shared_ptr<IShape> ParseCircleShape(std::istringstream& iss)
{
	double cx, cy, radius;
	if (!(iss >> cx >> cy >> radius))
	{
		throw std::invalid_argument("Missing circle parameters");
	}
	const auto [outline, fill] = ParseSolidColors(iss);
	return std::make_shared<CCircle>(CPoint{ cx, cy }, radius, outline, fill);
}

std::shared_ptr<IShape> ParseTriangleShape(std::istringstream& iss)
{
	double x1, y1, x2, y2, x3, y3;
	if (!(iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3))
	{
		throw std::invalid_argument("Missing triangle parameters");
	}
	const auto [outline, fill] = ParseSolidColors(iss);
	return std::make_shared<CTriangle>(CPoint{ x1, y1 }, CPoint{ x2, y2 }, CPoint{ x3, y3 }, outline, fill);
}

std::shared_ptr<IShape> CreateShape(const std::string& type, std::istringstream& iss)
{
	if (type == "line")
		return ParseLineSegment(iss);
	if (type == "rectangle")
		return ParseRectangleShape(iss);
	if (type == "circle")
		return ParseCircleShape(iss);
	if (type == "triangle")
		return ParseTriangleShape(iss);
	return nullptr;
}

bool IsEmptyLine(const std::string& line)
{
	return line.empty();
}

bool ExtractShapeType(std::istringstream& iss, std::string& outType)
{
	return (iss >> outType).get();
}

} // namespace

std::vector<std::shared_ptr<IShape>> ShapeParser::Parse(std::istream& input)
{
	std::vector<std::shared_ptr<IShape>> shapes;
	std::string line;
	int lineNumber = 0;

	while (std::getline(input, line))
	{
		lineNumber++;

		if (IsEmptyLine(line))
		{
			continue;
		}

		std::istringstream lineStream(line);
		std::string shapeType;

		if (!ExtractShapeType(lineStream, shapeType))
		{
			continue;
		}

		try
		{
			auto shape = CreateShape(shapeType, lineStream);
			if (!shape)
			{
				LogWarn("Unknown shape type at line " + std::to_string(lineNumber));
				continue;
			}
			shapes.emplace_back(shape);
		}
		catch (const std::exception& e)
		{
			LogError("Failed to parse line " + std::to_string(lineNumber) + ": " + e.what());
		}
	}

	return shapes;
}