#include "ShapeParser.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Logger.h"
#include "Rectangle.h"
#include "Triangle.h"

#include <sstream>
#include <stdexcept>
#include <string>

namespace
{

uint32_t ParseHexColor(const std::string color)
{
	std::string cleanedColor = color;
	if (!cleanedColor.empty() && cleanedColor[0] == '#')
	{
		cleanedColor.erase(0, 1);
	}

	if (cleanedColor.length() != 6)
	{
		throw std::invalid_argument("Invalid color lenght");
	}
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
			continue;

		try
		{
			if (type == "rectangle")
			{
				double x, y, w, h;
				std::string strokeHex, fillHex;
				if (!(iss >> x >> y >> w >> h >> strokeHex >> fillHex))
					throw std::invalid_argument("Missing rectangle parameters");

				shapes.emplace_back(std::make_shared<Rectangle>(
					x, y, w, h, ParseHexColor(strokeHex), ParseHexColor(fillHex)));
			}
			else if (type == "circle")
			{
				double x, y, r;
				std::string strokeHex, fillHex;
				if (!(iss >> x >> y >> r >> strokeHex >> fillHex))
					throw std::invalid_argument("Missing circle parameters");

				shapes.emplace_back(std::make_shared<Circle>(
					x, y, r, ParseHexColor(strokeHex), ParseHexColor(fillHex)));
			}
			else if (type == "triangle")
			{
				double x1, y1, x2, y2, x3, y3;
				std::string strokeHex, fillHex;
				if (!(iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> strokeHex >> fillHex))
					throw std::invalid_argument("Missing triangle parameters");

				shapes.emplace_back(std::make_shared<Triangle>(
					x1, y1, x2, y2, x3, y3, ParseHexColor(strokeHex), ParseHexColor(fillHex)));
			}
			else if (type == "ellipse")
			{
				double x, y, rx, ry;
				std::string strokeHex, fillHex;
				if (!(iss >> x >> y >> rx >> ry >> strokeHex >> fillHex))
					throw std::invalid_argument("Missing ellipse parameters");

				shapes.emplace_back(std::make_shared<Ellipse>(
					x, y, rx, ry, ParseHexColor(strokeHex), ParseHexColor(fillHex)));
			}
			else
			{
				LogWarn("Unknown shape type at line " + std::to_string(lineNum));
			}
		}
		catch (const std::exception& e)
		{
			LogError("Failed to parse line " + std::to_string(lineNum) + ": " + e.what());
		}
	}

	return shapes;
}