#include "HouseRenderer.h"
#include <cmath>

//#define PERSPECTIVE

HouseRenderer::HouseRenderer(Rectangle viewport) : Renderer(viewport) {
    initialize();
}

HouseRenderer::~HouseRenderer() {
	Control* mainMenuButton = houseRendererForm->getControl("mainMenuButton");
	mainMenuButton->removeListener(this);
	Control* refreshButton = houseRendererForm->getControl("refreshButton");
	refreshButton->removeListener(this);
	Control* clearButton = houseRendererForm->getControl("clearButton");
	clearButton->removeListener(this);
	SAFE_RELEASE(houseRendererForm);
	SAFE_RELEASE(scene);
}

void HouseRenderer::initialize() {
	scene = Scene::create("HouseScene");
	
#ifdef PERSPECTIVE
	Camera* camera = Camera::createPerspective(0, 0, 0, 0);
#else
    Camera* camera = Camera::createOrthographic(0, 0, 0, 0, 0);
#endif
	Node* cameraNode = scene->addNode();
	cameraNode->setCamera(camera);
    scene->setActiveCamera(camera);
#ifndef PERSPECTIVE
    cameraNode->rotateZ(3.14f / 4);
    cameraNode->rotateX(3.14f / 4);
#endif

	resize();

    createHouse(false);
    createRoom();
}

void HouseRenderer::resize() {
    this->aspectRatio = viewport.width / viewport.height;
	menuWidth = 300;
	renderViewPort.width = viewport.width - menuWidth;
	renderViewPort.height = viewport.height;
	
	float maxPixels = min((renderViewPort.width / (float)sqrt(2)), renderViewPort.height);
	renderHeight = 110 * viewport.height / maxPixels;
	Camera* camera = scene->getActiveCamera();
	Node* cameraNode = camera->getNode();
	cameraNode->setTranslation(Vector3::zero());

	camera->setAspectRatio(aspectRatio);
#ifdef PERSPECTIVE
	camera->setFieldOfView(45);
	camera->setNearPlane(0.25);
	camera->setFarPlane(100.1);
#else
	camera->setZoomX(aspectRatio * renderHeight);
	camera->setZoomY(renderHeight);
	camera->setNearPlane(0.25);
	camera->setFarPlane(600);

#endif
	cameraNode->translateLeft(-renderHeight * ((viewport.width - renderViewPort.width) / 2 - renderViewPort.x) / (viewport.height));
	cameraNode->translateUp(-renderHeight * ((viewport.height - renderViewPort.height) / 2 - renderViewPort.y) / (viewport.height));

    cameraNode->translateForward(-100);
	
	houseRendererForm = Form::create("res/menu/houseRenderer.form");
	Control* mainMenuButton = houseRendererForm->getControl("mainMenuButton");
	mainMenuButton->addListener(this, Control::Listener::CLICK);
	Control* refreshButton = houseRendererForm->getControl("refreshButton");
	refreshButton->addListener(this, Control::Listener::CLICK);
	Control* clearButton = houseRendererForm->getControl("clearButton");
	clearButton->addListener(this, Control::Listener::CLICK);
	createMenu(menuWidth);
}

void HouseRenderer::createMenu(float menuWidth) {
	nextRenderer = KEEP;

	Control* mainMenuButton = houseRendererForm->getControl("mainMenuButton");
	mainMenuButton->setWidth(menuWidth);
	Control* refreshButton = houseRendererForm->getControl("refreshButton");
	refreshButton->setWidth(menuWidth);
	Control* clearButton = houseRendererForm->getControl("clearButton");
	clearButton->setWidth(menuWidth);
}

