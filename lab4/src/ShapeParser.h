#pragma once

#include "IShape.h"
#include <istream>
#include <memory>
#include <vector>

class ShapeParser
{
public:
	static std::vector<std::shared_ptr<IShape>> Parse(std::istream& input);
};