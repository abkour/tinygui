#include "point2.hpp"

namespace tinygui {

Point2::Point2() {
	x = y = 0.f;
}

Point2::Point2(float px, float py) 
	: x(px)
	, y(py)
{}

std::ostream& operator<<(std::ostream& os, const Point2& point) {
	return os << point.x << ", " << point.y;
}

}