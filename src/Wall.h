#ifndef Wall_H_
#define Wall_H_

#include "gameplay.h"

using namespace std;
using namespace gameplay;

class Wall {
public:
	Wall(RenderState::StateBlock* stateBlock, Vector2* start, Vector2* end);
    ~Wall();

	Vector2* getStart();
	Vector2* getEnd();

	Mesh* getMesh();

protected:
	Vector2* start;
	Vector2* end;
	Model* wallModel;
};

#endif