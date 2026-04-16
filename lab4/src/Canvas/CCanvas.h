#pragma once
#include "ICanvas.h"
#include <SFML/Graphics.hpp>
#include <string>

class CCanvas : public ICanvas
{
public:
	CCanvas(double width, double height, const std::string& title);

	void DrawLine(CPoint from, CPoint to, uint32_t lineColor) override;
	void FillPolygon(const std::vector<CPoint>& points, uint32_t fillColor) override;
	void DrawCircle(CPoint center, double radius, uint32_t lineColor) override;
	void FillCircle(CPoint center, double radius, uint32_t fillColor) override;
	void BeginFrame();
	void Display();

private:
	sf::RenderWindow m_window;
	static sf::Color ToSFMLColor(uint32_t color);
};