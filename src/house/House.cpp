#include "House.h"

House::House(int w, int h): width(w), height(h) {
    if (width <= 0 || height <= 0) {
        throw "Invalid house size (has to be bigger than 0)";
    }
    floorTiles = new Floor*[getWidth() * getHeight()];
    memset(floorTiles, 0, getWidth() * getHeight());
    srand(time(NULL));
}

House::~House(void) {
	for (Room* room : rooms) {
		SAFE_DELETE(room);
	}
	for (int i = 0; i < getWidth() * getHeight(); i++) {
		SAFE_DELETE(floorTiles[i]);
	}
	SAFE_DELETE_ARRAY(floorTiles);
}

void House::addFloor(Scene* scene, float screenSize) {
    float maxAxis = max(getWidth(), getHeight());
	Floor::setHeightWidth(screenSize / maxAxis, screenSize / maxAxis);
    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight(); y++) {
            // Make a new floor tile
            Floor* floor = new Floor(getIdByXY(x, y));
			
            Node* tileNode = scene->addNode();
            tileNode->translateX((x - (float)getWidth() / 2) * Floor::getWidth() + Floor::getWidth() / 2);
            tileNode->translateY((y - (float)getHeight() / 2) * Floor::getHeight() + Floor::getHeight() / 2);
            tileNode->setModel(floor->getModel());
            floorTiles[getIdByXY(x, y)] = floor;
			//SAFE_RELEASE(tileNode);
        }
    };
}

void House::addRoom(Room* room) {
    rooms.push_back(room);
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

Floor* House::getFloorInDirection(int id, Floor::Direction dir) {
	return getFloorTile(getFloorIdInDirection(getXById(id), getYById(id), dir));
}

Floor* House::getFloorInDirection(int x, int y, Floor::Direction dir) {
	return getFloorTile(getFloorIdInDirection(x, y, dir));
}

int House::getFloorIdInDirection(int id, Floor::Direction dir) {
	return getFloorIdInDirection(getXById(id), getYById(id), dir);
}

int House::getFloorIdInDirection(int x, int y, Floor::Direction dir) {
	switch (dir) {
		case Floor::Direction::TOP: y--; break;
		case Floor::Direction::LEFT: x--; break;
		case Floor::Direction::BOTTOM: y++; break;
		case Floor::Direction::RIGHT: x++; break;
	}
	return getIdByXY(x, y);
}



list<Room*> House::getRooms() {
    return rooms;
}

Room* House::getRoom(Floor* tile) {
	for (Room* room : rooms) {
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