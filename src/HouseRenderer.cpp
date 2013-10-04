#include "HouseRenderer.h"
#include <cmath>

HouseRenderer::HouseRenderer(Rectangle viewport) : Renderer(viewport) {
    initialize();
}

HouseRenderer::~HouseRenderer() {
    Control* mainMenuButton = houseRendererForm->getControl("mainMenuButton");
    mainMenuButton->removeListener(this);
    Control* refreshButton = houseRendererForm->getControl("refreshButton");
    refreshButton->removeListener(this);
    Control* clearButton = houseRendererForm->getControl("clearButton");

    SAFE_DELETE(house);
    SAFE_DELETE(initialTranslate);
    SAFE_DELETE(curTranslate);
    SAFE_DELETE(prevFloor);
    SAFE_DELETE(prevHover);
    SAFE_RELEASE(houseRendererForm);
    SAFE_RELEASE(scene);
}

void HouseRenderer::initialize() {
    scene = Scene::create("HouseScene");
    zoomLevel = 1;
    curTranslate = new Vector3();

    Camera* camera = Camera::createOrthographic(0, 0, 0, 0.25, 600);

    Node* cameraNode = scene->addNode();
    cameraNode->setCamera(camera);
    scene->setActiveCamera(camera);

    cameraNode->rotateZ(3.14f / 4);
    cameraNode->rotateX(3.14f / 4);


    houseRendererForm = Form::create("res/menu/houseRenderer.form");
    houseRendererForm->setState(Control::State::NORMAL);
    Control* mainMenuButton = houseRendererForm->getControl("mainMenuButton");
    mainMenuButton->addListener(this, Control::Listener::CLICK);
    Control* refreshButton = houseRendererForm->getControl("refreshButton");
    refreshButton->addListener(this, Control::Listener::CLICK);
    Control* clearButton = houseRendererForm->getControl("clearButton");
    clearButton->addListener(this, Control::Listener::CLICK);
    resize();

    createHouse(false);
    createRoom();
}

void HouseRenderer::resize() {
    this->aspectRatio = viewport.width / viewport.height;
    menuWidth = 300;
    renderViewPort.width = viewport.width - menuWidth;
    renderViewPort.height = viewport.height;

    setCamera();

    createMenu(menuWidth);
    prevFloor = NULL;
}

void HouseRenderer::setCamera() {
    float maxPixels = min((renderViewPort.width / (float)sqrt(2)), renderViewPort.height);
    renderHeight = 110 * viewport.height / maxPixels * zoomLevel;

    Camera* camera = scene->getActiveCamera();
    Node* cameraNode = camera->getNode();
    cameraNode->setTranslation(Vector3::zero());

    camera->setAspectRatio(aspectRatio);

    camera->setZoomX(aspectRatio * renderHeight);
    camera->setZoomY(renderHeight);

    cameraNode->translateLeft(-renderHeight * ((viewport.width - renderViewPort.width) / 2 - renderViewPort.x) / (viewport.height));
    cameraNode->translateUp(-renderHeight * ((viewport.height - renderViewPort.height) / 2 - renderViewPort.y) / (viewport.height));
    cameraNode->translateForward(-100);

    initialTranslate = new Vector3(cameraNode->getTranslation());

    cameraNode->translate(*curTranslate);
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
    } else {
        house = new House(5, 5);
    }
    float screenSize = 100;

    house->addFloor(scene, screenSize);
}

