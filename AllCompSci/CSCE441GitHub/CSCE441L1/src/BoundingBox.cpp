#include "BoundingBox.h"

BoundingBox::BoundingBox(int xbig, int xsmall, int ybig, int ysmall) {
	xmin = xsmall;
	xmax = xbig;
	ymin = ysmall;
	ymax = ybig;
}