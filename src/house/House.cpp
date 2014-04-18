#include "House.h"

House::House(int w, int h, float screenSize): width(w), height(h), screenSize(screenSize), middleIndex(0) {
    if (width <= 0 || height <= 0) {
        throw "Invalid house size (has to be bigger than 0)";
    }
    float maxAxis = max(getWidth(), getHeight());
    Floor::setHeightWidth(screenSize / maxAxis, screenSize / maxAxis);
    
    srand(time(NULL));
}

House::~House() {
    for (list<Room*> floorRooms : rooms) {
        for (Room * room : floorRooms) {
            SAFE_DELETE(room);
        }
    }
    for (Floor** floor : floors) {
        for (int i = 0; i < getWidth() * getHeight(); i++) {
            SAFE_DELETE(floor[i]);
        }
        SAFE_DELETE_ARRAY(floor);
    }
}

bool House::floorUp() {
    return setFloor(floorIndex + 1);
}

bool House::floorDown() {
    return setFloor(floorIndex - 1);
}

void House::setFloorMiddle() {
    setFloor(middleIndex);
}

bool House::setFloor(unsigned int floorIndex) {
    if (floorIndex < floors.size()) {
        this->floorIndex = floorIndex;
        floorTiles = floors[floorIndex];
		trigger(CURRENT_FLOOR);
    }
    return false;
}

void House::addFloorBottom(Scene* scene) {
    floorIndex = 0;
    middleIndex++;
    addFloor(scene, floors.size() ? floors.front() : NULL);
    floors.insert(floors.begin(), floorTiles);
    rooms.insert(rooms.begin(), list<Room*>());
}

void House::addFloorTop(Scene* scene) {
    floorIndex = floors.size();
    addFloor(scene, floors.size() ? floors.back() : NULL);
    floors.push_back(floorTiles);
    rooms.push_back(list<Room*>());
}

void House::addFloor(Scene* scene, Floor** refFloor) {
    // get the floor (up or below this new floor) and use it as reference to where tiles should be
    
    floorTiles = new Floor*[getWidth() * getHeight()];
    memset(floorTiles, 0, getWidth() * getHeight());
    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight(); y++) {
            // Make a new floor tile
            int id = getIdByXY(x, y);
            // tile on reference floor should exist and should be part of a room
            if (!refFloor || refFloor[id] && refFloor[id]->getRoom()) {
                Floor* floor = new Floor(id);
                
                Node* tileNode = scene->addNode();
                tileNode->translateX((x - (float)getWidth() / 2) * Floor::getWidth() + Floor::getWidth() / 2);
                tileNode->translateY((y - (float)getHeight() / 2) * Floor::getHeight() + Floor::getHeight() / 2);
                tileNode->setModel(floor->getModel());
                floorTiles[id] = floor;
            } else {
                floorTiles[id] = NULL;
            }
        }
    };
}

void House::removeFloorTop() {
    for (int i = 0; i < getWidth() * getHeight(); i++) {
        SAFE_DELETE(floors[floors.size() - 1][i]);
    }
    SAFE_DELETE_ARRAY(floors[floors.size() - 1]);
    for (Room * room : rooms[rooms.size() - 1]) {
        SAFE_DELETE(room);
    }
    floors.pop_back();
    rooms.pop_back();
    setFloor(floorIndex - 1);
}

void House::removeFloorBottom() {
    for (int i = 0; i < getWidth() * getHeight(); i++) {
        SAFE_DELETE(floors[0][i]);
    }
    SAFE_DELETE_ARRAY(floors[0]);
    for (Room * room : rooms[0]) {
        SAFE_DELETE(room);
    }
    floors.erase(floors.begin());
    rooms.erase(rooms.begin());
    middleIndex--;
    setFloor(0);
}

void House::addRoom(Room* room) {
    rooms[floorIndex].push_back(room);
    
    trigger(ROOM_COUNT);
}

int House::getWidth() {
    return width;
}

int House::getHeight() {
    return height;
}



Floor** House::getFloorTiles() {
    return floorTiles;
}

Floor* House::getFloorTile(int id) {
    if (id < 0 || id > getWidth() * getHeight()) {
        return NULL;
    }
    return floorTiles[id];
}

Floor* House::getFloorTile(int x, int y) {
    return getFloorTile(getIdByXY(x, y));
}

Floor* House::getFloorInDirection(int id, FloorDirection::Direction dir) {
    return getFloorTile(getFloorIdInDirection(getXById(id), getYById(id), dir));
}

Floor* House::getFloorInDirection(int x, int y, FloorDirection::Direction dir) {
    return getFloorTile(getFloorIdInDirection(x, y, dir));
}

int House::getFloorIdInDirection(int id, FloorDirection::Direction dir) {
    return getFloorIdInDirection(getXById(id), getYById(id), dir);
}

int House::getFloorIdInDirection(int x, int y, FloorDirection::Direction dir) {
    switch (dir) {
        case FloorDirection::TOP:
            y--;
            break;
        case FloorDirection::LEFT:
            x--;
            break;
        case FloorDirection::BOTTOM:
            y++;
            break;
        case FloorDirection::RIGHT:
            x++;
            break;
    }
    return getIdByXY(x, y);
}



list<Room*> House::getRooms() {
    return rooms[floorIndex];
}

Room* House::getRoom(Floor* tile) {
    for (Room * room : getRooms()) {
        if (room->contains(tile)) {
            return room;
        }
    }
    return NULL;
}

int House::getIdByXY(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return -1;
    }
    return y * width + x;
}

int House::getXById(int id) {
    return id % width;
}

int House::getYById(int id) {
    return id / width;
}



int House::getRoomCount() {
	return rooms[floorIndex].size();
}

int House::getCurrentFloor() {
	return floorIndex;
}