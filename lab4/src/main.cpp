#include "Canvas/CCanvas.h"
#include "Canvas/ICanvasDrawable.h"
#include "ISolidShape.h"
#include "Logger/Logger.h"
#include "ShapeAnalyzer/ShapeAnalyzer.h"
#include "ShapeParser/ShapeParser.h"

#include <iomanip>
#include <iostream>

void PrintShapeInfo(const std::string& title, const std::shared_ptr<IShape>& shape)
{
	std::cout << "\n--- " << title << " ---" << std::endl;
	if (!shape)
	{
		std::cout << "No shape found." << std::endl;
		return;
	}

	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Area: " << shape->GetArea() << std::endl;
	std::cout << "Perimeter: " << shape->GetPerimeter() << std::endl;
	std::cout << "Outline Color: #" << std::hex << std::setfill('0') << std::setw(6)
			  << shape->GetOutlineColor() << std::dec << std::endl;

	if (auto* solid = dynamic_cast<const ISolidShape*>(shape.get()))
	{
		std::cout << "Fill Color:    #" << std::hex << std::setfill('0') << std::setw(6)
				  << solid->GetFillColor() << std::dec << std::endl;
	}
	else
	{
		std::cout << "Fill Color:    N/A (non-solid shape)" << std::endl;
	}

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

	if (!shapes.empty())
	{
		LogInfo("Creating visualization window...");

		CCanvas canvas(800, 600, "Shapes Visualization");

		canvas.BeginFrame();

		for (const auto& shape : shapes)
		{
			if (const auto* drawable = dynamic_cast<ICanvasDrawable*>(shape.get()))
			{
				drawable->Draw(canvas);
			}
		}

		LogInfo("Rendering complete. Close the window to exit.");

		canvas.Display();
	}

	std::cout << std::flush;
	return 0;
}