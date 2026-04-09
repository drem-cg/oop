#include "Logger.h"
#include "ShapeAnalyzer.h"
#include "ShapeParser.h"

#include <iomanip>
#include <iostream>

void PrintShapeInfo(const std::string& title, const std::shared_ptr<IShape>& shape)
{
	std::cout << "\n" << title << std::endl;
	if (!shape)
	{
		std::cout << "No shape found." << std::endl;
		return;
	}

	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Area: " << shape->GetArea() << std::endl;
	std::cout << "Perimeter: " << shape->GetPerimeter() << std::endl;
	std::cout << "Stroke Color: #" << std::hex << std::setfill('0') << std::setw(6) << shape->GetStrokeColor() << std::dec << std::endl;
	std::cout << "Fill Color:   #" << std::hex << std::setfill('0') << std::setw(6) << shape->GetFillColor() << std::dec << std::endl;
	std::cout << "Details: " << shape->ToString() << std::endl;
}

int main()
{
	LoggerInit(LogType::Info);
	LogInfo("Application started. Reading shapes from stdin...");

	const std::vector<std::shared_ptr<IShape>> shapes = ShapeParser::Parse(std::cin);

	LogInfo("Successfully parsed " + std::to_string(shapes.size()) + " shapes.");

	const auto maxAreaShape = ShapeAnalyzer::FindMaxArea(shapes);
	const auto minPerimeterShape = ShapeAnalyzer::FindMinPerimeter(shapes);

	PrintShapeInfo("Max Area Shape", maxAreaShape);
	PrintShapeInfo("Min Perimeter Shape", minPerimeterShape);
	return 0;
}