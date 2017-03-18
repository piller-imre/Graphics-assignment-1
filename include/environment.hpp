#pragma once
#include "sky.hpp"
#include "object.hpp"
#include "forest.hpp"
#include "camera.hpp"

class Environment
{
public:
	Environment();
	~Environment();
	/// reads and prepares objects and textures (sky, forest, trees, terrain)
	void initialize(HeightMapLoader* heightMap, Camera* camera);
	/// draws everything
	void update();
private:
	Sky *pSky;
	Tree *tree1;
	std::string tree1FileName = "pine1";
	Tree *tree2;
	std::string tree2FileName = "pine2";
	Tree *tree3;
	std::string tree3FileName = "pine3";
	Forest* forest;
	std::string forestFileName = "population.png";
	float visualRange = 6.5f;

	/// pointers to foreign objects
	HeightMapLoader* heightmap;
	Camera* m_pCamera;

	float calcDistanceToCamera(vec3f position);
	void drawTree(Tree* tree, float distanceToCamera);
	Tree* determineTree(std::string treeId);
};