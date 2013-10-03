#include "Wall.h"
#include "MaterialManager.h"

Wall::Wall(Vector2* start, Vector2* end): start(start), end(end) {
	model = NULL;
}
Wall::Wall(Vector2* start, Vector2* end, bool door): start(start), end(end), door(door) {
	model = NULL;
}

Wall::~Wall() {}

Vector2* Wall::getStart() {
    return start;
}

Vector2* Wall::getEnd() {
    return end;
}

Mesh* Wall::getMesh() {
    return Mesh::createQuad(Vector3(start->x, start->y, (Floor::width + Floor::height) / 4),
                            Vector3(start->x, start->y, 0),
                            Vector3(end->x, end->y, (Floor::width + Floor::height) / 4),
                            Vector3(end->x, end->y, 0));
}

Model* Wall::getModel() {
    if (model == NULL) {
        model = Model::create(getMesh());

        // Create the ground material
        model->setMaterial(MaterialManager::getMaterial(door?DOOR:WALL));
    }
    return model;
}
