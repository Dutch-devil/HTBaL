#ifndef Wall_H_
#define Wall_H_

#include "gameplay.h"

using namespace gameplay;
#include "Floor.h"

class Wall {
public:
	Wall(Node* node, bool door = false);
    ~Wall();

	static void calculateMesh();
	static void finalize();

	Node* getNode();
	Mesh* getMesh();
	Model* getModel();

protected:
	static Mesh* mesh;
	bool door;
	Node* node;
	Model* model;
};

#endif