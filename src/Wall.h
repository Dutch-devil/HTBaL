#ifndef Wall_H_
#define Wall_H_

#include "gameplay.h"

using namespace gameplay;
#include "Floor.h"

class Wall {
public:
	Wall(Vector2* start, Vector2* end);
	Wall(Vector2* start, Vector2* end, bool door);
    ~Wall();

	Vector2* getStart();
	Vector2* getEnd();

	Mesh* getMesh();
	Model* getModel();

protected:
	Vector2* start;
	Vector2* end;
	bool door;
	Model* model;
};

#endif