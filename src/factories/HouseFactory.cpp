#include "HouseFactory.h"

House* HouseFactory::createHouse(Scene* scene, float screenSize) {
    return createHouse(scene, screenSize, rand() % 11 + 5, rand() % 11 + 5);
}

House* HouseFactory::createHouse(Scene* scene, float screenSize, unsigned int width, unsigned int height) {
    House* house = new House(width, height);
    house->addFloor(scene, screenSize);
    return house;
}

House* HouseFactory::createRandomHouse(Scene* scene, float screenSize) {
    return createRandomHouse(scene, screenSize, rand() % 11 + 5, rand() % 11 + 5);
}

House* HouseFactory::createRandomHouse(Scene* scene, float screenSize, unsigned int width, unsigned int height) {
    print("%d, %d\n", width, height);
    House* house = createHouse(scene, screenSize, width, height);
    
    Floor* tile;                                                        // generic tile used along calculations
    
    bool largeEntrance;                                             // start with a small or large hall?
    list<Floor*> entranceTiles = list<Floor*>();                        // all tiles belonging to the hall start
    vector<WalledTile>* hallPossibilities = new vector<WalledTile>();   // list of all possible hall places
    
    list<Floor*> hallTiles = list<Floor*>();                            // all tiles belonging to the hall
    
    vector<WalledTile>* roomStartPosibilities = new vector<WalledTile>();   // all possible starts for new rooms
    vector<WalledTile>* roomPossibilities = new vector<WalledTile>();   // all possible places for the current room
    list<Floor*> roomTiles = list<Floor*>();                            // all tiles belonging to the current generated room
    
    // create a hall, either 1*1 or 2*2
    largeEntrance = (house->getWidth() + house->getHeight()) / 2 >= 8;
    int entranceX, entranceY;
    if (rand() % 2) {
        // start the hall on x-axis
        entranceY = rand() % 2 * (house->getHeight() - largeEntrance - 1);
        entranceX = rand() % (house->getWidth() - largeEntrance - 3) + 1;
    } else {
        // start the hall on y-axis
        entranceX = rand() % 2 * (house->getWidth() - largeEntrance - 1);
        entranceY = rand() % (house->getHeight() - largeEntrance - 3) + 1;
    }
    
    pushAllHallAround(house, hallPossibilities, entranceX, entranceY);
    if (largeEntrance) {
        // add extra tiles to entrance
        pushAllHallAround(house, hallPossibilities, entranceX + 1, entranceY);
        pushAllHallAround(house, hallPossibilities, entranceX, entranceY + 1);
        pushAllHallAround(house, hallPossibilities, entranceX + 1, entranceY + 1);
        entranceTiles.push_back(house->getFloorTile(entranceX, entranceY)->setSelected(true)->setColor(FLOOR_ENTRANCE));
        entranceTiles.push_back(house->getFloorTile(entranceX + 1, entranceY)->setSelected(true)->setColor(FLOOR_ENTRANCE));
        entranceTiles.push_back(house->getFloorTile(entranceX, entranceY + 1)->setSelected(true)->setColor(FLOOR_ENTRANCE));
        entranceTiles.push_back(house->getFloorTile(entranceX + 1, entranceY + 1)->setSelected(true)->setColor(FLOOR_ENTRANCE));
        
        for (vector<WalledTile>::iterator itr = hallPossibilities->begin(); itr != hallPossibilities->end();) {
            int id = (*itr).id;
            if (id == house->getIdByXY(entranceX, entranceY) || id == house->getIdByXY(entranceX + 1, entranceY) ||
                    id == house->getIdByXY(entranceX, entranceY + 1) || id == house->getIdByXY(entranceX + 1, entranceY + 1)) {
                itr = hallPossibilities->erase(itr);
            } else {
                itr++;
            }
        }
    } else {
        entranceTiles.push_back(house->getFloorTile(entranceX, entranceY)->setSelected(true)->setColor(FLOOR_ENTRANCE));
    }
    
    // create the hallway connecting to the hallstart
    // select random room from possible hall places
    WalledTile startTile = (*hallPossibilities)[(int)(rand() % hallPossibilities->size())];
    int id = startTile.id;
    
    hallPossibilities->clear();
    clearAllAround(house, hallPossibilities, id);
    pushAllHallAround(house, hallPossibilities, house->getXById(id), house->getYById(id));
    
    // for determining hallway direction
    int prevX = house->getXById(id);
    int prevY = house->getYById(id);
    
    tile = house->getFloorTile(id);
    
    tile->setDoor(startTile.dir, true);
    setDoor(house, entranceTiles, startTile);
    
    tile->setSelected(true);
    hallTiles.push_back(tile);
    tile->setColor(FLOOR_HALL);
    
    int curX, curY;
    Floor::Direction dir = Floor::Direction::NONE;
    tile = NULL;
    for (int i = 0; i < house->getWidth() * house->getHeight() / 6.2; i++) {
        if (hallPossibilities->empty()) {
            break;
        }
        id = -1;
        if (dir != Floor::Direction::NONE) {
            Floor* floor = house->getFloorInDirection(curX, curY, dir);
            if (canBeHallway(house, floor) && (rand() % 4)) {
                // keep on going straight
                id = floor->getId();
            }
        }
        if (tile != NULL) {
            tile->setSelected(true);
        }
        
        if (id == -1) {
            id = (*hallPossibilities)[(int)(rand() % hallPossibilities->size())].id;
        }
        curX = house->getXById(id);
        curY = house->getYById(id);
        clearAllAround(house, hallPossibilities, id);
        
        if (prevX == curX) {
            if (curY < prevY) {
                dir = Floor::Direction::TOP;    // up, x, y - 1
            } else {
                dir = Floor::Direction::BOTTOM; // down, x, y + 1
            }
        } else {
            if (curX < prevX) {
                dir = Floor::Direction::LEFT;   // left, x - 1, y
            } else {
                dir = Floor::Direction::RIGHT;  // right, x + 1, y
            }
        }
        prevX = curX;
        prevY = curY;
        pushAllHallAround(house, hallPossibilities, curX, curY);
        
        tile = house->getFloorTile(id);
        hallTiles.push_back(tile);
        tile->setColor(FLOOR_HALL);
    }
    tile->setSelected(true);
    
    // get all tiles next to hall tiles
    for (Floor * hallTile : hallTiles) {
        pushAllRoomAround(house, roomStartPosibilities, hallTile->getId());
    }
    list<Floor*> gaps = getGaps(house, roomStartPosibilities, 3);
    for (Floor * gap : gaps) {
        gap->setColor(FLOOR_HALL);
        removeId(roomStartPosibilities, gap->getId());
    }
    hallTiles.merge(gaps);
    
    // and add rooms to the house
    while (!roomStartPosibilities->empty()) {
        roomPossibilities->clear();
        roomTiles.clear();
        
        startTile = (*roomStartPosibilities)[(int)(rand() % roomStartPosibilities->size())];
        tile = house->getFloorTile(startTile.id);
        tile->setDoor(startTile.dir, true);
        roomTiles.push_back(tile->setSelected(true));
        removeId(roomStartPosibilities, tile->getId());
        pushAllRoomAround(house, roomPossibilities, tile->getId());
        for (int i = 0; i < rand() % house->getWidth() * house->getHeight() / 5 + 4; i++) {
            if (roomPossibilities->empty()) {
                break;
            }
            id = (*roomPossibilities)[(int)(rand() % roomPossibilities->size())].id;
            tile = house->getFloorTile(id);
            
            roomTiles.push_back(tile->setSelected(true));
            removeId(roomStartPosibilities, tile->getId());
            removeId(roomPossibilities, tile->getId());
            pushAllRoomAround(house, roomPossibilities, tile->getId());
        }
        list<Floor*> gaps = getGaps(house, roomPossibilities);
        for (Floor * gap : gaps) {
            removeId(roomStartPosibilities, gap->getId());
        }
        roomTiles.merge(gaps);
        
        if (roomTiles.size() >= 4) {
            setDoor(house, hallTiles, startTile);
            for (Floor * roomTile : roomTiles) {
                roomTile->setColor(FLOOR_ROOM);
            }
            house->addRoom(RoomFactory::createRoomFromFloor(scene, house, roomTiles));
        } else {
            for (Floor * roomTile : roomTiles) {
                roomTile->setSelected(false);
                roomTile->setDoor(startTile.dir, false);
            }
        }
    }
    
    house->addRoom(RoomFactory::createRoomFromFloor(scene, house, entranceTiles, Room::ENTRANCE));
    house->addRoom(RoomFactory::createRoomFromFloor(scene, house, hallTiles, Room::HALL));
    
    for (int i = 0; i < house->getWidth() * house->getHeight(); i++) {
        if (!house->getFloorTile(i)->getSelected()) {
            house->getFloorTile(i)->setColor(FLOOR_NONE);
        }
        house->getFloorTile(i)->setSelected(false);
    }
    SAFE_DELETE(hallPossibilities);
    SAFE_DELETE(roomStartPosibilities);
    SAFE_DELETE(roomPossibilities);
    return house;
}

