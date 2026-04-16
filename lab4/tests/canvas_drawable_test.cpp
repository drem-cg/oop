#include "Canvas/ICanvas.h"
#include "Circle/CCircle.h"
#include "Point/CPoint.h"
#include "Rectangle/CRectangle.h"
#include "Triangle/CTriangle.h"

#include <fakeit.hpp>
#include <gtest/gtest.h>
#include <vector>

using namespace fakeit;

TEST(CCircleDrawTest, CallsFillCircleAndDrawCircle)
{
	Mock<ICanvas> mockCanvas;
	When(Method(mockCanvas, FillCircle)).AlwaysReturn();
	When(Method(mockCanvas, DrawCircle)).AlwaysReturn();

	const CCircle circle(CPoint{ 10.0, 20.0 }, 5.0, 0xFF0000, 0x00FF00);
	circle.Draw(mockCanvas.get());

	Verify(Method(mockCanvas, FillCircle).Using(CPoint{ 10.0, 20.0 }, 5.0, 0x00FF00)).Once();
	Verify(Method(mockCanvas, DrawCircle).Using(CPoint{ 10.0, 20.0 }, 5.0, 0xFF0000)).Once();
	VerifyNoOtherInvocations(mockCanvas);
}

TEST(CRectangleDrawTest, CallsFillPolygonAndDrawLines)
{
	Mock<ICanvas> mockCanvas;
	When(Method(mockCanvas, FillPolygon)).AlwaysReturn();
	When(Method(mockCanvas, DrawLine)).AlwaysReturn();

	const CRectangle rect(CPoint{ 0.0, 0.0 }, CPoint{ 10.0, 5.0 }, 0x000000, 0xFFFFFF);
	rect.Draw(mockCanvas.get());

	Verify(Method(mockCanvas, FillPolygon)).Once();

	Verify(Method(mockCanvas, DrawLine)
			   .Using(CPoint{ 0.0, 0.0 }, CPoint{ 10.0, 0.0 }, 0x000000))
		.Once();
	Verify(Method(mockCanvas, DrawLine)
			   .Using(CPoint{ 10.0, 0.0 }, CPoint{ 10.0, 5.0 }, 0x000000))
		.Once();
	Verify(Method(mockCanvas, DrawLine)
			   .Using(CPoint{ 10.0, 5.0 }, CPoint{ 0.0, 5.0 }, 0x000000))
		.Once();
	Verify(Method(mockCanvas, DrawLine)
			   .Using(CPoint{ 0.0, 5.0 }, CPoint{ 0.0, 0.0 }, 0x000000))
		.Once();

	VerifyNoOtherInvocations(mockCanvas);
}

TEST(CTriangleDrawTest, CallsFillPolygonAndThreeLines)
{
	Mock<ICanvas> mockCanvas;
	When(Method(mockCanvas, FillPolygon)).AlwaysReturn();
	When(Method(mockCanvas, DrawLine)).AlwaysReturn();

	const CTriangle triangle(CPoint{ 0.0, 0.0 }, CPoint{ 3.0, 0.0 }, CPoint{ 0.0, 4.0 }, 0x111111, 0x222222);
	triangle.Draw(mockCanvas.get());

	Verify(Method(mockCanvas, FillPolygon)).Once();

	Verify(Method(mockCanvas, DrawLine).Using(CPoint{ 0.0, 0.0 }, CPoint{ 3.0, 0.0 }, 0x111111)).Once();
	Verify(Method(mockCanvas, DrawLine).Using(CPoint{ 3.0, 0.0 }, CPoint{ 0.0, 4.0 }, 0x111111)).Once();
	Verify(Method(mockCanvas, DrawLine).Using(CPoint{ 0.0, 4.0 }, CPoint{ 0.0, 0.0 }, 0x111111)).Once();

	VerifyNoOtherInvocations(mockCanvas);
}