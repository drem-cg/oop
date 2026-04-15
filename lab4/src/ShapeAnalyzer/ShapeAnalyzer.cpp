#include "ShapeAnalyzer.h"
#include "Logger/Logger.h"

#include <algorithm>

std::shared_ptr<IShape> ShapeAnalyzer::FindMaxArea(const std::vector<std::shared_ptr<IShape>>& shapes)
{
	if (shapes.empty())
	{
		LogWarn("No shapes provided for max area search");
		return nullptr;
	}

	const auto maxIt = std::max_element(shapes.begin(), shapes.end(),
		[](const std::shared_ptr<IShape>& a, const std::shared_ptr<IShape>& b) {
			return a->GetArea() < b->GetArea();
		});

	return *maxIt;
}

std::shared_ptr<IShape> ShapeAnalyzer::FindMinPerimeter(const std::vector<std::shared_ptr<IShape>>& shapes)
{
	if (shapes.empty())
	{
		LogWarn("No shapes provided for min perimeter search");
		return nullptr;
	}

	const auto minIt = std::min_element(shapes.begin(), shapes.end(),
		[](const std::shared_ptr<IShape>& a, const std::shared_ptr<IShape>& b) {
			return a->GetPerimeter() < b->GetPerimeter();
		});

	return *minIt;
}