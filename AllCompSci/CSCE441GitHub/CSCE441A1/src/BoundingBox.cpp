#include "BoundingBox.h"

//Find triangle area using Triangle class and 1/2 w * h formula 
float triangle_area(Triangle t) {
	float tArea = (float).5 * ((t.x2 - t.x1) * (t.y3 - t.y1) - (t.x3 - t.x1) * (t.y2 - t.y1));
	return tArea;
}

//Find triangle area given 6 coordinates of a triangle and 1/2 b * h formula
float point_tri_area(float x1, float x2, float x3, float y1, float y2, float y3) {
	float tArea = (float).5 * ((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1));
	return tArea;
}

//Scale a value according to the maximum and minimum values of the image and the ratio of 
//the point divided by the total image
float scale_z(float z, float zmin, float zmax) {
	float new_z = (z - zmin) / (zmax - zmin);
	return new_z;
}

//Obtain barycentric coordinates using two random points and a given triangle, formula
//obtained from https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
vector<float> bary(int x, int y, Triangle t) {
	vector<float> coordinates;
	float total_area = triangle_area(t);
	float alpha = point_tri_area((float)x, (float)t.x2, (float)t.x3, (float)y, (float)t.y2, (float)t.y3) / total_area;
	coordinates.push_back(alpha);
	float beta = point_tri_area((float)x, (float)t.x3, (float)t.x1, (float)y, (float)t.y3, (float)t.y1) / total_area;
	coordinates.push_back(beta);
	float gamma = point_tri_area((float)x, (float)t.x1, (float)t.x2, (float)y, (float)t.y1, (float)t.y2) / total_area;
	coordinates.push_back(gamma);
	return coordinates;
}


//Initialize Bounding box with max and min x y and z values as well as the triangle it is bounding
BoundingBox::BoundingBox(float xbig, float xsmall, float ybig, float ysmall, float zbig, float zsmall, Triangle t){
	xmin = xsmall;
	xmax = xbig;
	ymin = ysmall;
	ymax = ybig;
	zmin = zsmall;
	zmax = zbig;
	tri = t;
}

//Create rectangular bounding box by looping through all coordinates in between
//given max and min values 
void BoundingBox::createBox(shared_ptr<Image> &image){
	char r = rand() % 256;
	char g = rand() % 256;
	char b = rand() % 256;
	//For Bounding Boxes
	for (int i = (int)this->xmin; i < (int)this->xmax; i++) {
			for (int j = (int)this->ymin; j < (int)this->ymax; j++) {
					image->setPixel(i, j, r, g, b);
			}
		}
	
}

//Create triangular image by using barycentric coordinates formula (listed above)
//and looping through all coordinates in between max and min values
void BoundingBox::createTriangle(shared_ptr<Image> &image) {
	//Initialize each point with a random r g and b value
	float p1r =  (float)(rand() % 256);
	float p1g =  (float)(rand() % 256);
	float p1b =  (float)(rand() % 256);
	float p2r =  (float)(rand() % 256);
	float p2g =  (float)(rand() % 256);
	float p2b =  (float)(rand() % 256);
	float p3r =  (float)(rand() % 256);
	float p3g =  (float)(rand() % 256);
	float p3b =  (float)(rand() % 256);
	for (int i = (int)this->xmin; i < this->xmax; i++) {
		for (int j = (int)this->ymin; j < this->ymax; j++) {
			vector<float> bar = bary(i, j, tri);
			//Barycentric coordinates must be in between 0 and 1
			if ((bar[0] >= 0 && bar[0] <= 1) && (bar[1] >= 0 && bar[1] <= 1) && (bar[2] >= 0 && bar[2] <= 1)) {
				//Calculate new colors by multiplying r g and b values by given barycentric values
				unsigned char new_r = (char)((bar[0] * p1r) + (bar[1] * p2r) + (bar[2] * p3r));
				unsigned char new_g = (char)((bar[0] * p1g) + (bar[1] * p2g) + (bar[2] * p3g));
				unsigned char new_b = (char)((bar[0] * p1b) + (bar[1] * p2b) + (bar[2] * p3b));
				image->setPixel(i, j, new_r, new_g, new_b);
			}
		}
	}
}

//Color the image using a z buffer and looping through all given values in between the minimum and maximum
//Values of the triangle.
//The image is blue towards the back and red towards the front
void BoundingBox::createZColor(shared_ptr<Image> &image, vector<vector<float>> &zBuf, BoundingBox total_bbox) {
	for (int i = (int)this->xmin; i < this->xmax; i++) {
		for (int j = (int)this->ymin; j < this->ymax; j++) {
			vector<float> bar = bary(i, j, tri);
			//Barycentric values must be in between 0 and 1
			if ((bar[0] >= 0 && bar[0] <= 1) && (bar[1] >= 0 && bar[1] <= 1) && (bar[2] >= 0 && bar[2] <= 1)) {
				//Calculate z value using barycentric values
				float z = (bar[0] * tri.z1) + (bar[1] * tri.z2) + (bar[2] * tri.z3);
				//Scale z value according to the maximum and minimum z values in the mesh
				float scaleZ = scale_z(z, total_bbox.zmin, total_bbox.zmax);

				//Scale rgb values according to the scaled z value
				char new_r = (char)(((1 - scaleZ) * 0) + (scaleZ * 255));
				char new_g = (char)(((1 - scaleZ) * 0) + (scaleZ * 0));
				char new_b = (char)(((1 - scaleZ) * 255) + (scaleZ * 0));
				//Store z buffer value and draw pixel if new z value exists
				//so there is no pixel drawing overlap
				if (zBuf[i][j] < z) {
					zBuf[i][j] = z;
					image->setPixel(i, j, new_r, new_g, new_b);
				}
			}
		}
	}
}

