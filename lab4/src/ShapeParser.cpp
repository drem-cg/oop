#include "ShapeParser.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Logger.h"
#include "Rectangle.h"
#include "Triangle.h"

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

std::pair<uint32_t, uint32_t> ParseColors(std::istringstream& iss)
{
	std::string strokeHex, fillHex;
	if (!(iss >> strokeHex >> fillHex))
	{
		throw std::invalid_argument("Missing color parameters");
	}
	return { ParseHexColor(strokeHex), ParseHexColor(fillHex) };
}

std::shared_ptr<IShape> ParseRectangle(std::istringstream& iss)
{
	double x, y, w, h;
	if (!(iss >> x >> y >> w >> h))
	{
		throw std::invalid_argument("Missing rectangle parameters");
	}
	auto [stroke, fill] = ParseColors(iss);
	return std::make_shared<Rectangle>(x, y, w, h, stroke, fill);
}

std::shared_ptr<IShape> ParseCircle(std::istringstream& iss)
{
	double x, y, r;
	if (!(iss >> x >> y >> r))
	{
		throw std::invalid_argument("Missing circle parameters");
	}
	auto [stroke, fill] = ParseColors(iss);
	return std::make_shared<Circle>(x, y, r, stroke, fill);
}

std::shared_ptr<IShape> ParseTriangle(std::istringstream& iss)
{
	double x1, y1, x2, y2, x3, y3;
	if (!(iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3))
	{
		throw std::invalid_argument("Missing triangle parameters");
	}
	auto [stroke, fill] = ParseColors(iss);
	return std::make_shared<Triangle>(x1, y1, x2, y2, x3, y3, stroke, fill);
}

std::shared_ptr<IShape> ParseEllipse(std::istringstream& iss)
{
	double x, y, rx, ry;
	if (!(iss >> x >> y >> rx >> ry))
	{
		throw std::invalid_argument("Missing ellipse parameters");
	}
	auto [stroke, fill] = ParseColors(iss);
	return std::make_shared<Ellipse>(x, y, rx, ry, stroke, fill);
}

std::shared_ptr<IShape> ParseShape(const std::string& type, std::istringstream& iss)
{
	if (type == "rectangle")
		return ParseRectangle(iss);
	if (type == "circle")
		return ParseCircle(iss);
	if (type == "triangle")
		return ParseTriangle(iss);
	if (type == "ellipse")
		return ParseEllipse(iss);
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
		{
			continue;
		}

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