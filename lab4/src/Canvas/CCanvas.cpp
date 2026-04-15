#include "CCanvas.h"
#include <stdexcept>

CCanvas::CCanvas(double width, double height, const std::string& title)
    : m_window(sf::VideoMode(static_cast<unsigned int>(width), static_cast<unsigned int>(height)), title)
{
    m_window.setVerticalSyncEnabled(false);
}

sf::Color CCanvas::ToSFMLColor(const uint32_t color)
{
	const uint8_t r = (color >> 16) & 0xFF;
	const uint8_t g = (color >> 8) & 0xFF;
	const uint8_t b = color & 0xFF;
	return {r, g, b};
}

void CCanvas::DrawLine(const CPoint from, const CPoint to, const uint32_t lineColor)
{
	const sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(static_cast<float>(from.x), static_cast<float>(from.y)), ToSFMLColor(lineColor)),
        sf::Vertex(sf::Vector2f(static_cast<float>(to.x), static_cast<float>(to.y)), ToSFMLColor(lineColor))
    };
    m_window.draw(line, 2, sf::Lines);
}

void CCanvas::FillPolygon(const std::vector<CPoint>& points, const uint32_t fillColor)
{
    if (points.empty()) return;

    sf::ConvexShape polygon(points.size());
    for (size_t i = 0; i < points.size(); ++i)
    {
        polygon.setPoint(i, sf::Vector2f(static_cast<float>(points[i].x), static_cast<float>(points[i].y)));
    }
    polygon.setFillColor(ToSFMLColor(fillColor));
    m_window.draw(polygon);
}

void CCanvas::DrawCircle(const CPoint center, const double radius, const uint32_t lineColor)
{
    sf::CircleShape circle(static_cast<float>(radius));
    circle.setPosition(static_cast<float>(center.x) - static_cast<float>(radius),
                       static_cast<float>(center.y) - static_cast<float>(radius));

    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(ToSFMLColor(lineColor));
    circle.setOutlineThickness(1.0f);

    m_window.draw(circle);
}

void CCanvas::FillCircle(const CPoint center, const double radius, const uint32_t fillColor)
{
    sf::CircleShape circle(static_cast<float>(radius));
    circle.setPosition(static_cast<float>(center.x) - static_cast<float>(radius),
                       static_cast<float>(center.y) - static_cast<float>(radius));

    circle.setFillColor(ToSFMLColor(fillColor));

    m_window.draw(circle);
}

void CCanvas::BeginFrame()
{
    m_window.clear(sf::Color::Black);
}

void CCanvas::Display()
{
    m_window.display();

    bool running = true;
    while (running)
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                running = false;
        }
        sf::sleep(sf::milliseconds(100));
    }
}