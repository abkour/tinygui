#pragma once
#include <ostream>

namespace tinygui {

struct Point2 {

	Point2();
	Point2(float px, float py);

	float x, y;
};

std::ostream& operator<<(std::ostream& os, const Point2& point);

}