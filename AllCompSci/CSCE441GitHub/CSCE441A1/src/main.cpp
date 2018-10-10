#include <iostream>
#include <string>
#include "BoundingBox.h"
#include "Triangle.h"
#include "Image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;


int main(int argc, char **argv)
{
	if(argc < 6) {
		cout << "Usage: A1 meshfile filename width height color_mode" << endl;
		return 0;
	}
	string meshName(argv[1]);
	
	string filename(argv[2]);
	
	// Width of image
	int width = atoi(argv[3]);
	// Height of image
	int height = atoi(argv[4]);
	//Color Mode
	int mode = atoi(argv[5]);
	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);
	// Load geometry
	vector<float> posBuf; // list of vertex positions
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if(!rc) {
		cerr << errStr << endl;
	} else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for(size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	//Create 3D vector to store all z values, initialize with -1000000 in each spot
	vector<vector<float>> zBuf;
	for (int i = 0; i < width; i++) {
		vector<float> line;
		for (int j = 0; j < height; j++) {
			line.push_back(-100000);
		}
		zBuf.push_back(line);
	}
	cout << "Number of vertices: " << posBuf.size()/3 << endl;
	//Initialize total max and min with corresponding high or low number
	float xmax_vec = -100000000;
	float xmin_vec = 100000000;
	float ymax_vec = -100000000;
	float ymin_vec = 100000000;
	float zmin_vec = 100000000;
	float zmax_vec = -100000000;
	//Create vector of bounding boxes
	vector<BoundingBox> bbox_vec = vector<BoundingBox>();
	//Loop through posBuf to get all x y and z values and store the highest and lowest values for total bounding box.
	for(size_t i = 0; i < posBuf.size(); i += 9){
		Triangle t = Triangle(posBuf[i], posBuf[i + 3], posBuf[i + 6], posBuf[i + 1], posBuf[i + 4], posBuf[i + 7], posBuf[i + 2], posBuf[i + 5], posBuf[i + 8]);
		float highest_x = t.find_highest(t.x1, t.x2, t.x3);
		if (highest_x > xmax_vec) {
			xmax_vec = highest_x;
		}
		float highest_y = t.find_highest(t.y1, t.y2, t.y3);
		if (highest_y > ymax_vec) {
			ymax_vec = highest_y;
		}
		float lowest_x = t.find_lowest(t.x1, t.x2, t.x3);
		if (lowest_x < xmin_vec) {
			xmin_vec = lowest_x;
		}
		float lowest_y = t.find_lowest(t.y1, t.y2, t.y3);
		if (lowest_y < ymin_vec) {
			ymin_vec = lowest_y;
		}
		float lowest_z = t.find_lowest(t.z1, t.z2, t.z3);
		if (lowest_z < zmin_vec) {
			zmin_vec = lowest_z;
		}
		float highest_z = t.find_highest(t.z1, t.z2, t.z3);
		if (highest_z > zmax_vec) {
			zmax_vec = highest_z;
		}
		BoundingBox bbox = BoundingBox(highest_x,lowest_x,highest_y, lowest_y, highest_z, lowest_z, t);
		bbox_vec.push_back(bbox);
	}
	//If xmin is offset into negative coordinates, move all x coordinates to the right by the lowest x value
	if (xmin_vec < 0) {
		float conversion = xmin_vec;
		for (size_t i = 0; i < bbox_vec.size(); i++) {
			bbox_vec[i].xmin = bbox_vec[i].xmin - conversion;
			bbox_vec[i].xmax = bbox_vec[i].xmax - conversion;
			bbox_vec[i].tri.x1 = bbox_vec[i].tri.x1 - conversion;
			bbox_vec[i].tri.x2 = bbox_vec[i].tri.x2 - conversion;
			bbox_vec[i].tri.x3 = bbox_vec[i].tri.x3 - conversion;
		}
		xmin_vec = 0;
		xmax_vec -= conversion;
	}
	//If xmin is offset into positive coordinates, move all x coordinates to the left by the lowest x value
	else if (xmin_vec > 0) {
		float conversion = xmin_vec;
		for (size_t i = 0; i < bbox_vec.size(); i++) {
			bbox_vec[i].xmin = bbox_vec[i].xmin - conversion;
			bbox_vec[i].xmax = bbox_vec[i].xmax - conversion;
			bbox_vec[i].tri.x1 = bbox_vec[i].tri.x1 - conversion;
			bbox_vec[i].tri.x2 = bbox_vec[i].tri.x2 - conversion;
			bbox_vec[i].tri.x3 = bbox_vec[i].tri.x3 - conversion;
		}
		xmin_vec = 0;
		xmax_vec -= conversion;
	}
	//If ymin is offset into negative coordinates, move all y coordinates to the up by the lowest y value
	if (ymin_vec < 0) {
		float conversion = ymin_vec;
		for (size_t i = 0; i < bbox_vec.size(); i++) {
			bbox_vec[i].ymin = bbox_vec[i].ymin - conversion;
			bbox_vec[i].ymax = bbox_vec[i].ymax - conversion;
			bbox_vec[i].tri.y1 = bbox_vec[i].tri.y1 - conversion;
			bbox_vec[i].tri.y2 = bbox_vec[i].tri.y2 - conversion;
			bbox_vec[i].tri.y3 = bbox_vec[i].tri.y3 - conversion;
		}
		ymin_vec = 0;
		ymax_vec -= conversion;
	}
	//If ymin is offset into positive coordinates, move all y coordinates to the down by the lowest y value
	else if (ymin_vec > 0) {
		float conversion = ymin_vec;
		for (size_t i = 0; i < bbox_vec.size(); i++) {
			bbox_vec[i].ymin = bbox_vec[i].ymin - conversion;
			bbox_vec[i].ymax = bbox_vec[i].ymax - conversion;
			bbox_vec[i].tri.y1 = bbox_vec[i].tri.y1 - conversion;
			bbox_vec[i].tri.y2 = bbox_vec[i].tri.y2 - conversion;
			bbox_vec[i].tri.y3 = bbox_vec[i].tri.y3 - conversion;
		}
		ymin_vec = 0;
		ymax_vec -= conversion;
	}
	Triangle temp = Triangle();
	//Initialize bounding box for entire mesh using max and min values
	BoundingBox final_box = BoundingBox(xmax_vec, xmin_vec, ymax_vec, ymin_vec, zmax_vec, zmin_vec, temp);
	//Initialize scaled y max and min values to corresponding integer
	float scaled_ymax = -10000000;
	float scaled_ymin = 10000000;
	//Loop through Bounding Boxes to scale to the image size, also store highest and lowest scaled y values for color mode 2
	for (size_t i = 0; i < bbox_vec.size(); i++) {
		bbox_vec[i].resize(final_box, width, height);
		if (bbox_vec[i].ymax > scaled_ymax) {
			scaled_ymax = bbox_vec[i].ymax;
		}
		if (bbox_vec[i].ymin < scaled_ymin) {
			scaled_ymin = bbox_vec[i].ymin;
		}
	}
	//Loop through Bounding Boxes to draw pixels on image using corresponding color mode function
	for(size_t i = 0; i < bbox_vec.size(); i++){
		if (mode == 0) {
			bbox_vec[i].createTriangle(image);
		}
		else if (mode == 1) {
			bbox_vec[i].createZColor(image, zBuf, final_box);
		}
		else if (mode == 2) {
			bbox_vec[i].createYColor(image, zBuf, final_box, scaled_ymax, scaled_ymin);
		}
		else {
			cerr << "Invalid Color Code" << endl;
		}
	}
	// Write image to file
	image->writeToFile(filename);
	return 0;
}
