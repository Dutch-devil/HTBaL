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
	static void finalize();

	Mesh* getMesh();
	Model* getModel();

protected:
	static Mesh* mesh;
	bool door;
	Model* model;
};

#endif