void HouseFactory::setDoor(House* house, list<Floor*> tiles, WalledTile adjacent) {
    int searchId = house->getFloorIdInDirection(adjacent.id, adjacent.dir);
    for (Floor * tile : tiles) {
        if (tile->getId() == searchId) {
            /**
            * @todo vervangend door Direction.invert
            */
            tile->setDoor((Floor::Direction)((adjacent.dir + 2) % 4), true);
            break;
        }
    }
}

void HouseFactory::clearAllAround(House* house, vector<WalledTile>* ids, int startId) {
    // clear all the possibilities around this newly generated tile
    for (vector<WalledTile>::iterator itr = ids->begin(); itr != ids->end();) {
        int id = (*itr).id;
        if (id == startId || id == house->getFloorIdInDirection(startId, Floor::Direction::LEFT) || id == house->getFloorIdInDirection(startId, Floor::Direction::TOP) ||
                id == house->getFloorIdInDirection(startId, Floor::Direction::RIGHT) || id == house->getFloorIdInDirection(startId, Floor::Direction::BOTTOM)) {
            itr = ids->erase(itr);
        } else {
            itr++;
        }
    }
}

void HouseFactory::pushAllHallAround(House* house, vector<WalledTile>* ids, int x, int y) {
    // only non occupied tiles with no side next to them can be made
    // also no hallway tile may touch another occupied tile
    Floor* floor = house->getFloorInDirection(x, y, Floor::Direction::LEFT);
    if (canBeHallway(house, floor)) {
        ids->push_back(WalledTile(floor->getId(), Floor::Direction::RIGHT));
    }
    floor = house->getFloorInDirection(x, y, Floor::Direction::BOTTOM);
    if (canBeHallway(house, floor)) {
        ids->push_back(WalledTile(floor->getId(), Floor::Direction::TOP));
    }
    floor = house->getFloorInDirection(x, y, Floor::Direction::RIGHT);
    if (canBeHallway(house, floor)) {
        ids->push_back(WalledTile(floor->getId(), Floor::Direction::LEFT));
    }
    floor = house->getFloorInDirection(x, y, Floor::Direction::TOP);
    if (canBeHallway(house, floor)) {
        ids->push_back(WalledTile(floor->getId(), Floor::Direction::BOTTOM));
    }
}

