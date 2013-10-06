#include "House.h"
#include "MaterialManager.h"

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

void House::addRandomRooms(Scene* scene) {
    if (this->getRooms().size() != 0) {
        throw "Cannot add rooms to non-empty house";
    }
	Floor* tile;														// generic tile used along calculations

	bool largeHallStart;												// start with a small or large hall?
    list<Floor*> hallStartTiles = list<Floor*>();						// all tiles belonging to the hall start
    vector<WalledTile>* hallPossibilities = new vector<WalledTile>();	// list of all possible hall places
	
    list<Floor*> hallTiles = list<Floor*>();							// all tiles belonging to the hall
	
	vector<WalledTile>* roomStartPosibilities = new vector<WalledTile>();	// all possible starts for new rooms
    vector<WalledTile>* roomPossibilities = new vector<WalledTile>();	// all possible places for the current room
	list<Floor*> roomTiles = list<Floor*>();							// all tiles belonging to the current generated room

    // create a hall, either 1*1 or 2*2
   largeHallStart = (getWidth() + getHeight()) / 2 >= 8;
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

    pushAllHallAround(hallPossibilities, hallStartX, hallStartY);
    if (largeHallStart) {
        // add extra tiles to hallstart
        pushAllHallAround(hallPossibilities, hallStartX + 1, hallStartY);
        pushAllHallAround(hallPossibilities, hallStartX, hallStartY + 1);
        pushAllHallAround(hallPossibilities, hallStartX + 1, hallStartY + 1);
        hallStartTiles.push_back(getFloorTile(hallStartX, hallStartY)->setSelected(true)->setColor(FLOOR_HALL_START));
        hallStartTiles.push_back(getFloorTile(hallStartX + 1, hallStartY)->setSelected(true)->setColor(FLOOR_HALL_START));
        hallStartTiles.push_back(getFloorTile(hallStartX, hallStartY + 1)->setSelected(true)->setColor(FLOOR_HALL_START));
        hallStartTiles.push_back(getFloorTile(hallStartX + 1, hallStartY + 1)->setSelected(true)->setColor(FLOOR_HALL_START));

        for (vector<WalledTile>::iterator itr = hallPossibilities->begin(); itr != hallPossibilities->end();) {
            int id = (*itr).id;
            if (id == getIdByXY(hallStartX, hallStartY) || id == getIdByXY(hallStartX + 1, hallStartY) ||
                    id == getIdByXY(hallStartX, hallStartY + 1) || id == getIdByXY(hallStartX + 1, hallStartY + 1)) {
                itr = hallPossibilities->erase(itr);
            } else {
                itr++;
            }
        }
    } else {
        hallStartTiles.push_back(getFloorTile(hallStartX, hallStartY)->setSelected(true)->setColor(FLOOR_HALL_START));
    }

    // create the hallway connecting to the hallstart
    // select random room from possible hall places
    WalledTile startTile = (*hallPossibilities)[(int)(rand() % hallPossibilities->size())];
	int id = startTile.id;

    hallPossibilities->clear();
    clearAllAround(hallPossibilities, getXById(id), getYById(id));
    pushAllHallAround(hallPossibilities, getXById(id), getYById(id));

    // for determining hallway direction
    int prevX = getXById(id);
    int prevY = getYById(id);

    tile = getFloorTile(id);

	tile->setDoor(startTile.dir, true);
	setDoor(hallStartTiles, startTile);

    tile->setSelected(true);
    hallTiles.push_back(tile);
    tile->setColor(FLOOR_HALL);

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
            id = (*hallPossibilities)[(int)(rand() % hallPossibilities->size())].id;
        }
        curX = getXById(id);
        curY = getYById(id);
        clearAllAround(hallPossibilities, curX, curY);

        if (prevX == curX) {
            if (curY < prevY) {
                dir = 1;	// up, x, y - 1
            } else {
                dir = 3;	// down, x, y + 1
            }
        } else {
            if (curX < prevX) {
                dir = 0;	// left, x - 1, y
            } else {
                dir = 2;	// right, x + 1, y
            }
        }
        prevX = curX;
        prevY = curY;
        pushAllHallAround(hallPossibilities, curX, curY);

        tile = getFloorTile(id);
        hallTiles.push_back(tile);
        tile->setColor(FLOOR_HALL);
    }
	tile->setSelected(true);

    // get all tiles next to hall tiles
    for (Floor* hallTile : hallTiles) {
        pushAllRoomAround(roomStartPosibilities, getXById(hallTile->getId()), getYById(hallTile->getId()));
    }
	list<Floor*> gaps = getGaps(roomStartPosibilities, 3);
	for (Floor* gap : gaps) {
		gap->setColor(FLOOR_HALL);
		removeId(roomStartPosibilities, gap->getId());
	}
	hallTiles.merge(gaps);
	
    // and add rooms to the house
    while (!roomStartPosibilities->empty()) {
        roomPossibilities->clear();
        roomTiles.clear();

        startTile = (*roomStartPosibilities)[(int)(rand() % roomStartPosibilities->size())];
        tile = getFloorTile(startTile.id);
		tile->setDoor(startTile.dir, true);
        roomTiles.push_back(tile->setSelected(true));
        removeId(roomStartPosibilities, tile->getId());
        pushAllRoomAround(roomPossibilities, getXById(tile->getId()), getYById(tile->getId()));
        for (int i = 0; i < rand() % getWidth() * getHeight() / 5 + 4; i++) {
            if (roomPossibilities->empty()) {
                break;
            }
            id = (*roomPossibilities)[(int)(rand() % roomPossibilities->size())].id;
            tile = getFloorTile(id);

            roomTiles.push_back(tile->setSelected(true));
            removeId(roomStartPosibilities, tile->getId());
            removeId(roomPossibilities, tile->getId());
            pushAllRoomAround(roomPossibilities, getXById(tile->getId()), getYById(tile->getId()));
        }
		list<Floor*> gaps = getGaps(roomPossibilities);
		for (Floor* gap : gaps) {
			removeId(roomStartPosibilities, gap->getId());
		}
        roomTiles.merge(gaps);

        if (roomTiles.size() >= 4) {
			setDoor(hallTiles, startTile);
            for (Floor* roomTile : roomTiles) {
                roomTile->setColor(FLOOR_ROOM);
            }
            addRoom(Room::createRoomFromFloor(scene, this, roomTiles));
        }else {
			for (Floor* roomTile : roomTiles) {
				roomTile->setSelected(false);
				roomTile->setDoor(startTile.dir, false);
			}
		}
    }

    addRoom(Room::createRoomFromFloor(scene, this, hallStartTiles));
    addRoom(Room::createRoomFromFloor(scene, this, hallTiles));

	for (int i = 0; i < getWidth() * getHeight(); i++) {
		if (!getFloorTile(i)->getSelected()) {
			getFloorTile(i)->setColor(FLOOR_NONE);
		}
		getFloorTile(i)->setSelected(false);
	}
    SAFE_DELETE(hallPossibilities);
	SAFE_DELETE(roomStartPosibilities);
	SAFE_DELETE(roomPossibilities);
}

