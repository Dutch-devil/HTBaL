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
            Floor* floor = new Floor(getIdByXY(x, y), stateBlock, (x - (float)getWidth() / 2) * Floor::width + Floor::width / 2, (y - (float)getHeight() / 2) * Floor::height + Floor::height / 2);

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
        hallStartX = rand() % (getWidth() - largeHallStart - 3) + 1;
    } else {
        // start the hall on y-axis
        hallStartX = rand() % 2 * (getWidth() - largeHallStart - 1);
        hallStartY = rand() % (getHeight() - largeHallStart - 3) + 1;
    }
    list<Floor*> roomTiles = list<Floor*>();

    // list of all possible hall places
    vector<int>* hallPossibilities = new vector<int>();

    pushAllHallAround(hallPossibilities, hallStartX, hallStartY);
    if (largeHallStart) {
        // add extra tiles to hallstart
        pushAllHallAround(hallPossibilities, hallStartX + 1, hallStartY);
        pushAllHallAround(hallPossibilities, hallStartX, hallStartY + 1);
        pushAllHallAround(hallPossibilities, hallStartX + 1, hallStartY + 1);
        roomTiles.push_back(getFloorTile(hallStartX, hallStartY)->setSelected(true));
        roomTiles.push_back(getFloorTile(hallStartX + 1, hallStartY)->setSelected(true));
        roomTiles.push_back(getFloorTile(hallStartX, hallStartY + 1)->setSelected(true));
        roomTiles.push_back(getFloorTile(hallStartX + 1, hallStartY + 1)->setSelected(true));

        for (vector<int>::iterator itr = hallPossibilities->begin(); itr != hallPossibilities->end();) {
            int id = *itr;
            if (id == getIdByXY(hallStartX, hallStartY) || id == getIdByXY(hallStartX + 1, hallStartY) ||
                    id == getIdByXY(hallStartX, hallStartY + 1) || id == getIdByXY(hallStartX + 1, hallStartY + 1)) {
                itr = hallPossibilities->erase(itr);
            } else {
                itr++;
            }
        }
    } else {
        roomTiles.push_back(getFloorTile(hallStartX, hallStartY)->setSelected(true));
    }
    addRoom(Room::createRoomFromFloor(scene, this, stateBlock, roomTiles));

    // create the hallway connecting to the hallstart
    // select random room from possible hall places
    roomTiles = list<Floor*>();
    int id = (*hallPossibilities)[(int)(rand() % hallPossibilities->size())];

    hallPossibilities->clear();
    clearAllAround(hallPossibilities, getXById(id), getYById(id));
    pushAllHallAround(hallPossibilities, getXById(id), getYById(id));

	// for determining hallway direction
	int prevX = getXById(id);
	int prevY = getYById(id);

    Floor* tile = getFloorTile(id);
    tile->setSelected(true);
    roomTiles.push_back(tile);

	int curX, curY, dir = -1;
	tile = NULL;
    for (int i = 0; i < getWidth() * getHeight() / 6.2; i++) {
		if (hallPossibilities->empty()) {
			break;
		}
		id = -1;
		if (dir != -1) {
			int testX = curX + (dir == 0?-1:(dir == 2?1:0));
			int testY = curY + (dir == 1?-1:(dir == 3?1:0));
			if (canBeHallway(testX, testY) && (rand() % 4)) {
				// keep on going straight
				id = getIdByXY(testX, testY);
			}
		}
		if (tile != NULL) {
			tile->setSelected(true);
		}

		if (id == -1) {
			id = (*hallPossibilities)[(int)(rand() % hallPossibilities->size())];
		}
		curX = getXById(id);
		curY = getYById(id);
        clearAllAround(hallPossibilities, curX, curY);

		if (prevX == curX) {
			if (curY < prevY) {
				dir = 1;	// up, x, y - 1
			}else {
				dir = 3;	// down, x, y + 1
			}
		}else {
			if (curX < prevX) {
				dir = 0;	// left, x - 1, y
			}else {
				dir = 2;	// right, x + 1, y
			}
		}
		prevX = curX;
		prevY = curY;
        pushAllHallAround(hallPossibilities, curX, curY);

        tile = getFloorTile(id);
        roomTiles.push_back(tile);
    }
	tile->setSelected(true);
    addRoom(Room::createRoomFromFloor(scene, this, stateBlock, roomTiles));

    for (int id : *hallPossibilities) {
        getFloorTile(id)->setSelected(true);
    }

    return this;
}

void House::clearAllAround(vector<int>* ids, int x, int y) {
    // clear all the possibilities around this newly generated tile
    for (vector<int>::iterator itr = ids->begin(); itr != ids->end();) {
        int id = *itr;
        if (id == getIdByXY(x, y) || id == getIdByXY(x - 1, y) || id == getIdByXY(x, y - 1) ||
                id == getIdByXY(x + 1, y) || id == getIdByXY(x, y + 1)) {
            itr = ids->erase(itr);
        } else {
            itr++;
        }
    }
}

void House::pushAllHallAround(vector<int>* ids, int x, int y) {
    // only non occupied tiles with no side next to them can be made
    // also no hallway tile may touch another occupied tile
	if (canBeHallway(x - 1, y)) {
        ids->push_back(getIdByXY(x - 1, y));
	}
	if (canBeHallway(x, y - 1)) {
        ids->push_back(getIdByXY(x, y - 1));
	}
	if (canBeHallway(x + 1, y)) {
        ids->push_back(getIdByXY(x + 1, y));
	}
	if (canBeHallway(x, y + 1)) {
        ids->push_back(getIdByXY(x, y + 1));
	}
}

bool House::canBeHallway(int x, int y) {
    Floor* tile = getFloorTile(x, y);
    return tile != NULL && !tile->getSelected() && !floorTouchesSide(x, y) && !floorHasNeighbours(x, y);
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