bool HouseFactory::canBeHallway(House* house, Floor* tile) {
    return tile != NULL && !tile->getSelected() && !floorTouchesSide(house, tile->getId()) && !floorHasNeighbours(house, tile->getId());
}

bool HouseFactory::floorTouchesSide(House* house, int id) {
    int x = house->getXById(id);
    int y = house->getYById(id);
    return x + 1 >= house->getWidth() || x - 1 < 0 || y + 1 >= house->getHeight() || y - 1 < 0;
}

bool HouseFactory::floorHasNeighbours(House* house, int id) {
    int nextId = house->getFloorIdInDirection(id, Floor::Direction::LEFT);
    if (nextId != -1 && house->getFloorTile(nextId)->getSelected()) {
        return true;
    }
    nextId = house->getFloorIdInDirection(id, Floor::Direction::TOP);
    if (nextId != -1 && house->getFloorTile(nextId)->getSelected()) {
        return true;
    }
    nextId = house->getFloorIdInDirection(id, Floor::Direction::RIGHT);
    if (nextId != -1 && house->getFloorTile(nextId)->getSelected()) {
        return true;
    }
    nextId = house->getFloorIdInDirection(id, Floor::Direction::BOTTOM);
    if (nextId != -1 && house->getFloorTile(nextId)->getSelected()) {
        return true;
    }
    return false;
}