void HouseRenderer::createRoom() {
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
    } else if (evt == Mouse::MOUSE_MOVE) {
        checkHover(x, y);
    } else if (evt == Mouse::MOUSE_WHEEL) {
        // Scrolled the mouse wheel
        zoomLevel *= (1 - (float)wheelData * ZOOM_SPEED);
        setCamera();
		// reset hover when zooming, as the mouse moves relative to the screen
		checkHover(x, y);
    } else {
        // Clicked any button on the mouse
        if (evt == Mouse::MouseEvent::MOUSE_RELEASE_LEFT_BUTTON) {
            houseRendererForm->setState(Control::State::NORMAL);
        }

        if (evt == Mouse::MOUSE_PRESS_LEFT_BUTTON && prevHover != NULL) {
            prevHover->setHover(false);
            prevHover = NULL;
        } else if (evt == Mouse::MOUSE_RELEASE_LEFT_BUTTON) {
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

void HouseRenderer::checkHover(int x, int y) {
    // Moved mouse withoud dragging
    int id = getViewTileId(x, y);
    if (prevHover != NULL && id != prevHover->getId()) {
        prevHover->setHover(false);
        prevHover = NULL;
    }
    if (id != -1) {
        prevHover = house->getFloorTile(id)->setHover(true);
    }
}

void HouseRenderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {
    if (evt == Touch::TouchEvent::TOUCH_RELEASE) {
        // reset last floor on release
        prevFloor = NULL;
        return;
    }

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
}

int HouseRenderer::getViewTileId(int x, int y) {
    Camera* camera = scene->getActiveCamera();

    Vector2* rotated = new Vector2();
    rotated->x = (-curTranslate->x - curTranslate->y) / sqrt(2);
    rotated->y = (-curTranslate->x + curTranslate->y);

    x -= renderViewPort.x;
    y -= renderViewPort.y;
    if (x < 0 || x > renderViewPort.width || y < 0 || y > renderViewPort.height) {
        return -1;
    }

    Vector2* destination = new Vector2();

    destination->x = (x - renderViewPort.width / 2) / viewport.height * renderHeight - rotated->x;
    destination->y = (y - renderViewPort.height / 2) / viewport.height * renderHeight - rotated->y;

    destination->x = destination->x / sqrt(2);

    SAFE_DELETE(rotated);
    rotated = new Vector2();
    rotated->x = (destination->x + destination->y);
    rotated->y = (destination->x - destination->y);

    SAFE_DELETE(destination);
    destination = rotated;

    int maxSize = max(house->getWidth(), house->getHeight());
    int floorX = (int)(destination->x / 100 * maxSize + (float)house->getWidth() / 2);
    int floorY = (int)(destination->y / 100 * maxSize + (float)house->getHeight() / 2);

    SAFE_DELETE(rotated);
    return house->getIdByXY(floorX, floorY);
}

void HouseRenderer::keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags) {
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
        }
    }
}
void HouseRenderer::resizeEvent(unsigned int width, unsigned int height) {
    Renderer::resizeEvent(width, height);
    resize();
}

Renderers HouseRenderer::update(float elapsedTime) {
    float translation = SCROLL_SPEED * elapsedTime * zoomLevel;

    houseRendererForm->update(elapsedTime);
    bool changed = false;
    if (getKeyFlags()->getFlag('w') || getKeyFlags()->getFlag(Keyboard::Key::KEY_UP_ARROW)) {
        scene->getActiveCamera()->getNode()->translateUp(translation);
        changed = true;
    }
    if (getKeyFlags()->getFlag('a') || getKeyFlags()->getFlag(Keyboard::Key::KEY_LEFT_ARROW)) {
        scene->getActiveCamera()->getNode()->translateLeft(translation);
        changed = true;
    }
    if (getKeyFlags()->getFlag('s') || getKeyFlags()->getFlag(Keyboard::Key::KEY_DOWN_ARROW)) {
        scene->getActiveCamera()->getNode()->translateUp(-translation);
        changed = true;
    }
    if (getKeyFlags()->getFlag('d') || getKeyFlags()->getFlag(Keyboard::Key::KEY_RIGHT_ARROW)) {
        scene->getActiveCamera()->getNode()->translateLeft(-translation);
        changed = true;
    }
    if (changed) {
        Vector3::subtract(scene->getActiveCamera()->getNode()->getTranslation(), *initialTranslate, curTranslate);
    }
    return nextRenderer;
}

void HouseRenderer::render(float elapsedTime) {
    // Draw the scene
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
    houseRendererForm->draw();
}

void HouseRenderer::controlEvent(Control* control, Control::Listener::EventType evt) {
    if (!strcmp("mainMenuButton", control->getId())) {
        nextRenderer = MAIN_MENU;
    } else if (!strcmp("refreshButton", control->getId())) {
        createHouse(true);
        createRoom();
    } else if (!strcmp("clearButton", control->getId())) {
        createHouse(true);
    }
    control->setState(Control::State::NORMAL);
}