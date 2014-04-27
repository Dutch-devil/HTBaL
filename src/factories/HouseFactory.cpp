#include "HouseFactory.h"

House* HouseFactory::createHouse(Scene* scene, float screenSize) {
    return createHouse(scene, screenSize, rand() % 11 + 5, rand() % 11 + 5);
}

House* HouseFactory::createHouse(Scene* scene, float screenSize, unsigned int width, unsigned int height) {
    House* house = new House(width, height, screenSize);
	house->pauseTrigger();
    house->addFloorTop(scene);
	house->resumeTrigger();
    return house;
}

House* HouseFactory::createRandomHouse(Scene* scene, float screenSize) {
    return createRandomHouse(scene, screenSize, rand() % 11 + 5, rand() % 11 + 5);
}

House* HouseFactory::createRandomHouse(Scene* scene, float screenSize, unsigned int width, unsigned int height) {
    House* house = createHouse(scene, screenSize, width, height);
	house->pauseTrigger();
    
    vector<WalledTile>* hallPossibilities = new vector<WalledTile>();   // list of all possible hall places
    vector<WalledTile>* roomStartPosibilities = new vector<WalledTile>();   // all possible starts for new rooms
    vector<WalledTile>* roomPossibilities = new vector<WalledTile>();   // all possible places for the current room
    
    // first entrance can be large, after that always small
    bool largeEntrance = (house->getWidth() + house->getHeight()) / 2 >= 8;// start with a small or large hall?
    
    // first x and y are random, then it is the last hall tile
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
    
    int stairX = -1, stairY = -1;
    
    int max = 10;
    for (int floorIndex = 0; floorIndex < max; floorIndex++) {
        if (floorIndex) {
            if (floorIndex > max / 2) {
                house->addFloorBottom(scene);
            } else {
                house->addFloorTop(scene);
            }
        }
        Floor* tile;                                                        // generic tile used along calculations
        list<Floor*> entranceTiles = list<Floor*>();                        // all tiles belonging to the hall start
        list<Floor*> hallTiles = list<Floor*>();                            // all tiles belonging to the hall
        list<Floor*> roomTiles = list<Floor*>();                            // all tiles belonging to the current generated room
        hallPossibilities->clear();
        roomStartPosibilities->clear();
        roomPossibilities->clear();
        
        
        if (stairX >= 0) {
            pushAllHallAround(house, hallPossibilities, stairX, stairY);
            if (hallPossibilities->empty()) {
                // no more hall possibilies, invalid floor
                if (floorIndex > max / 2) {
                    house->removeFloorBottom();
                    break;
                } else {
                    house->removeFloorTop();
                    floorIndex = max / 2;
                    stairX = entranceX;
                    stairY = entranceY;
                    continue;
                }
            }
            // create stairs
            entranceTiles.push_back(house->getFloorTile(stairX, stairY)->setSelected(true));
            
        } else {
            // create a hall, either 1*1 or 2*2
            pushAllHallAround(house, hallPossibilities, entranceX, entranceY);
            if (largeEntrance) {
                // add extra tiles to entrance
                pushAllHallAround(house, hallPossibilities, entranceX + 1, entranceY);
                pushAllHallAround(house, hallPossibilities, entranceX, entranceY + 1);
                pushAllHallAround(house, hallPossibilities, entranceX + 1, entranceY + 1);
                entranceTiles.push_back(house->getFloorTile(entranceX, entranceY)->setSelected(true));
                entranceTiles.push_back(house->getFloorTile(entranceX + 1, entranceY)->setSelected(true));
                entranceTiles.push_back(house->getFloorTile(entranceX, entranceY + 1)->setSelected(true));
                entranceTiles.push_back(house->getFloorTile(entranceX + 1, entranceY + 1)->setSelected(true));
                
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
                entranceTiles.push_back(house->getFloorTile(entranceX, entranceY)->setSelected(true));
            }
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
        
        int curX, curY;
        FloorDirection::Direction dir = FloorDirection::NONE;
        tile = NULL;
        for (int i = 0; i < house->getWidth() * house->getHeight() / 6.2; i++) {
            if (hallPossibilities->empty()) {
                break;
            }
            id = -1;
            if (dir != FloorDirection::NONE) {
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
                    dir = FloorDirection::TOP;    // up, x, y - 1
                } else {
                    dir = FloorDirection::BOTTOM; // down, x, y + 1
                }
            } else {
                if (curX < prevX) {
                    dir = FloorDirection::LEFT;   // left, x - 1, y
                } else {
                    dir = FloorDirection::RIGHT;  // right, x + 1, y
                }
            }
            prevX = curX;
            prevY = curY;
            pushAllHallAround(house, hallPossibilities, curX, curY);
            
            tile = house->getFloorTile(id);
            hallTiles.push_back(tile);
        }
        if (!tile) {
            // no more hall tile could be added?
            if (floorIndex > max / 2) {
                house->removeFloorBottom();
                break;
            } else {
                house->removeFloorTop();
                floorIndex = max / 2;
                stairX = entranceX;
                stairY = entranceY;
                continue;
            }
        }
        tile->setSelected(true);
        
        // get all tiles next to hall tiles
        for (Floor * hallTile : hallTiles) {
            pushAllRoomAround(house, roomStartPosibilities, hallTile->getId());
        }
        list<Floor*> gaps = getGaps(house, roomStartPosibilities, 3);
        for (Floor * gap : gaps) {
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
				house->resumeTrigger();
				house->addRoom(RoomFactory::createRoomFromFloor(scene, house, roomTiles));
				house->pauseTrigger();
            } else {
                for (Floor * roomTile : roomTiles) {
                    roomTile->setSelected(false);
                    roomTile->setDoor(startTile.dir, false);
                }
            }
        }

		house->resumeTrigger();
        house->addRoom(RoomFactory::createRoomFromFloor(scene, house, entranceTiles, floorIndex ? (floorIndex > max / 2 ? Room::STAIRS_UP : Room::STAIRS_DOWN) : Room::ENTRANCE));
        house->addRoom(RoomFactory::createRoomFromFloor(scene, house, hallTiles, Room::HALL));
		house->pauseTrigger();
        
        // reset all floor tiles to default state
        for (int i = 0; i < house->getWidth() * house->getHeight(); i++) {
            tile = house->getFloorTile(i);
            if (tile) {
                house->getFloorTile(i)->setSelected(false);
            }
        }
        
        if (floorIndex) {
            // connect stairs on other floors
            if (floorIndex > max / 2) {
                house->floorUp();
            } else {
                house->floorDown();
            }
            
            tile = house->getFloorTile(stairX, stairY);
            Room* stair = tile->getRoom();
            if (stair->getRoomType() == Room::Type::STAIRS_UP || stair->getRoomType() == Room::Type::STAIRS_DOWN) {
                // change stair to double stair
                stair->setRoomType(Room::Type::STAIRS_UP_DOWN);
            } else {
                // create a new room and make it the right stair
                // remove stair tile from this room
                list<Floor*> erasedFloor = stair->getFloor();
                erasedFloor.remove(tile);
                stair->setFloor(erasedFloor);
                // and create a new room with just the stair tile
                erasedFloor = list<Floor*>();
                erasedFloor.push_back(tile);
				house->resumeTrigger();
                house->addRoom(RoomFactory::createRoomFromFloor(scene, house, erasedFloor, floorIndex > max / 2 ? Room::STAIRS_DOWN : Room::STAIRS_UP));
				house->pauseTrigger();
            }
            tile->updateMaterial();
        }
        largeEntrance = false;
        
        // select stair tile
        if (floorIndex != max / 2) {
            // get last placed hall tile and use it as stair. Keep it for later reference
            stairX = prevX;
            stairY = prevY;
            if (!floorIndex) {
                entranceX = prevX;
                entranceY = prevY;
            }
        } else {
            // Use the kept hall tile when switching between building up and building downs
            stairX = entranceX;
            stairY = entranceY;
        }
    }
    SAFE_DELETE(hallPossibilities);
    SAFE_DELETE(roomStartPosibilities);
    SAFE_DELETE(roomPossibilities);
    house->setFloorMiddle();
	house->resumeTrigger();
    return house;
}

