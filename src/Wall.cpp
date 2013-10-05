#include "Wall.h"
#include "MaterialManager.h"


Mesh* Wall::mesh = NULL;

Wall::Wall(): door(false) {
	model = NULL;
}
Wall::Wall(bool door): door(door) {
	model = NULL;
}

Wall::~Wall() {}

void Wall::calculateMesh() {
    SAFE_RELEASE(mesh);
	mesh = Mesh::createQuad(Vector3(Floor::getWidth() / 2, -Floor::getHeight() / 2, (Floor::getWidth() + Floor::getHeight()) / 4),
						Vector3(Floor::getWidth() / 2, -Floor::getHeight() / 2, 0),
						Vector3(Floor::getWidth() / 2, Floor::getHeight() / 2, (Floor::getWidth() + Floor::getHeight()) / 4),
						Vector3(Floor::getWidth() / 2, Floor::getHeight() / 2, 0));
}

Mesh* Wall::getMesh() {
	if (mesh == NULL) {
		calculateMesh();
	}
	return mesh;
}

Model* Wall::getModel() {
    if (model == NULL) {
		Mesh* mesh = getMesh();
        model = Model::create(mesh);
		SAFE_RELEASE(mesh);

        // Create the ground material
		Material* material = MaterialManager::getMaterial(door?DOOR:WALL);
        model->setMaterial(material);
		SAFE_RELEASE(material);
    }
    return model;
}
