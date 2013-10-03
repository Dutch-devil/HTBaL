#include "HouseRenderer.h"
#include <cmath>

//#define PERSPECTIVE

HouseRenderer::HouseRenderer(Rectangle viewport) : Renderer(viewport) {
    this->aspectRatio = viewport.width / viewport.height;
	menuWidth = 300;
	renderViewPort.width = viewport.width - menuWidth;
	renderViewPort.height = viewport.height;
    initialize();
}

HouseRenderer::~HouseRenderer() {
	Control* mainMenuButton = houseRendererForm->getControl("mainMenuButton");
	mainMenuButton->removeListener(this);
	SAFE_RELEASE(houseRendererForm);
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
	renderHeight = 110 * viewport.height / maxPixels;
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
	
	Control* refreshButton = houseRendererForm->getControl("refreshButton");
	refreshButton->addListener(this, Control::Listener::CLICK);
	refreshButton->setWidth(menuWidth);
}

void HouseRenderer::createHouse(float renderHeight) {
    house = new House(rand() % 11 + 5, rand() % 11 + 5);
#ifdef PERSPECTIVE
    Vector3* destination = new Vector3();
    scene->getActiveCamera()->unproject(viewport, 0, 0, 1, destination);

    float screenWidth = -2 * destination->x;
    float screenHeight = 2 * destination->y;
    float screenSize = (screenWidth > screenHeight)?screenHeight:screenWidth;
#else
    float screenSize = 100;
#endif

	house->addFloor(scene, stateBlock, screenSize);
}

void HouseRenderer::createRoom() {
	/*Floor** roomTiles = new Floor*[house->getWidth()*house->getHeight()];
	for(int i = 0; i < house->getWidth()*house->getHeight(); i++) {
		switch(i) {
		case 0:
		case 1:
		case 5:
		case 6:
		case 7:
			roomTiles[i] = house->getFloorTile(i);
			break;
		default:
			roomTiles[i] = NULL;
		}
	}*/

	//house->addRoom(Room::createRoomFromFloor(scene, house, stateBlock, roomTiles, house->getWidth()*house->getHeight()));

	house->addRandomRooms(scene, stateBlock);
}

void HouseRenderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {
	x -= renderViewPort.x;
	y -= renderViewPort.y;
#ifdef PERSPECTIVE
    Vector3* destination = new Vector3();
    scene->getActiveCamera()->unproject(viewport, x, y, 1, destination);
#else
    Vector2* destination = new Vector2();
	destination->x = (x - renderViewPort.width/2) / viewport.height * renderHeight;
	destination->y = (y - renderViewPort.height/2) / viewport.height * renderHeight;

	destination->x = destination->x / sqrt(2);

    Vector2* rotated = new Vector2();
    rotated->x = (destination->x + destination->y);
    rotated->y = (destination->x - destination->y);

    destination = rotated;
#endif

	int maxSize = max(house->getWidth(), house->getHeight());
    int floorX = (int)(destination->x / 100 * maxSize + house->getWidth() / 2);
    int floorY = (int)(destination->y / 100 * maxSize + house->getHeight() / 2);

	int id = house->getIdByXY(floorX, floorY);
	if (id == -1) {
		prevFloor = NULL;
		return;
	}
	Floor* floor = house->getFloorTile(id);

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
		list<Floor*> roomTiles = list<Floor*>();
		
		for (int i = 0; i < house->getWidth() * house->getHeight(); i++) {
			if (house->getFloorTile(i)->getSelected()) {
				house->getFloorTile(i)->toggleSelect();
				roomTiles.push_back(house->getFloorTile(i));
			}
		}
		house->addRoom(Room::createRoomFromFloor(scene, house, stateBlock, roomTiles));
	}
}

Renderers HouseRenderer::update(float elapsedTime) {
	houseRendererForm->update(elapsedTime);
    return nextRenderer;
}

void HouseRenderer::render(float elapsedTime) {
    // Draw the scene
	houseRendererForm->draw();
	Floor** curFloor = house->getFloorTiles();
	while (curFloor - house->getFloorTiles() < house->getWidth() * house->getHeight()) {
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
	}else if (!strcmp("refreshButton", control->getId())) {
		createHouse(0);
		createRoom();
	}
}