//Color the image using a the z buffer teqnique but with the y values and looping through 
//all given values in between the minimum and maximum Values of the triangle.
//The image is red towards the top and pink towards the bottom
void BoundingBox::createYColor(shared_ptr<Image> &image, vector<vector<float>> &zBuf, BoundingBox total_bbox, float new_ymax, float new_ymin) {
	for (int i = (int)this->xmin; i < this->xmax; i++) {
		for (int j = (int)this->ymin; j < this->ymax; j++) {
			vector<float> bar = bary(i, j, tri);
			//Barycentric values must be in between 0 and 1
			if ((bar[0] >= 0 && bar[0] <= 1) && (bar[1] >= 0 && bar[1] <= 1) && (bar[2] >= 0 && bar[2] <= 1)) {
				//Scale y value using the given y coordinate and the maximum and minimum coordinate y values of
				//the scaled image
				float scaleY = scale_z((float)j, new_ymin, new_ymax);

				//Scale rgb values according to the scaled y value
				char new_r = (char)(((1 - scaleY) * 255) + (scaleY * 255));
				char new_g = (char)(((1 - scaleY) * 0) + (scaleY * 0));
				char new_b = (char)(((1 - scaleY) * 255) + (scaleY * 0));


				float z = (bar[0] * tri.z1) + (bar[1] * tri.z2) + (bar[2] * tri.z3);
				//Store z buffer value and draw pixel if new z value exists
				//so there is no pixel drawing overlap
				if (zBuf[i][j] < z) {
					zBuf[i][j] = z;
					image->setPixel(i, j, new_r, new_g, new_b);
				}
			}
		}
	}
}

//Resize the image to fit the size of the screen given by width and height in pixels
void BoundingBox::resize(BoundingBox total_bbox, int width, int height) {
	//Find height and width values of total mesh
	float width_big = total_bbox.xmax - total_bbox.xmin;
	float height_big = total_bbox.ymax - total_bbox.ymin;
	//Subtract from total width and height and half to obtain value to center the image
	float width_conv = ((float)width - width_big) / (float)2.0;
	float height_conv = ((float)height - height_big) / (float)2.0;
	//Use conversion value to center the image
	this->xmin += width_conv;
	this->xmax += width_conv;
	this->ymin += height_conv;
	this->ymax += height_conv;
	tri.x1 += width_conv;
	tri.x2 += width_conv;
	tri.x3 += width_conv;
	tri.y1 += height_conv;
	tri.y2 += height_conv;
	tri.y3 += height_conv;
	//Use ratios to determine what the new height and width values should be to scale
	//image to screen size
	float new_width = ((width_big / height_big) * (float)height);
	float new_height = ((height_big / width_big) * (float)width);
	//Find x and y value coordinates at the center of the screen
	const float x_center = (float)width / (float)2.0;
	const float y_center = (float)height / (float)2.0;
	//Determine ratio of height and width using the new width and height just determined above
	const float width_ratio = new_width / width_big;
	const float height_ratio = new_height / height_big;
	//If the width ratio is greater than the height ratio, use the width ratio as to not
	//distort the dimensions of the image
	if(width_ratio > height_ratio){
		//Determine new coordinate values by multiplying ratio against difference between values
		//and center coordinates and adding that value to the numeric value of the center
		//coordinates
		this->xmin = (x_center + ((height_ratio) * ((this->xmin - x_center))));
		this->xmax = (x_center + ((height_ratio) * ((this->xmax - x_center))));
		this->ymin = (y_center + ((height_ratio) * ((this->ymin - y_center))));
		this->ymax = (y_center + ((height_ratio) * ((this->ymax - y_center))));
		tri.x1 = (x_center + ((height_ratio) * ((tri.x1 - x_center))));
		tri.x2 = (x_center + ((height_ratio) * ((tri.x2 - x_center))));
		tri.x3 = (x_center + ((height_ratio) * ((tri.x3 - x_center))));
		tri.y1 = (y_center + ((height_ratio) * ((tri.y1 - y_center))));
		tri.y2 = (y_center + ((height_ratio) * ((tri.y2 - y_center))));
		tri.y3 = (y_center + ((height_ratio) * ((tri.y3 - y_center))));
	}
	//If the width ratio is less than the height ratio, use the height ratio as to not
	//distort the dimensions of the image
	else{
		//Determine new coordinate values by multiplying ratio against difference between values
		//and center coordinates and adding that value to the numeric value of the center
		//coordinates
		this->xmin = (x_center + ((width_ratio) * ((this->xmin - x_center))));
		this->xmax = (x_center + ((width_ratio) * ((this->xmax - x_center))));
		this->ymin = (y_center + ((width_ratio) * ((this->ymin - y_center))));
		this->ymax = (y_center + ((width_ratio) * ((this->ymax - y_center))));
		tri.x1 = (x_center + ((width_ratio) * ((tri.x1 - x_center))));
		tri.x2 = (x_center + ((width_ratio) * ((tri.x2 - x_center))));
		tri.x3 = (x_center + ((width_ratio) * ((tri.x3 - x_center))));
		tri.y1 = (y_center + ((width_ratio) * ((tri.y1 - y_center))));
		tri.y2 = (y_center + ((width_ratio) * ((tri.y2 - y_center))));
		tri.y3 = (y_center + ((width_ratio) * ((tri.y3 - y_center))));
		
	}
}