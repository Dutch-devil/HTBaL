#include "HouseRenderer.h"
#include <cmath>

HouseRenderer::HouseRenderer(Scene* scene, Rectangle viewport, float aspectRatio) {
	this->scene = scene;
	this->viewport = viewport;
	this->aspectRatio = aspectRatio;
    initialize();
}

HouseRenderer::~HouseRenderer() {
}

void HouseRenderer::initialize() {
    house = new House(6, 6);

    // Create our render state block that will be reused
    // across all materials
    stateBlock = RenderState::StateBlock::create();
    stateBlock->setCullFace(true);
    stateBlock->setBlend(true);
    stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

    //Camera* camera = Camera::createPerspective(45, aspectRatio, 0.25, 100);
	float screenHeight = 100;
	Camera* camera = Camera::createOrthographic(aspectRatio * screenHeight, screenHeight, aspectRatio, 0.25, 600);
    Node* cameraNode = scene->addNode();

    cameraNode->setCamera(camera);
	scene->setActiveCamera(camera);

    createHouse();
	createRoom();
	cameraNode->rotateZ(-3.14 / 4);
	cameraNode->rotateX(3.14 / 4);
	cameraNode->translateForward(-300);
}

void HouseRenderer::createHouse() {
	//Vector3* destination = new Vector3();
	//scene->getActiveCamera()->unproject(viewport, 0, 0, 1, destination);

    //float screenWidth = -2 * destination->x;
    //float screenHeight = 2 * destination->y;
	float screenSize = 100;
	//float screenSize = (screenWidth > screenHeight)?screenHeight:screenWidth;

    tileWidth = screenSize / house->getWidth();
    tileHeight = screenSize / house->getHeight();

    for (int x = 0; x < house->getWidth(); x++) {
        for (int y = 0; y < house->getHeight(); y++) {
            Mesh* tileMesh = Mesh::createQuad(Vector3(-tileWidth / 2, -tileHeight / 2, 0),
                                               Vector3(tileWidth / 2, -tileHeight / 2, 0),
                                               Vector3(-tileWidth / 2, tileHeight / 2, 0),
                                               Vector3(tileWidth / 2, tileHeight / 2, 0));

            Model* tileModel = Model::create(tileMesh);

            // Create the ground material
			Material* tileMaterial = tileModel->setMaterial("res/grid.material");
            tileMaterial->setStateBlock(stateBlock);

			Node* tileNode = scene->addNode();
			tileNode->translateX((x - (float)house->getWidth() / 2) * tileWidth + tileWidth / 2);
			tileNode->translateY((y - (float)house->getHeight() / 2) * tileHeight + tileHeight / 2);
			tileNode->setModel(tileModel);
			tileModels.push_back(tileModel);
        }
    };
}

void HouseRenderer::createRoom() {
	float startX = -(float)house->getWidth() / 2 * tileWidth;
	float startY = -(float)house->getHeight() / 2 * tileHeight;
	list<Vector2*> walls;

	for (int y = 0; y < house->getHeight() + 1 ; y++) {
		walls.push_back(new Vector2(startX + tileWidth, startY + tileHeight * y));
	}
	house->addRoom(Room(0, 0, walls));

	walls = list<Vector2*>();

	for (int x = 0; x < house->getWidth() + 1 ; x++) {
		walls.push_back(new Vector2(startX + tileWidth * x, -startY - tileHeight));
	}
	house->addRoom(Room(0, 0, walls));

	
	for (Room room : house->getRooms()) {
		Vector2* prevWall = NULL;
		for (Vector2* wall : room.getWalls()) {
			print("%f, %f\n", wall->x, wall->y);
			if (prevWall == NULL) {
				prevWall = wall;
				continue;
			}
			Mesh* wallMesh = Mesh::createQuad(Vector3(prevWall->x, prevWall->y, 0),
                                               Vector3(wall->x, wall->y, 0),
                                               Vector3(prevWall->x, prevWall->y, 10),
                                               Vector3(wall->x, wall->y, 10));

            Model* wallModel = Model::create(wallMesh);

            // Create the ground material
			Material* wallMaterial = wallModel->setMaterial("res/grid.material");
            wallMaterial->setStateBlock(stateBlock);

			Node* wallNode = scene->addNode();
			wallNode->translateZ(-scene->getActiveCamera()->getFarPlane() + .1);
			wallNode->setModel(wallModel);
			wallModels.push_back(wallModel);

			prevWall = wall;
		}
	}
}

void HouseRenderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {
	Vector2* destination = new Vector2();
	destination->x = (float)x / viewport.width * 100 / sqrt(2) * aspectRatio - 50 / sqrt(2) * aspectRatio;
	destination->y = (float)y / viewport.height * 100 - 50;

	print("%f, %f, %f\n", destination->x, destination->y);

	Vector2* rotated = new Vector2();
	rotated->x = (destination->x - destination->y);
	rotated->y = -(destination->x + destination->y);

	print("%f, %f, %f\n", rotated->x, rotated->y);

	destination = rotated;

	int i = 0;
    for (Model* tileModel : tileModels) {
        Node* tileNode = tileModel->getNode();
		if (tileNode->getTranslationX() - tileWidth / 2 < destination->x && tileNode->getTranslationX() + tileWidth / 2 > destination->x &&
				tileNode->getTranslationY() - tileHeight / 2 < destination->y && tileNode->getTranslationY() + tileHeight / 2 > destination->y) {
			// clicked this model
			tileModel->getMaterial()->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(Vector3(1, 0, 0));
			break;
		}
		i++;
    }
}

Renderers HouseRenderer::update(float elapsedTime) {
    return KEEP;
}

void HouseRenderer::render(float elapsedTime) {
    // Draw the scene
    for (Model* tileModel : tileModels) {
        tileModel->draw();
    }
    for (Model* wallModel : wallModels) {
        wallModel->draw();
    }
}