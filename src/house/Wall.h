#ifndef Wall_H_
#define Wall_H_

class Wall;

#include "gameplay.h"
#include "Floor.h"

using namespace gameplay;

class Wall {
public:
	Wall(bool inverted);
	Wall(bool inverted, bool door);
    ~Wall();

	static void calculateMesh();
	static void finalize();

	Node* getNode();
	Mesh* getMesh();
	Model* getModel();
	
	static void releaseMesh();

protected:
	static Mesh* mesh, *mesh_inv;
	bool inverted, door;
	Model* model;
};

#endif