void HouseRenderer::createHouse(bool random) {
	prevHover = false;
	if (random) {
		house = new House(rand() % 11 + 5, rand() % 11 + 5);
	}else {
		house = new House(5, 5);
	}
#ifdef PERSPECTIVE
    Vector3* destination = new Vector3();
    scene->getActiveCamera()->unproject(viewport, 0, 0, 1, destination);

    float screenWidth = -2 * destination->x;
    float screenHeight = 2 * destination->y;
    float screenSize = (screenWidth > screenHeight)?screenHeight:screenWidth;
#else
    float screenSize = 100;
#endif

	house->addFloor(scene, screenSize);
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

	house->addRandomRooms(scene);
}
	
bool HouseRenderer::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging) {
	if (dragging) {
		if (leftButtonDown()) {
			// Dragging left button
		}
		if (rightButtonDown()) {
			// Dragging right button
		}
		if (middleButtonDown()) {
			// Dragging middle button
		}
		// Dragging should be handled by the touchEvent
		return false;
	}else if (evt == Mouse::MOUSE_MOVE) {
		// Moved mouse withoud dragging
		int id = getViewTileId(x, y);
		if (prevHover != NULL && id != prevHover->getId()) {
			prevHover->setHover(false);
			prevHover = NULL;
		}
		if (id != -1) {
			prevHover = house->getFloorTile(id)->setHover(true);
		}
	}else if (evt == Mouse::MOUSE_WHEEL) {
		// Scrolled the mouse wheel
		Camera* camera = scene->getActiveCamera();
#ifdef PERSPECTIVE
#else
		camera->setZoomX(camera->getZoomX() * (1 - (float)wheelData / 10));
		camera->setZoomY(camera->getZoomY() * (1 - (float)wheelData / 10));
#endif
	}else {
		// Clicked any button on the mouse
		if (evt == Mouse::MOUSE_PRESS_LEFT_BUTTON && prevHover != NULL) {
			prevHover->setHover(false);
			prevHover = NULL;
		}else if (evt == Mouse::MOUSE_RELEASE_LEFT_BUTTON) {
			int id = getViewTileId(x, y);
			if (id != -1) {
				prevHover = house->getFloorTile(id)->setHover(true);
			}
		}
		// Mouse click should be handled by touchEvent
		return false;
	}
	return true;
}

void HouseRenderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {
	int id = getViewTileId(x, y);
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

int HouseRenderer::getViewTileId(int x, int y) {
	print("%f, %f \n", scene->getActiveCamera()->getNode()->getTranslationWorld().x, scene->getActiveCamera()->getNode()->getTranslationWorld().y);

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
    int floorX = (int)(destination->x / 100 * maxSize + (float)house->getWidth() / 2);
    int floorY = (int)(destination->y / 100 * maxSize + (float)house->getHeight() / 2);

	return house->getIdByXY(floorX, floorY);
}

void HouseRenderer::keyEvent(Keyboard::KeyEvent evt, int key) {
	if (evt == Keyboard::KeyEvent::KEY_RELEASE) {
		if (key == 'r') {
			list<Floor*> roomTiles = list<Floor*>();
		
			for (int i = 0; i < house->getWidth() * house->getHeight(); i++) {
				if (house->getFloorTile(i)->getSelected()) {
					house->getFloorTile(i)->toggleSelect();
					roomTiles.push_back(house->getFloorTile(i));
				}
			}
			house->addRoom(Room::createRoomFromFloor(scene, house, roomTiles));
		}else if (key == 'w') {
			scene->getActiveCamera()->getNode()->translateUp(10);
		}else if (key == 'a') {
			scene->getActiveCamera()->getNode()->translateLeft(10);
		}else if (key == 's') {
			scene->getActiveCamera()->getNode()->translateUp(-10);
		}else if (key == 'd') {
			scene->getActiveCamera()->getNode()->translateLeft(-10);
		}
	}
}
void HouseRenderer::resizeEvent(unsigned int width, unsigned int height) {
	Renderer::resizeEvent(width, height);
	resize();
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
		createHouse(true);
		createRoom();
	}else if (!strcmp("clearButton", control->getId())) {
		createHouse(true);
	}
}