#include "point2.hpp"

namespace tinygui {

struct Rectangle {

	Rectangle();
	Rectangle(const Point2 pmin, const Point2 pmax);

	Point2 min, max;
};


}