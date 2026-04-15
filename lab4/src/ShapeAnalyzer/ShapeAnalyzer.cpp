#include "ShapeAnalyzer.h"
#include "../Logger/Logger.h"

std::shared_ptr<IShape> ShapeAnalyzer::FindMaxArea(const std::vector<std::shared_ptr<IShape>>& shapes)
{
	if (shapes.empty())
	{
		LogWarn("No shapes provided for max area search");
		return nullptr;
	}

	auto maxIt = shapes.begin();
	for (auto it = shapes.begin() + 1; it != shapes.end(); ++it)
	{
		if ((*it)->GetArea() > (*maxIt)->GetArea())
		{
			maxIt = it;
		}
	}
	return *maxIt;
}

std::shared_ptr<IShape> ShapeAnalyzer::FindMinPerimeter(const std::vector<std::shared_ptr<IShape>>& shapes)
{
	if (shapes.empty())
	{
		LogWarn("No shapes provided for min perimeter search");
		return nullptr;
	}

	auto minIt = shapes.begin();
	for (auto it = shapes.begin() + 1; it != shapes.end(); ++it)
	{
		if ((*it)->GetPerimeter() < (*minIt)->GetPerimeter())
		{
			minIt = it;
		}
	}
	return *minIt;
}