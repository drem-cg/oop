#pragma once
#include "IShape.h"

#include <memory>
#include <vector>

class ShapeAnalyzer
{
public:
	static std::shared_ptr<IShape> FindMaxArea(const std::vector<std::shared_ptr<IShape>>& shapes);
	static std::shared_ptr<IShape> FindMinPerimeter(const std::vector<std::shared_ptr<IShape>>& shapes);
};