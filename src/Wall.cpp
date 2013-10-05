#include "Wall.h"
#include "MaterialManager.h"


Mesh* Wall::mesh = NULL;

Wall::Wall(Node* node, bool door): door(door), model(NULL), node(node) {
	node->addRef();
}

Wall::~Wall() {
	SAFE_RELEASE(node);
	print("wallmodelrefcount: %d\n", model->getRefCount());
	//SAFE_RELEASE(model);
}

void Wall::calculateMesh() {
    SAFE_RELEASE(mesh);
	mesh = Mesh::createQuad(Vector3(Floor::getWidth() / 2, -Floor::getHeight() / 2, (Floor::getWidth() + Floor::getHeight()) / 4),
						Vector3(Floor::getWidth() / 2, -Floor::getHeight() / 2, 0),
						Vector3(Floor::getWidth() / 2, Floor::getHeight() / 2, (Floor::getWidth() + Floor::getHeight()) / 4),
						Vector3(Floor::getWidth() / 2, Floor::getHeight() / 2, 0));
}

Node* Wall::getNode(){
	return node;
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