void HouseFactory::setDoor(House* house, list<Floor*> tiles, WalledTile adjacent) {
    int searchId = house->getFloorIdInDirection(adjacent.id, adjacent.dir);
    for (Floor * tile : tiles) {
        if (tile->getId() == searchId) {
            tile->setDoor(FloorDirection::invert(adjacent.dir), true);
            break;
        }
    }
}

void HouseFactory::clearAllAround(House* house, vector<WalledTile>* ids, int startId) {
    // clear all the possibilities around this newly generated tile
    for (vector<WalledTile>::iterator itr = ids->begin(); itr != ids->end();) {
        int id = (*itr).id;
        if (id == startId || id == house->getFloorIdInDirection(startId, FloorDirection::LEFT) || id == house->getFloorIdInDirection(startId, FloorDirection::TOP) ||
                id == house->getFloorIdInDirection(startId, FloorDirection::RIGHT) || id == house->getFloorIdInDirection(startId, FloorDirection::BOTTOM)) {
            itr = ids->erase(itr);
        } else {
            itr++;
        }
    }
}

void HouseFactory::pushAllHallAround(House* house, vector<WalledTile>* ids, int x, int y) {
    // only non occupied tiles with no side next to them can be made
    // also no hallway tile may touch another occupied tile
    Floor* floor = house->getFloorInDirection(x, y, FloorDirection::LEFT);
    if (canBeHallway(house, floor)) {
        ids->push_back(WalledTile(floor->getId(), FloorDirection::RIGHT));
    }
    floor = house->getFloorInDirection(x, y, FloorDirection::BOTTOM);
    if (canBeHallway(house, floor)) {
        ids->push_back(WalledTile(floor->getId(), FloorDirection::TOP));
    }
    floor = house->getFloorInDirection(x, y, FloorDirection::RIGHT);
    if (canBeHallway(house, floor)) {
        ids->push_back(WalledTile(floor->getId(), FloorDirection::LEFT));
    }
    floor = house->getFloorInDirection(x, y, FloorDirection::TOP);
    if (canBeHallway(house, floor)) {
        ids->push_back(WalledTile(floor->getId(), FloorDirection::BOTTOM));
    }
}

