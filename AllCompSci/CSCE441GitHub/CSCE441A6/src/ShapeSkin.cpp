#include <iostream>
#include <fstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "ShapeSkin.h"
#include "GLSL.h"
#include "Program.h"

using namespace std;

ShapeSkin::ShapeSkin() :
	prog(NULL),
	elemBufID(0),
	posBufID(0),
	norBufID(0),
	texBufID(0),
	weightBufID(0),
	boneBufID(0),
	infBufID(0),
	skinBufID(0)
{
}

ShapeSkin::~ShapeSkin()
{
}

void ShapeSkin::loadMesh(const string &meshName)
{
	// Load geometry
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if(!rc) {
		cerr << errStr << endl;
	} else {
		posBuf = attrib.vertices;
		norBuf = attrib.normals;
		texBuf = attrib.texcoords;
		assert(posBuf.size() == norBuf.size());
		// Loop over shapes
		for(size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			const tinyobj::mesh_t &mesh = shapes[s].mesh;
			size_t index_offset = 0;
			for(size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
				size_t fv = mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = mesh.indices[index_offset + v];
					elemBuf.push_back(idx.vertex_index);
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	skinBuf = posBuf;
	origNormBuf = norBuf;
}

vector<vector<float>> ShapeSkin::loadAttachment(const std::string &filename)
{
	ifstream in;
	in.open(filename);
	if(!in.good()) {
		cout << "Cannot read " << filename << endl;
		return sweights;
	}
	string line;
	getline(in, line); // comment
	getline(in, line); // comment
	getline(in, line);
	//Set number of bones and verticies after first line of file
	stringstream ss0(line);
	ss0 >> nverts;
	ss0 >> nbones;
	assert(nverts == posBuf.size()/3);
	int vertex = 0;
	int bone = 0;
	while (1) {
		if (in.eof()) {
			break;
		}
		getline(in, line);
		// Parse line
		stringstream ss(line);
		std::vector<float> k;
		//Loop through all bones and push back bone weights for each vertex
		for (int i = 0; i < nbones; i++) {
			float m;
			ss >> m;
			k.push_back(m);
		}
		sweights.push_back(k);
		k.clear();
		vertex++;
	}
	in.close();
	return sweights;
}

vector<glm::mat4> ShapeSkin::loadSkeleton(const std::string &filename) {
	
	ifstream in;
	in.open(filename);
	if (!in.good()) {
		cout << "Cannot read " << filename << endl;
		return matvector;
	}
	string line;
	getline(in, line); // comment
	getline(in, line); // comment
	getline(in, line); // comment
	getline(in, line);
	//Set num of frame and num of bones from first line of file
	stringstream ss0(line);
	ss0 >> frameCount;
	ss0 >> boneCount;
	int count = 0;
	//Reserve Memory for size of vector large enough to hold frame * bone elements
	matvector.reserve(frameCount * boneCount);
	//Run through rest of matrix to set animation matricies
	while(1){
		if (in.eof()) {
			break;
		}
		getline(in, line);
		stringstream ss(line);
		// Parse line and set matrix per bone
		for (int i = 0; i < nbones; i++) {
			glm::quat q;
			ss >> q.x;
			ss >> q.y;
			ss >> q.z;
			ss >> q.w;
			glm::vec3 p;
			ss >> p.x;
			ss >> p.y;
			ss >> p.z;
			glm::mat4 E = glm::mat4_cast(q);
			E[3] = glm::vec4(p, 1.0f);
			//Create inverted Bind Matrix vector
			if (count == 0) {
				bindvec.push_back(glm::inverse(E));
			}
			matvector.push_back(E);
		}
		count++;
	}
	in.close();
	return matvector;
}

void ShapeSkin::calcSkinning() {
	//Reserve weight, bone index and number of influences vectors size
	boneBuf.reserve(16 * nverts);
	weightBuf.reserve(16 * nverts);
	infBuf.reserve(nverts);
	//Loop through all frames
	for (int k = 0; k < (frameCount + 1); k++) {
		std::vector<glm::vec4> skintemp, newnormtemp;
		//Loop through all verticies
		for (size_t i = 0; i < sweights.size(); i++) {
			glm::vec4 total = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			glm::vec4 totalnorm = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			int index = i * 3;
			float zero_count = 0;
			//Loop through all bones and sum the calculated verticies
			for (int j = 0; j < nbones; j++) {
				if (sweights[i][j] != 0.0f) {
					glm::vec4 x = glm::vec4(skinBuf[index], skinBuf[(index + 1)], skinBuf[(index + 2)], 1.0f);
					glm::vec4 temp = (sweights[i][j] * (matvector[((k * nbones) + j)] * (bindvec[j] * x)));
					total = total + temp;
					glm::vec4 y = glm::vec4(origNormBuf[index], origNormBuf[(index + 1)], origNormBuf[(index + 2)], 0.0f);
					glm::vec4 temp1 = (sweights[i][j] * (matvector[((k * nbones) + j)] * (bindvec[j] * y)));
					totalnorm = totalnorm + temp1;
					//Set bone index and weight buffer for one frame, since all frames are the same for weight and bone index
					if (k == 0) {
						boneBuf.push_back(float(j));
						weightBuf.push_back(sweights[i][j]);
					}
				}
				//Count how many weights are zero per vertex
				else {
					zero_count++;
				}
			}
			//Push back zero count - 2 0's into bone and weight buffer to set length to be 16 for each attribute.
			if (k == 0) {
				for (int h = 0; h < (zero_count - 2); h++) {
					boneBuf.push_back(0.0f);
					weightBuf.push_back(0.0f);
				}
				infBuf.push_back(zero_count - 2);
			}
			skintemp.push_back(total);
			newnormtemp.push_back(totalnorm);
		}
		skin.push_back(skintemp);
		newnorm.push_back(newnormtemp);
	}
}

//Return inverted Bind Matrix Vector
std::vector<glm::mat4> ShapeSkin::getBindVec() {
	return bindvec;
}

//Return animation vector for specific frame
std::vector<glm::mat4> ShapeSkin::getAnimVec(int frame, float g_boolean) {
	std::vector<glm::mat4> returnvec;
	for (int i = frame; i < (frame + nbones); i++) {
		returnvec.push_back(matvector[i]);
	}
	//If GPU skinning is selected, reset position and normal buffers to bind pose
	if (int(g_boolean) == 1) {
		posBuf = skinBuf;
		norBuf = origNormBuf;
		glGenBuffers(1, &posBufID);
		glBindBuffer(GL_ARRAY_BUFFER, posBufID);
		glBufferData(GL_ARRAY_BUFFER, posBuf.size() * sizeof(float), &posBuf[0], GL_STATIC_DRAW);


		glGenBuffers(1, &norBufID);
		glBindBuffer(GL_ARRAY_BUFFER, norBufID);
		glBufferData(GL_ARRAY_BUFFER, norBuf.size() * sizeof(float), &norBuf[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	return returnvec;
}

void ShapeSkin::implementSkinning(int frame) {
	int count = 0;
	//Loop through all verticies and set new coordinates from CPU skinning.
	for (size_t i = 0; i < posBuf.size(); i = i + 3) {
		glm::vec3 t = glm::vec3(skin[((frame / 18))][count]);
		glm::vec3 m = glm::vec3(newnorm[((frame / 18))][count]);
		posBuf[i] = t.x;
		posBuf[i + 1] = t.y;
		posBuf[i + 2] = t.z;
		norBuf[i] = m.x;
		norBuf[i + 1] = m.y;
		norBuf[i + 2] = m.z;
		count++;
	}
	//Reinitialize Position and Normal Buffers after each new frame calculation for GPU skinning.
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size() * sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	

	glGenBuffers(1, &norBufID);
	glBindBuffer(GL_ARRAY_BUFFER, norBufID);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size() * sizeof(float), &norBuf[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ShapeSkin::init()
{
	
	// Send the position array to the GPU
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	
	// Send the normal array to the GPU
	glGenBuffers(1, &norBufID);
	glBindBuffer(GL_ARRAY_BUFFER, norBufID);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);

	// No texture info
	texBufID = 0;

	//Send weight buffer to GPU
	glGenBuffers(1, &weightBufID);
	glBindBuffer(GL_ARRAY_BUFFER, weightBufID);
	glBufferData(GL_ARRAY_BUFFER, weightBuf.size() * sizeof(float), &weightBuf[0], GL_STATIC_DRAW);

	//Send bone index buffer to GPU
	glGenBuffers(1, &boneBufID);
	glBindBuffer(GL_ARRAY_BUFFER, boneBufID);
	glBufferData(GL_ARRAY_BUFFER, boneBuf.size() * sizeof(float), &boneBuf[0], GL_STATIC_DRAW);

	//Send influence buffer to GPU
	glGenBuffers(1, &infBufID);
	glBindBuffer(GL_ARRAY_BUFFER, infBufID);
	glBufferData(GL_ARRAY_BUFFER, infBuf.size() * sizeof(float), &infBuf[0], GL_STATIC_DRAW);
	
	// Send the element array to the GPU
	glGenBuffers(1, &elemBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemBuf.size()*sizeof(unsigned int), &elemBuf[0], GL_STATIC_DRAW);
	
	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	assert(glGetError() == GL_NO_ERROR);

}

void ShapeSkin::draw() const
{
	assert(prog);
	// Send the position array to the GPU

	int h_pos = prog->getAttribute("aPos");
	glEnableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
	
	int h_nor = prog->getAttribute("aNor");
	glEnableVertexAttribArray(h_nor);
	glBindBuffer(GL_ARRAY_BUFFER, norBufID);
	glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
	

	//Send weight info to corresponding attribute vectors
	int h_weight0 = prog->getAttribute("weights0");
	glEnableVertexAttribArray(h_weight0);
	int h_weight1 = prog->getAttribute("weights1");
	glEnableVertexAttribArray(h_weight1);
	int h_weight2 = prog->getAttribute("weights2");
	glEnableVertexAttribArray(h_weight2);
	int h_weight3 = prog->getAttribute("weights3");
	glEnableVertexAttribArray(h_weight3);
	glBindBuffer(GL_ARRAY_BUFFER, weightBufID);
	unsigned stride = 16 * sizeof(float);
	glVertexAttribPointer(h_weight0, 4, GL_FLOAT, GL_FALSE, stride, (const void *)(0 * sizeof(float)));
	glVertexAttribPointer(h_weight1, 4, GL_FLOAT, GL_FALSE, stride, (const void *)(4 * sizeof(float)));
	glVertexAttribPointer(h_weight2, 4, GL_FLOAT, GL_FALSE, stride, (const void *)(8 * sizeof(float)));
	glVertexAttribPointer(h_weight3, 4, GL_FLOAT, GL_FALSE, stride, (const void *)(12 * sizeof(float)));
	
	//Send bone index info to corresponding attribute vectors
	int h_bone0 = prog->getAttribute("bones0");
	glEnableVertexAttribArray(h_bone0);
	int h_bone1 = prog->getAttribute("bones1");
	glEnableVertexAttribArray(h_bone1);
	int h_bone2 = prog->getAttribute("bones2");
	glEnableVertexAttribArray(h_bone2);
	int h_bone3 = prog->getAttribute("bones3");
	glEnableVertexAttribArray(h_bone3);
	glBindBuffer(GL_ARRAY_BUFFER, boneBufID);
	glVertexAttribPointer(h_bone0, 4, GL_FLOAT, GL_FALSE, stride, (const void *)(0 * sizeof(float)));
	glVertexAttribPointer(h_bone1, 4, GL_FLOAT, GL_FALSE, stride, (const void *)(4 * sizeof(float)));
	glVertexAttribPointer(h_bone2, 4, GL_FLOAT, GL_FALSE, stride, (const void *)(8 * sizeof(float)));
	glVertexAttribPointer(h_bone3, 4, GL_FLOAT, GL_FALSE, stride, (const void *)(12 * sizeof(float)));

	//Send influence info to corresponding attribute vectors
	int h_inf = prog->getAttribute("numInfl");
	glEnableVertexAttribArray(h_inf);
	glBindBuffer(GL_ARRAY_BUFFER, infBufID);
	glVertexAttribPointer(h_inf, 1, GL_FLOAT, GL_FALSE, 0, (const void *)0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBufID);
	glDrawElements(GL_TRIANGLES, (int)elemBuf.size(), GL_UNSIGNED_INT, (const void *)0);

	//Disable all vertex attribute arrays
	glDisableVertexAttribArray(h_bone0);
	glDisableVertexAttribArray(h_bone1);
	glDisableVertexAttribArray(h_bone2);
	glDisableVertexAttribArray(h_bone3);
	glDisableVertexAttribArray(h_inf);
	glDisableVertexAttribArray(h_weight0);
	glDisableVertexAttribArray(h_weight1);
	glDisableVertexAttribArray(h_weight2);
	glDisableVertexAttribArray(h_weight3);
	glDisableVertexAttribArray(h_nor);
	glDisableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGetError();
}

