#include "HouseRenderer.h"
#include <cmath>

//#define PERSPECTIVE

HouseRenderer::HouseRenderer(Scene* scene, Rectangle viewport, float aspectRatio) {
    this->scene = scene;
    this->viewport = viewport;
    this->aspectRatio = aspectRatio;
    initialize();
}

HouseRenderer::~HouseRenderer() {
}

void HouseRenderer::initialize() {
    house = new House(5, 5);
    floorTiles = new Floor*[house->getWidth() * house->getHeight()];

    // Create our render state block that will be reused
    // across all materials
    stateBlock = RenderState::StateBlock::create();
    stateBlock->setCullFace(false);
    stateBlock->setBlend(true);
    stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

#ifdef PERSPECTIVE
    Camera* camera = Camera::createPerspective(45, aspectRatio, 0.25, 100.1);
    Node* cameraNode = scene->addNode();
#else
    float screenHeight = 100;
    Camera* camera = Camera::createOrthographic(aspectRatio * screenHeight, screenHeight, aspectRatio, 0.25, 600);
    Node* cameraNode = scene->addNode();
    cameraNode->rotateZ(3.14f / 4);
    cameraNode->rotateX(3.14f / 4);
#endif

    cameraNode->setCamera(camera);
    scene->setActiveCamera(camera);

    cameraNode->translateForward(-100);
    createHouse();
    createRoom();
}

void HouseRenderer::createHouse() {
#ifdef PERSPECTIVE
    Vector3* destination = new Vector3();
    scene->getActiveCamera()->unproject(viewport, 0, 0, 1, destination);

    float screenWidth = -2 * destination->x;
    float screenHeight = 2 * destination->y;
    float screenSize = (screenWidth > screenHeight)?screenHeight:screenWidth;
#else
    float screenSize = 100;
#endif

    Floor::width = screenSize / house->getWidth();
    Floor::height = screenSize / house->getHeight();

    for (int x = 0; x < house->getWidth(); x++) {
        for (int y = 0; y < house->getHeight(); y++) {
            // Make a new floor tile
            Floor* floor = new Floor(stateBlock);

            Node* tileNode = scene->addNode();
            tileNode->translateX((x - (float)house->getWidth() / 2) * Floor::width + Floor::width / 2);
            tileNode->translateY((y - (float)house->getHeight() / 2) * Floor::height + Floor::height / 2);
            tileNode->setModel(floor->getModel());
            floorTiles[x + y * house->getWidth()] = floor;
        }
    };
}

void HouseRenderer::createRoom() {
    float startX = -(float)house->getWidth() / 2 * Floor::width;
    float startY = -(float)house->getHeight() / 2 * Floor::height;
    list<Vector2*> walls;

    for (int y = house->getHeight(); y >= 0 ; y--) {
        walls.push_back(new Vector2(startX + Floor::width, startY + Floor::height * y));
    }
    house->addRoom(Room(0, 0, walls));

    walls = list<Vector2*>();

    for (int x = 0; x < house->getWidth() + 1 ; x++) {
        walls.push_back(new Vector2(startX + Floor::width * x, -startY - Floor::height));
    }
    house->addRoom(Room(0, 0, walls));

    for (Room room : house->getRooms()) {
        Vector2* prevWall = NULL;
        for (Vector2* wall : room.getWalls()) {
            if (prevWall == NULL) {
                prevWall = wall;
                continue;
            }
            Mesh* wallMesh = Mesh::createQuad(Vector3(prevWall->x, prevWall->y, 0),
                                              Vector3(wall->x, wall->y, 0),
                                              Vector3(prevWall->x, prevWall->y, (Floor::width + Floor::height) / 2),
                                              Vector3(wall->x, wall->y, (Floor::width + Floor::height) / 2));

            Model* wallModel = Model::create(wallMesh);

            // Create the ground material
            Material* wallMaterial = wallModel->setMaterial("res/grid.material");
            wallMaterial->setStateBlock(stateBlock);

            Node* wallNode = scene->addNode();
            wallNode->setModel(wallModel);
            wallModels.push_back(wallModel);

            prevWall = wall;
        }
    }
}

void HouseRenderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {
    if (evt == Touch::TouchEvent::TOUCH_PRESS) {
        return;
    }
#ifdef PERSPECTIVE
    Vector3* destination = new Vector3();
    scene->getActiveCamera()->unproject(viewport, x, y, 1, destination);
#else
    Vector2* destination = new Vector2();
    destination->x = (float)x / viewport.width * 100 / sqrt(2) * aspectRatio - 50 / sqrt(2) * aspectRatio;
    destination->y = (float)y / viewport.height * 100 - 50;

    Vector2* rotated = new Vector2();
    rotated->x = (destination->x + destination->y);
    rotated->y = (destination->x - destination->y);

    destination = rotated;
#endif
	if (destination->x < -50 || destination->x > 50 || destination->y < -50 || destination->y > 50) {
		prevFloor = NULL;
		return;
	}

    destination->x += 50;
    destination->y += 50;

    int floorX = (int)(destination->x / 100 * house->getWidth());
    int floorY = (int)(destination->y / 100 * house->getHeight());
    Floor* floor = floorTiles[floorX + floorY * house->getWidth()];

    if (floor != prevFloor) {
        // same model twice, don't toggle
        floor->toggleSelect();
        prevFloor = floor;
    }
    if (evt == Touch::TouchEvent::TOUCH_RELEASE) {
        // reset last floor on release
        prevFloor = NULL;
    }
}

Renderers HouseRenderer::update(float elapsedTime) {
    return KEEP;
}

void HouseRenderer::render(float elapsedTime) {
    // Draw the scene
	Floor** curFloor = floorTiles;
	while (curFloor - floorTiles < house->getWidth() * house->getHeight()) {
		(*curFloor)->getModel()->draw();
		curFloor++;
    }
    for (Model* wallModel : wallModels) {
        wallModel->draw(false);
    }
}