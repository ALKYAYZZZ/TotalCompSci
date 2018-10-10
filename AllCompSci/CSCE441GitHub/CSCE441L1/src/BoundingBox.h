#include <string>
#include <vector>

class BoundingBox {
public:
	BoundingBox(int xmax, int xmin, int ymax, int ymin);
	int xmax;
	int ymax;
	int xmin;
	int ymin;
};
