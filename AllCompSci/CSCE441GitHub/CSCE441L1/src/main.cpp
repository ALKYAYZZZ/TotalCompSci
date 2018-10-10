#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Image.h"
#include "BoundingBox.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

int find_highest(int a, int b, int c)
{
	if (a > b) {
		if (a > c) {
			return a;
		}
		else {
			return c;
		}
	}
	else {
		if (b > c) {
			return b;
		}
		else {
			return c;
		}
	}
}
int find_lowest(int a, int b, int c)
{
	if (a < b) {
		if (a < c) {
			return a;
		}
		else {
			return c;
		}
	}
	else {
		if (b < c) {
			return b;
		}
		else {
			return c;
		}
	}
}
int main(int argc, char **argv)
{
	if(argc < 4) {
		cout << "Usage: L01 filename width height" << endl;
		return 0;
	}
	// Output filename
	string filename(argv[1]);
	// Width of image
	int width = atoi(argv[2]);
	// Height of image
	int height = atoi(argv[3]);
	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);
	// Vertex 1 X
	int v1x = atoi(argv[4]);
	// Vertex 1 Y
	int v1y = atoi(argv[5]);
	// Vertex 2 X
	int v2x = atoi(argv[6]);
	// Vertex 2 Y
	int v2y = atoi(argv[7]);
	// Vertex 3 X
	int v3x = atoi(argv[8]);
	// Vertex 3 Y
	int v3y = atoi(argv[9]);
	// Draw a triangle
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 255;
		image->setPixel(v1x, v1y, r, g, b);
		image->setPixel(v2x, v2y, r, g, b);
		image->setPixel(v3x, v3y, r, g, b);
	// Bounding Box Creation
		int xmax = find_highest(v1x, v2x, v3x);
		int xmin = find_lowest(v1x, v2x, v3x);
		int ymax = find_highest(v1y, v2y, v3y);
		int ymin = find_lowest(v1y, v2y, v3y);
		BoundingBox bbox = BoundingBox(xmax, xmin, ymax, ymin);
		for (int i = bbox.xmin; i < bbox.xmax; i++) {
			for (int j = bbox.ymin; j < bbox.ymax; j++) {
				if (j % 2 == 0) {
					r = 0;
					g = 255;
					image->setPixel(i, j, r, g, b);
				}
				else {
					r = 255;
					g = 0;
					image->setPixel(i, j, r, g, b);
				}
			}
		}
	// Write image to file
	image->writeToFile(filename);
	return 0;
}
