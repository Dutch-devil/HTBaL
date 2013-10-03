#include "HouseRenderer.h"
#include <cmath>

//#define PERSPECTIVE

HouseRenderer::HouseRenderer(Rectangle viewport) : Renderer(viewport) {
    this->aspectRatio = viewport.width / viewport.height;
	menuWidth = 100;
	renderViewPort.width = viewport.width - menuWidth;
	renderViewPort.height = viewport.height - 100;
	renderViewPort.y = 100;
    initialize();
}

HouseRenderer::~HouseRenderer() {
	SAFE_RELEASE(scene);
}

void HouseRenderer::initialize() {
	scene = Scene::create("HouseScene");

    // Create our render state block that will be reused
    // across all materials
    stateBlock = RenderState::StateBlock::create();
    stateBlock->setCullFace(false);
    stateBlock->setBlend(true);
    stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
	
	float maxPixels = min((renderViewPort.width / (float)sqrt(2)), renderViewPort.height);
	renderHeight = 100 * viewport.height / maxPixels;
#ifdef PERSPECTIVE
    Camera* camera = Camera::createPerspective(45, aspectRatio, 0.25, 100.1);
    Node* cameraNode = scene->addNode();
#else
    Camera* camera = Camera::createOrthographic(aspectRatio * renderHeight, renderHeight, aspectRatio, 0.25, 600);
    Node* cameraNode = scene->addNode();
    cameraNode->rotateZ(3.14f / 4);
    cameraNode->rotateX(3.14f / 4);
#endif
	cameraNode->translateLeft(-renderHeight * ((viewport.width - renderViewPort.width) / 2 - renderViewPort.x) / (viewport.height));
	cameraNode->translateUp(-renderHeight * ((viewport.height - renderViewPort.height) / 2 - renderViewPort.y) / (viewport.height));
	 
    cameraNode->setCamera(camera);
    scene->setActiveCamera(camera);

    cameraNode->translateForward(-100);

	createMenu(menuWidth);
    createHouse(renderHeight);
    createRoom();
}

void HouseRenderer::createMenu(float menuWidth) {
	nextRenderer = KEEP;
	houseRendererForm = Form::create("res/menu/houseRenderer.form");
	Control* mainMenuButton = houseRendererForm->getControl("mainMenuButton");
	mainMenuButton->addListener(this, Control::Listener::CLICK);
	mainMenuButton->setWidth(menuWidth);
}

void HouseRenderer::createHouse(float renderHeight) {
    house = new House(7, 7);
    floorTiles = new Floor*[house->getWidth() * house->getHeight()];
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
            Floor* floor = new Floor(stateBlock, (x - (float)house->getWidth() / 2) * Floor::width + Floor::width / 2, (y - (float)house->getHeight() / 2) * Floor::height + Floor::height / 2);

            Node* tileNode = scene->addNode();
            tileNode->translateX(floor->getX());
            tileNode->translateY(floor->getY());
            tileNode->setModel(floor->getModel());
            floorTiles[x + y * house->getWidth()] = floor;
        }
    };
}

void HouseRenderer::createRoom() {
	Floor** roomTiles = new Floor*[house->getWidth()*house->getHeight()];
	for(int i = 0; i < house->getWidth()*house->getHeight(); i++) {
		switch(i) {
		case 0:
		case 1:
		case 5:
		case 6:
		case 7:
			roomTiles[i] = floorTiles[i];
			break;
		default:
			roomTiles[i] = NULL;
		}
	}

	house->addRoom(Room::createRoomFromFloor(scene, house, stateBlock, roomTiles, house->getWidth()*house->getHeight()));
}

void HouseRenderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {
	x -= renderViewPort.x;
	y -= renderViewPort.y;
#ifdef PERSPECTIVE
    Vector3* destination = new Vector3();
    scene->getActiveCamera()->unproject(viewport, x, y, 1, destination);
#else
    Vector2* destination = new Vector2();
    destination->x = (float)x / renderViewPort.width * renderHeight / sqrt(2) * aspectRatio - renderHeight / 2 / sqrt(2) * aspectRatio;
    destination->y = (float)y / renderViewPort.height * renderHeight - renderHeight / 2;

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

void HouseRenderer::keyEvent(Keyboard::KeyEvent evt, int key) {
	if (key == 'r') {
		Floor** roomTiles = new Floor*[house->getWidth()*house->getHeight()];
		
		for (int i = 0; i < house->getWidth() * house->getHeight(); i++) {
			if (floorTiles[i]->getSelected()) {
				floorTiles[i]->toggleSelect();
				roomTiles[i] = floorTiles[i];
			} else {
				roomTiles[i] = NULL;
			}
		}
		house->addRoom(Room::createRoomFromFloor(scene, house, stateBlock, roomTiles, house->getWidth()*house->getHeight()));
	}
}

Renderers HouseRenderer::update(float elapsedTime) {
	houseRendererForm->update(elapsedTime);
    return nextRenderer;
}

void HouseRenderer::render(float elapsedTime) {
    // Draw the scene
	houseRendererForm->draw();
	Floor** curFloor = floorTiles;
	while (curFloor - floorTiles < house->getWidth() * house->getHeight()) {
		(*curFloor)->getModel()->draw();
		curFloor++;
    }
	for (Room* room : house->getRooms()) {
		for (Wall* wall : room->getWalls()) {
			wall->getModel()->draw();
		}
	}
}

void HouseRenderer::controlEvent(Control* control, Control::Listener::EventType evt) {
	if (!strcmp("mainMenuButton", control->getId())) {
		nextRenderer = MAIN_MENU;
	}
}