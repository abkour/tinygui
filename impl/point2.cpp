#include "point2.hpp"

namespace tinygui {

Point2::Point2() {
	x = y = 0.f;
}

Point2::Point2(float px, float py) 
	: x(px)
	, y(py)
{}

}