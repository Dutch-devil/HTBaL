#include "Wall.h"
#include "MaterialManager.h"


Mesh* Wall::mesh = NULL;

Wall::Wall(bool inverted): inverted(inverted), door(false) {
	model = NULL;
}
Wall::Wall(bool inverted, bool door): inverted(inverted), door(door) {
	model = NULL;
}

Wall::~Wall() {
	SAFE_RELEASE(model);
}

void Wall::releaseMesh() {
	SAFE_RELEASE(mesh);
}

void Wall::calculateMesh() {
    SAFE_RELEASE(mesh);
	mesh = Mesh::createQuad(Vector3(Floor::getWidth() / 2, -Floor::getHeight() / 2, (Floor::getWidth() + Floor::getHeight()) / 4),
						Vector3(Floor::getWidth() / 2, -Floor::getHeight() / 2, 0),
						Vector3(Floor::getWidth() / 2, Floor::getHeight() / 2, (Floor::getWidth() + Floor::getHeight()) / 4),
						Vector3(Floor::getWidth() / 2, Floor::getHeight() / 2, 0));
}

Node* Wall::getNode(){
	return model->getNode();
}

Mesh* Wall::getMesh() {
	if (mesh == NULL) {
		calculateMesh();
	}
	return mesh;
}

void Wall::finalize() {
	SAFE_RELEASE(mesh);
}

Model* Wall::getModel() {
    if (model == NULL) {
        model = Model::create(getMesh());

        // Create the ground material
		Material* material = MaterialManager::getMaterial(door?DOOR:WALL);
        model->setMaterial(material);
		SAFE_RELEASE(material);
    }
    return model;
}
