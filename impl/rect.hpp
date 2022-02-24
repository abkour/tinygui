#include "point2.hpp"

namespace tinygui {

struct Rectangle {

	Rectangle();
	Rectangle(const Point2 pmin, const Point2 pmax);

	void makeQuad(float* quad);

	Point2 min, max;
	unsigned int id;
};


}