#include "House.h"


House::House(int w, int h): width(w), height(h) {
    if(width <= 0 || height <= 0) {
        throw "Invalid house size (has to be bigger than 0)";
    }
    floorTiles = new Floor*[getWidth() * getHeight()];
	memset(floorTiles, 0, getWidth() * getHeight());
	srand(time(NULL));
}

House::~House(void) {
}

void House::addFloor(Scene* scene, RenderState::StateBlock* stateBlock, float screenSize) {
	float maxAxis = max(getWidth(), getHeight());
	Floor::width = screenSize / maxAxis;
    Floor::height = screenSize / maxAxis;

    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight(); y++) {
            // Make a new floor tile
            Floor* floor = new Floor(stateBlock, (x - (float)getWidth() / 2) * Floor::width + Floor::width / 2, (y - (float)getHeight() / 2) * Floor::height + Floor::height / 2);

            Node* tileNode = scene->addNode();
            tileNode->translateX(floor->getX());
            tileNode->translateY(floor->getY());
            tileNode->setModel(floor->getModel());
            floorTiles[getIdByXY(x, y)] = floor;
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

list<Room*> House::getRooms() {
    return rooms;
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

House* House::addRandomRooms(Scene* scene, RenderState::StateBlock* stateBlock) {
	if (this->getRooms().size() != 0) {
		throw "Cannot add rooms to non-empty house";
	}

	// create a hall, either 1*1 or 2*2
	bool largeHallStart = (getWidth() + getHeight()) / 2 >= 8;
	int hallStartX, hallStartY;
	if (rand() % 2) {
		// start the hall on x-axis
		hallStartY = rand() % 2 * (getHeight() - largeHallStart - 1);
		hallStartX = rand() % (getWidth() - largeHallStart - 1);
	}else {
		// start the hall on y-axis
		hallStartX = rand() % 2 * (getWidth() - largeHallStart - 1);
		hallStartY = rand() % (getHeight() - largeHallStart - 1);
	}
	Floor** roomTiles;
	roomTiles = new Floor*[getWidth() * getHeight()];
	memset(roomTiles, NULL, sizeof(Floor*) * getWidth() * getHeight());

	// list of all possible hall places
	vector<int>* hallPossibilities = new vector<int>();

	pushAllHallAround(hallPossibilities, hallStartX, hallStartY);
	roomTiles[getIdByXY(hallStartX, hallStartY)] = getFloorTile(hallStartX, hallStartY)->setSelected(true);
	if (largeHallStart) {
		// add extra tiles to hallstart
		pushAllHallAround(hallPossibilities, hallStartX + 1, hallStartY);
		pushAllHallAround(hallPossibilities, hallStartX, hallStartY + 1);
		pushAllHallAround(hallPossibilities, hallStartX + 1, hallStartY + 1);
		roomTiles[getIdByXY(hallStartX + 1, hallStartY)] = getFloorTile(hallStartX + 1, hallStartY)->setSelected(true);
		roomTiles[getIdByXY(hallStartX, hallStartY + 1)] = getFloorTile(hallStartX, hallStartY + 1)->setSelected(true);
		roomTiles[getIdByXY(hallStartX + 1, hallStartY + 1)] = getFloorTile(hallStartX + 1, hallStartY + 1)->setSelected(true);
	}
	addRoom(Room::createRoomFromFloor(scene, this, stateBlock, roomTiles, getWidth() * getHeight()));

	// create the hallway connecting to the hallstart
	memset(roomTiles, NULL, sizeof(Floor*) * getWidth() * getHeight());
	// select random room from possible hall places
	int id = (int)(rand() % hallPossibilities->size());
	Floor* tile = getFloorTile((*hallPossibilities)[id]);
	tile->setSelected(true);
	roomTiles[getIdByXY(tile->getX(), tile->getY())] = tile;
	addRoom(Room::createRoomFromFloor(scene, this, stateBlock, roomTiles, getWidth() * getHeight()));


	return NULL;
}

void House::pushAllHallAround(vector<int>* ids, int x, int y) {
	// only non occupied tiles with no side next to them can be made
	// also no hallway tile may touch another occupied tile
	Floor* tile = getFloorTile(x - 1, y);
	if (tile != NULL && !tile->getSelected() && !floorTouchesSide(x - 1, y) && !floorHasNeighbours(x - 1, y)) {
		ids->push_back(getIdByXY(x - 1, y));
	}
	tile = getFloorTile(x, y - 1);
	if (tile != NULL && !tile->getSelected() && !floorTouchesSide(x, y - 1) && !floorHasNeighbours(x, y - 1)) {
		ids->push_back(getIdByXY(x, y - 1));
	}
	tile = getFloorTile(x + 1, y);
	if (tile != NULL && !tile->getSelected() && !floorTouchesSide(x + 1, y) && !floorHasNeighbours(x + 1, y)) {
		ids->push_back(getIdByXY(x + 1, y));
	}
	tile = getFloorTile(x, y + 1);
	if (tile != NULL && !tile->getSelected() && !floorTouchesSide(x, y + 1) && !floorHasNeighbours(x, y + 1)) {
		ids->push_back(getIdByXY(x, y + 1));
	}
}

bool House::floorTouchesSide(int x, int y) {
	return x + 1 >= getWidth() || x - 1 < 0
			|| y + 1 >= getHeight() || y - 1 < 0;
}

bool House::floorHasNeighbours(int x, int y) {
	int id = getIdByXY(x - 1, y);
	if (id != -1 && getFloorTile(id)->getSelected()) {
		return true;
	}
	id = getIdByXY(x, y - 1);
	if (id != -1 && getFloorTile(id)->getSelected()) {
		return true;
	}
	id = getIdByXY(x + 1, y);
	if (id != -1 && getFloorTile(id)->getSelected()) {
		return true;
	}
	id = getIdByXY(x, y + 1);
	if (id != -1 && getFloorTile(id)->getSelected()) {
		return true;
	}
	return false;
}