bool HouseFactory::canBeHallway(House* house, Floor* tile) {
    return tile != NULL && !tile->getSelected() && !floorTouchesSide(house, tile->getId()) && !floorHasNeighbours(house, tile->getId());
}

bool HouseFactory::floorTouchesSide(House* house, int id) {
    return !(house->getFloorInDirection(id, FloorDirection::TOP) && house->getFloorInDirection(id, FloorDirection::LEFT) &&
             house->getFloorInDirection(id, FloorDirection::BOTTOM) && house->getFloorInDirection(id, FloorDirection::RIGHT) &&
             house->getFloorTile(id));
}

bool HouseFactory::floorHasNeighbours(House* house, int id) {
    Floor* floor = house->getFloorInDirection(id, FloorDirection::LEFT);
    if (floor && floor->getSelected()) {
        return true;
    }
    floor = house->getFloorInDirection(id, FloorDirection::TOP);
    if (floor && floor->getSelected()) {
        return true;
    }
    floor = house->getFloorInDirection(id, FloorDirection::RIGHT);
    if (floor && floor->getSelected()) {
        return true;
    }
    floor = house->getFloorInDirection(id, FloorDirection::BOTTOM);
    if (floor && floor->getSelected()) {
        return true;
    }
    return false;
}

void HouseFactory::pushAllRoomAround(House* house, vector<int>* ids, int id) {
    // all non-occupied tiles next to this one can be room
    Floor* floor = house->getFloorInDirection(id, FloorDirection::LEFT);
    if (canBeRoom(floor)) {
        ids->push_back(floor->getId());
    }
    floor = house->getFloorInDirection(id, FloorDirection::BOTTOM);
    if (canBeRoom(floor)) {
        ids->push_back(floor->getId());
    }
    floor = house->getFloorInDirection(id, FloorDirection::RIGHT);
    if (canBeRoom(floor)) {
        ids->push_back(floor->getId());
    }
    floor = house->getFloorInDirection(id, FloorDirection::TOP);
    if (canBeRoom(floor)) {
        ids->push_back(floor->getId());
    }
}

void HouseFactory::pushAllRoomAround(House* house, vector<WalledTile>* ids, int id) {
    // all non-occupied tiles next to this one can be room
    Floor* floor = house->getFloorInDirection(id, FloorDirection::LEFT);
    if (canBeRoom(floor)) {
        ids->push_back(WalledTile(floor->getId(), FloorDirection::RIGHT));
    }
    floor = house->getFloorInDirection(id, FloorDirection::BOTTOM);
    if (canBeRoom(floor)) {
        ids->push_back(WalledTile(floor->getId(), FloorDirection::TOP));
    }
    floor = house->getFloorInDirection(id, FloorDirection::RIGHT);
    if (canBeRoom(floor)) {
        ids->push_back(WalledTile(floor->getId(), FloorDirection::LEFT));
    }
    floor = house->getFloorInDirection(id, FloorDirection::TOP);
    if (canBeRoom(floor)) {
        ids->push_back(WalledTile(floor->getId(), FloorDirection::BOTTOM));
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