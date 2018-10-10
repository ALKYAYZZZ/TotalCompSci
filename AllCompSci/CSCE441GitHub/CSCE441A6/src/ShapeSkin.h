#pragma once
#ifndef _SHAPESKIN_H_
#define _SHAPESKIN_H_

#include <memory>


class MatrixStack;
class Program;

class ShapeSkin
{
public:
	ShapeSkin();
	virtual ~ShapeSkin();
	void loadMesh(const std::string &meshName);
	std::vector<std::vector<float>> loadAttachment(const std::string &filename);
	std::vector<glm::mat4> loadSkeleton(const std::string &filename);
	void calcSkinning();
	void implementSkinning(int frame);
	void setProgram(std::shared_ptr<Program> p) { prog = p; }
	std::vector<glm::mat4> getBindVec();
	std::vector<glm::mat4> getAnimVec(int frame, float g_boolean);
	void init();
	void draw() const;
	
private:
	std::shared_ptr<Program> prog;
	std::vector<unsigned int> elemBuf;
	std::vector<float> posBuf;
	std::vector<float> weightBuf;
	std::vector<float> boneBuf;
	std::vector<float> infBuf;
	std::vector<float> skinBuf;
	std::vector<float> origNormBuf;
	std::vector<float> norBuf;
	std::vector<float> texBuf;
	unsigned elemBufID;
	unsigned posBufID;
	unsigned weightBufID;
	unsigned boneBufID;
	unsigned infBufID;
	unsigned norBufID;
	unsigned texBufID;
	unsigned skinBufID;
	std::vector<std::vector<float>> sweights;
	std::vector<glm::mat4> matvector;
	std::vector<glm::mat4> bindvec;
	std::vector<std::vector<glm::vec4>> skin;
	std::vector<std::vector<glm::vec4>> newnorm;
	int nverts, nbones;
	int frameCount, boneCount;
};

#endif
