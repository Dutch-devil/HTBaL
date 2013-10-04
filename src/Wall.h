#ifndef Wall_H_
#define Wall_H_

#include "gameplay.h"

using namespace gameplay;
#include "Floor.h"

class Wall {
public:
	Wall();
	Wall(bool door);
    ~Wall();

	static void calculateMesh();
	Mesh* getMesh();
	Model* getModel();
	
	static void releaseMesh();

protected:
	static Mesh* mesh;
	bool door;
	Model* model;
};

#endif