void HouseFactory::pushAllRoomAround(House* house, vector<int>* ids, int id) {
    // all non-occupied tiles next to this one can be room
    Floor* floor = house->getFloorInDirection(id, Floor::Direction::LEFT);
    if (canBeRoom(floor)) {
        ids->push_back(floor->getId());
    }
    floor = house->getFloorInDirection(id, Floor::Direction::BOTTOM);
    if (canBeRoom(floor)) {
        ids->push_back(floor->getId());
    }
    floor = house->getFloorInDirection(id, Floor::Direction::RIGHT);
    if (canBeRoom(floor)) {
        ids->push_back(floor->getId());
    }
    floor = house->getFloorInDirection(id, Floor::Direction::TOP);
    if (canBeRoom(floor)) {
        ids->push_back(floor->getId());
    }
}

void HouseFactory::pushAllRoomAround(House* house, vector<WalledTile>* ids, int id) {
    // all non-occupied tiles next to this one can be room
    Floor* floor = house->getFloorInDirection(id, Floor::Direction::LEFT);
    if (canBeRoom(floor)) {
        ids->push_back(WalledTile(floor->getId(), Floor::Direction::RIGHT));
    }
    floor = house->getFloorInDirection(id, Floor::Direction::BOTTOM);
    if (canBeRoom(floor)) {
        ids->push_back(WalledTile(floor->getId(), Floor::Direction::TOP));
    }
    floor = house->getFloorInDirection(id, Floor::Direction::RIGHT);
    if (canBeRoom(floor)) {
        ids->push_back(WalledTile(floor->getId(), Floor::Direction::LEFT));
    }
    floor = house->getFloorInDirection(id, Floor::Direction::TOP);
    if (canBeRoom(floor)) {
        ids->push_back(WalledTile(floor->getId(), Floor::Direction::BOTTOM));
    }
}

bool HouseFactory::canBeRoom(Floor* tile) {
    return tile != NULL && !tile->getSelected();
}

void HouseFactory::removeId(vector<int>* ids, int id) {
    for (vector<int>::iterator itr = ids->begin(); itr != ids->end();) {
        if (*itr == id) {
            itr = ids->erase(itr);
        } else {
            itr++;
        }
    }
}

void HouseFactory::removeId(vector<WalledTile>* ids, int id) {
    for (vector<WalledTile>::iterator itr = ids->begin(); itr != ids->end();) {
        if ((*itr).id == id) {
            itr = ids->erase(itr);
        } else {
            itr++;
        }
    }
}

list<Floor*> HouseFactory::getGaps(House* house, vector<WalledTile>* toCheck) {
    return getGaps(house, toCheck, 0);
}

list<Floor*> HouseFactory::getGaps(House* house, vector<WalledTile>* toCheck, unsigned int maxSize) {
    list<Floor*> gapTiles;
    for (WalledTile possibility : *toCheck) {
        list<Floor*>* tmp = new list<Floor*>();
        if (!getEnclosed(house, possibility.id, tmp) || (maxSize > 0 && tmp->size() > maxSize)) {
            for (Floor * unselect : *tmp) {
                unselect->setSelected(false);
            }
        } else {
            gapTiles.merge(*tmp);
        }
        SAFE_DELETE(tmp);
    }
    return gapTiles;
}

bool HouseFactory::getEnclosed(House* house, int startId, list<Floor*>* others) {
    if (floorTouchesSide(house, startId)) {
        return false;
    }
    if (!house->getFloorTile(startId)->getSelected()) {
        others->push_back(house->getFloorTile(startId)->setSelected(true));
    }
    vector<int>* ids = new vector<int>();
    pushAllRoomAround(house, ids, startId);
    for (int id : *ids) {
        if (!getEnclosed(house, id, others)) {
            SAFE_DELETE(ids);
            return false;
        }
    }
    SAFE_DELETE(ids);
    return true;
}