void House::setDoor(list<Floor*> tiles, WalledTile adjacent) {
	int searchId = getIdByXY(getXById(adjacent.id) + (adjacent.dir == LEFT?-1:(adjacent.dir == RIGHT?1:0)),
							 getYById(adjacent.id) + (adjacent.dir == BOTTOM?-1:(adjacent.dir == TOP?1:0)));
	for (Floor* tile : tiles) {
		if (tile->getId() == searchId) {
			tile->setDoor(adjacent.dir == LEFT?RIGHT:(adjacent.dir == RIGHT?LEFT:(adjacent.dir == TOP?BOTTOM:(adjacent.dir == BOTTOM?TOP:NONE))), true);
			break;
		}
	}
}

void House::clearAllAround(vector<WalledTile>* ids, int x, int y) {
    // clear all the possibilities around this newly generated tile
    for (vector<WalledTile>::iterator itr = ids->begin(); itr != ids->end();) {
        int id = (*itr).id;
        if (id == getIdByXY(x, y) || id == getIdByXY(x - 1, y) || id == getIdByXY(x, y - 1) ||
                id == getIdByXY(x + 1, y) || id == getIdByXY(x, y + 1)) {
            itr = ids->erase(itr);
        } else {
            itr++;
        }
    }
}

void House::pushAllHallAround(vector<WalledTile>* ids, int x, int y) {
    // only non occupied tiles with no side next to them can be made
    // also no hallway tile may touch another occupied tile
    if (canBeHallway(x - 1, y)) {
        ids->push_back(WalledTile(getIdByXY(x - 1, y), RIGHT));
    }
    if (canBeHallway(x, y - 1)) {
        ids->push_back(WalledTile(getIdByXY(x, y - 1), TOP));
    }
    if (canBeHallway(x + 1, y)) {
        ids->push_back(WalledTile(getIdByXY(x + 1, y), LEFT));
    }
    if (canBeHallway(x, y + 1)) {
        ids->push_back(WalledTile(getIdByXY(x, y + 1), BOTTOM));
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

void House::pushAllRoomAround(vector<int>* ids, int x, int y) {
    // all non-occupied tiles next to this one can be room
    if (canBeRoom(x - 1, y)) {
        ids->push_back(getIdByXY(x - 1, y));
    }
    if (canBeRoom(x, y - 1)) {
        ids->push_back(getIdByXY(x, y - 1));
    }
    if (canBeRoom(x + 1, y)) {
        ids->push_back(getIdByXY(x + 1, y));
    }
    if (canBeRoom(x, y + 1)) {
        ids->push_back(getIdByXY(x, y + 1));
    }
}

void House::pushAllRoomAround(vector<WalledTile>* ids, int x, int y) {
    // all non-occupied tiles next to this one can be room
    if (canBeRoom(x - 1, y)) {
		ids->push_back(WalledTile(getIdByXY(x - 1, y), RIGHT));
    }
    if (canBeRoom(x, y - 1)) {
        ids->push_back(WalledTile(getIdByXY(x, y - 1), TOP));
    }
    if (canBeRoom(x + 1, y)) {
        ids->push_back(WalledTile(getIdByXY(x + 1, y), LEFT));
    }
    if (canBeRoom(x, y + 1)) {
        ids->push_back(WalledTile(getIdByXY(x, y + 1), BOTTOM));
    }
}

bool House::canBeRoom(int x, int y) {
    Floor* tile = getFloorTile(x, y);
    return tile != NULL && !tile->getSelected();
}

void House::removeId(vector<int>* ids, int id) {
    for (vector<int>::iterator itr = ids->begin(); itr != ids->end();) {
        if (*itr == id) {
            itr = ids->erase(itr);
        } else {
            itr++;
        }
    }
}

void House::removeId(vector<WalledTile>* ids, int id) {
    for (vector<WalledTile>::iterator itr = ids->begin(); itr != ids->end();) {
        if ((*itr).id == id) {
            itr = ids->erase(itr);
        } else {
            itr++;
        }
    }
}

list<Floor*> House::getGaps(vector<WalledTile>* toCheck) {
	return getGaps(toCheck, 0);
}

list<Floor*> House::getGaps(vector<WalledTile>* toCheck, unsigned int maxSize) {
	list<Floor*> gapTiles;
    for (WalledTile possibility : *toCheck) {
        list<Floor*>* tmp = new list<Floor*>();
        if (!getEnclosed(possibility.id, tmp) || (maxSize > 0 && tmp->size() > maxSize)) {
            for (Floor* unselect : *tmp) {
                unselect->setSelected(false);
            }
        } else {
            gapTiles.merge(*tmp);
        }
		SAFE_DELETE(tmp);
    }
	return gapTiles;
}

bool House::getEnclosed(int startId, list<Floor*>* others) {
    if (floorTouchesSide(getXById(startId), getYById(startId))) {
        return false;
    }
    others->push_back(getFloorTile(startId)->setSelected(true));
    vector<int>* ids = new vector<int>();
    pushAllRoomAround(ids, getXById(startId), getYById(startId));
    for (int id : *ids) {
        if (!getEnclosed(id, others)) {
			SAFE_DELETE(ids);
            return false;
        }
    }
	SAFE_DELETE(ids);
    return true;
}