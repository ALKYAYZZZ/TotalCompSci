#include <string>
#include <vector>
#include <iostream>
#include <vector>
#include <memory>
#include "Image.h"
#include "Triangle.h"

using namespace std;

class BoundingBox {
public:
	BoundingBox(float xmax, float xmin, float ymax, float ymin, float zmax, float zmin, Triangle tri);
	void createBox(shared_ptr<Image> &image);
	void resize(BoundingBox total_bbox, int width, int height);
	float xmax;
	float ymax;
	float xmin;
	float ymin;
	float zmax;
	float zmin;
	Triangle tri;
	void createTriangle(shared_ptr<Image> &image);
	void createZColor(shared_ptr<Image> &image, vector<vector<float>> &zBuf, BoundingBox total_bbox);
	void createYColor(shared_ptr<Image> &image, vector<vector<float>> &zBuf, BoundingBox total_bbox, float new_ymax, float new_ymin);
};
