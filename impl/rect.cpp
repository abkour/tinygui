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

void Rectangle::makeQuad(float* quad) {
	// Triangle 1
	quad[0] = min.x; quad[1] = min.y;
	quad[2] = max.x; quad[3] = min.y;
	quad[4] = max.x; quad[5] = max.y;
	// Triangle 2
	quad[6] = min.x; quad[7] = min.y;
	quad[8] = max.x; quad[9] = max.y;
	quad[10] = min.x; quad[11] = max.y;
}


}