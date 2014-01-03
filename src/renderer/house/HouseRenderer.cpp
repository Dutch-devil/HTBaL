#include "HouseRenderer.h"

HouseRenderer::HouseRenderer(Rectangle viewport) : Renderer(viewport), initialTranslate(NULL) {
    initialize();
}

HouseRenderer::~HouseRenderer() {
    Control* mainMenuButton = houseRendererForm->getControl("mainMenuButton");
    mainMenuButton->removeListener(this);
    Control* refreshButton = houseRendererForm->getControl("refreshButton");
    refreshButton->removeListener(this);
    Control* clearButton = houseRendererForm->getControl("clearButton");
    clearButton->removeListener(this);
    Control* floorUpButton = houseRendererForm->getControl("floorUpButton");
    floorUpButton->removeListener(this);
    Control* floorDownButton = houseRendererForm->getControl("floorDownButton");
    floorDownButton->removeListener(this);
    
    SAFE_DELETE(initialTranslate);
    SAFE_DELETE(curTranslate);
    SAFE_RELEASE(houseRendererForm);
    
    removeHouse();
    SAFE_RELEASE(scene);
    Floor::finalize();
    Wall::finalize();
    
    SAFE_DELETE(roomTypes);
}

void HouseRenderer::initialize() {
    scene = Scene::create("HouseScene");
    zoomLevel = 1;
    curTranslate = new Vector3();
    
    Camera* camera = Camera::createOrthographic(0, 0, 0, 0.25, 600);
    
    Node* cameraNode = scene->addNode();
    cameraNode->setCamera(camera);
    scene->setActiveCamera(camera);
    
    SAFE_RELEASE(camera);
    
    cameraNode->rotateZ(MATH_PI / 4);
    cameraNode->rotateX(MATH_PI / 4);
    
    houseRendererForm = Form::create("res/menu/houseRenderer.form");
    houseRendererForm->setState(Control::State::NORMAL);
    Control* mainMenuButton = houseRendererForm->getControl("mainMenuButton");
    mainMenuButton->addListener(this, Control::Listener::CLICK);
    Control* refreshButton = houseRendererForm->getControl("refreshButton");
    refreshButton->addListener(this, Control::Listener::CLICK);
    Control* clearButton = houseRendererForm->getControl("clearButton");
    clearButton->addListener(this, Control::Listener::CLICK);
    Control* floorUpButton = houseRendererForm->getControl("floorUpButton");
    floorUpButton->addListener(this, Control::Listener::CLICK);
    Control* floorDownButton = houseRendererForm->getControl("floorDownButton");
    floorDownButton->addListener(this, Control::Listener::CLICK);
    
    resize();
    
    MenuWheelPart::setSize(renderHeight);
    roomTypes = HouseMenuWheel::create(scene, viewport);
    roomTypes->addListener(this);
    Node* roomTypesNode = roomTypes->getNode();
    roomTypesNode->translateX(aspectRatio * renderHeight / 2);
    roomTypesNode->translateY(-renderHeight / 2);
    roomTypesNode->translateZ(10);
    cameraNode->addChild(roomTypesNode);
    
    createHouse(false, true);
}

void HouseRenderer::resize() {
    this->aspectRatio = viewport.width / viewport.height;
    menuWidth = 300;
    renderViewPort.width = viewport.width - menuWidth;
    renderViewPort.height = viewport.height;
    
    setCamera();
    
    createMenu(menuWidth);
    prevRoom = hoverRoom = NULL;
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
    
    SAFE_DELETE(initialTranslate);
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
    Control* floorUpButton = houseRendererForm->getControl("floorUpButton");
    floorUpButton->setWidth(menuWidth);
    Control* floorDownButton = houseRendererForm->getControl("floorDownButton");
    floorDownButton->setWidth(menuWidth);
}

void HouseRenderer::createHouse(bool random, bool rooms) {
    prevHover = false;
    float screenSize = 100;
    if (rooms) {
        if (random) {
            house = HouseFactory::createRandomHouse(scene, screenSize);
        } else {
            house = HouseFactory::createRandomHouse(scene, screenSize, 5, 5);
        }
    } else {
        if (random) {
            house = HouseFactory::createHouse(scene, screenSize);
        } else {
            house = HouseFactory::createHouse(scene, screenSize, 5, 5);
        }
    }
}

void HouseRenderer::removeHouse() {
    SAFE_DELETE(house);
    prevRoom = hoverRoom = NULL;
    prevFloor = NULL;
}

bool HouseRenderer::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging, bool clicked) {
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
    } else if (clicked) {
        // handle in the touchEvent
        return false;
        //if (roomTypes->click(x, y)) {
        //  return true;
        //}
    } else if (evt == Mouse::MOUSE_MOVE) {
        if (roomTypes->hover(x, y)) {
            return true;
        }
        checkHover(x, y);
    } else if (evt == Mouse::MOUSE_WHEEL) {
        // Scrolled the mouse wheel
        if (wheelData > 5) {
            wheelData = 5;
        } else if (wheelData < -5) {
            wheelData = -5;
        }
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
        prevHover = house->getFloorTile(id);
        if (prevHover) {
            prevHover->setHover(true);
        }
    }
}

void HouseRenderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked) {
    if (clicked && roomTypes->click(x, y)) {
        return;
    }
    if (evt == Touch::TOUCH_MOVE && roomTypes->drag(x, y)) {
        return;
    }
    if (evt == Touch::TouchEvent::TOUCH_RELEASE) {
        // reset last floor on release
        prevFloor = NULL;
        hoverRoom = NULL;
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
        if (hoverRoom == NULL || !hoverRoom->contains(floor)) {
            // not the same room, toggle room
            hoverRoom = house->getRoom(floor);
            if (prevRoom != NULL && prevRoom != hoverRoom) {
                for (Floor * roomTile : prevRoom->getFloor()) {
                    roomTile->setSelected(false);
                }
            }
            if (hoverRoom != NULL) {
                for (Floor * roomTile : hoverRoom->getFloor()) {
                    roomTile->toggleSelect();
                }
            }
        }
        if (hoverRoom == NULL) {
            floor->toggleSelect();
        }
        prevFloor = floor;
    }
    prevRoom = hoverRoom;
}

int HouseRenderer::getViewTileId(int x, int y) {
    Camera* camera = scene->getActiveCamera();
    
    x -= renderViewPort.x;
    y -= renderViewPort.y;
    if (x < 0 || x > renderViewPort.width || y < 0 || y > renderViewPort.height) {
        return -1;
    }
    
    Vector2* rotated = new Vector2();
    rotated->x = (-curTranslate->x - curTranslate->y) / sqrt(2);
    rotated->y = (-curTranslate->x + curTranslate->y);
    
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
            house->addRoom(RoomFactory::createRoomFromFloor(scene, house, roomTiles));
        }
    }
}
void HouseRenderer::resizeEvent(unsigned int width, unsigned int height) {
    Renderer::resizeEvent(width, height);
    resize();
}

void HouseRenderer::update(float elapsedTime) {
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
}

Renderers HouseRenderer::getNextRenderer() {
    return nextRenderer;
}

void HouseRenderer::render(float elapsedTime) {
    // Draw the scene
    Floor** curFloor = house->getFloorTiles();
    while (curFloor - house->getFloorTiles() < house->getWidth() * house->getHeight()) {
        if (*curFloor) {
            (*curFloor)->getModel()->draw();
        }
        curFloor++;
    }
    for (Room * room : house->getRooms()) {
        for (Wall * wall : room->getWalls()) {
            wall->getModel()->draw();
        }
    }
    houseRendererForm->draw();
    roomTypes->draw(elapsedTime);
    
    if (prevHover) {
        Room* hoverRoom = house->getRoom(prevHover);
        if (hoverRoom) {
            char* buf = new char[100];
            sprintf(buf, "Room type: %s", hoverRoom->getRoomTypeString());
            drawText(Vector4(1, 1, 1, 1), 5, 50, buf);
            
            sprintf(buf, "Room size: %d", hoverRoom->getSize());
            drawText(Vector4(1, 1, 1, 1), 5, 65, buf);
            
            sprintf(buf, "Max sized rectangle: %d m2, at tile %d,%d in the %s direction.",
                    hoverRoom->getMaxAreaRect(), house->getXById(hoverRoom->getMaxRectangleId()),
                    house->getYById(hoverRoom->getMaxRectangleId()), (hoverRoom->getMaxRectDir() == Room::XDIR) ? "X" : "Y");
            drawText(Vector4(1, 1, 1, 1), 5, 80, buf);
            
            sprintf(buf, "Max sized square: %d m2, at tile %d,%d\n",
                    hoverRoom->getMaxAreaSquare(), house->getXById(hoverRoom->getMaxSquareId()), house->getYById(hoverRoom->getMaxSquareId()));
            drawText(Vector4(1, 1, 1, 1), 5, 95, buf);
            
            sprintf(buf, "Max line: %d", hoverRoom->getMaxLine());
            drawText(Vector4(1, 1, 1, 1), 5, 110, buf);
            delete[] buf;
        }
    }
}

void HouseRenderer::controlEvent(Control* control, Control::Listener::EventType evt) {
    if (!strcmp("mainMenuButton", control->getId())) {
        nextRenderer = MAIN_MENU;
    } else if (!strcmp("refreshButton", control->getId())) {
        removeHouse();
        createHouse(true, true);
    } else if (!strcmp("clearButton", control->getId())) {
        removeHouse();
        createHouse(true, false);
    } else if (!strcmp("floorUpButton", control->getId())) {
        house->floorUp();
        //house->addFloorTop(scene);
    } else if (!strcmp("floorDownButton", control->getId())) {
        house->floorDown();
        //house->addFloorBottom(scene);
    }
    control->setState(Control::State::NORMAL);
}


void HouseRenderer::menuWheelEvent(MenuWheelPart* clickedPart) {
    if (prevRoom != NULL) {
        prevRoom->setRoomType((Room::Type)clickedPart->getId());
    }
}