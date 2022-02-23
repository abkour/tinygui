#include "../impl/rect.hpp"

namespace tinygui {

Rectangle::Rectangle()
	: min(Point2(0, 0))
	, max(Point2(0, 0))
{}

Rectangle::Rectangle(const Point2 pmin, const Point2 pmax)
	: min(pmin)
	, max(pmax)
{}

}