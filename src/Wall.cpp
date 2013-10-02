#include "Wall.h"

using namespace std;

Wall::Wall(RenderState::StateBlock* stateBlock, Vector2* start, Vector2* end): start(start), end(end) {
	wallModel = Model::create(getMesh());

    // Create the ground material
    Material* wallMaterial = wallModel->setMaterial("res/grid.material");
    wallMaterial->setStateBlock(stateBlock);
}


Wall::~Wall() {
}

Vector2* Wall::getStart() {
	return start;
}

Vector2* Wall::getEnd() {
	return end;
}

Mesh* Wall::getMesh() {
	return NULL;
	//return Mesh::createQuad(Vector3(prevWall->x, prevWall->y, 0),
 //                                             Vector3(wall->x, wall->y, 0),
 //                                             Vector3(prevWall->x, prevWall->y, (Floor::width + Floor::height) / 2),
 //                                             Vector3(wall->x, wall->y, (Floor::width + Floor::height) / 2));
}
