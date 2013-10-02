#include "Wall.h"

Wall::Wall(RenderState::StateBlock* stateBlock, Vector2* start, Vector2* end): start(start), end(end) {
	model = Model::create(getMesh());

    // Create the ground material
    Material* wallMaterial = model->setMaterial("res/grid.material");
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
	return Mesh::createQuad(Vector3(start->x, start->y, 0),
                            Vector3(end->x, end->y, 0),
                            Vector3(start->x, start->y, (Floor::width + Floor::height) / 2),
                            Vector3(end->x, end->y, (Floor::width + Floor::height) / 2));
}

Model* Wall::getModel() {
	return model;
}
