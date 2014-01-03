#include "Room.h"

Room::Room(House* house, list<Floor*> floor, list<Wall*> walls) {
    Room::Room(house, floor, walls, ROOM_EMPTY);
}

Room::Room(House* house, list<Floor*> floor, list<Wall*> walls, Room::Type roomType): house(house), floor(floor), walls(walls), roomType(roomType) {
    for (Floor * floorTile : floor) {
        floorTile->setRoom(this);
    }
    
    maxLine = -1;
    maxSquareId = -1;
    maxRectangleId = -1;
    maxAreaSquare = -1;
    maxAreaRect = -1;
    maxRectDir = UNDEF;
}

Room::~Room(void) {
    if (!walls.empty()) {
        Scene* scene = walls.back()->getNode()->getScene();
        while (!walls.empty()) {
            scene->removeNode(walls.back()->getNode());
            SAFE_DELETE(walls.back());
            walls.pop_back();
        }
    }
}

Room::Type Room::getRoomType() {
    return roomType;
}

void Room::setRoomType(Room::Type roomType) {
    if (this->roomType != roomType) {
        this->roomType = roomType;
        for (Floor * floor : getFloor()) {
            floor->updateMaterial();
        }
    }
}

const char* Room::getRoomTypeString() {
    return getRoomTypeString(roomType);
}

const char* Room::getRoomTypeString(Room::Type roomType) {
    switch (roomType) {
        case LIVING_QUARTERS:
            return "Living quarters";
        case BEDROOM:
            return "Bedroom";
        case DINING_ROOM:
            return "Dining room";
        case STORAGE_ROOM:
            return "Storage room";
        case LIBARY:
            return "Library";
        case LABORATORIUM:
            return "Laboratorium";
        case ARBORETUM:
            return "Arboretum";
        case WORKSHOP:
            return "Workshop";
        case SMITHY:
            return "Smithy";
        case STINKY_SEWERS:
            return "Stinky sewers";
        case DIRTPLOT:
            return "Dirtplot";
        case STABLES:
            return "Stables";
        case ARSENAL_ROOM:
            return "Arsenal room";
        case AVIARY:
            return "Aviary";
        case POOL:
            return "Pool";
        case ART_ROOM:
            return "Art room";
        case BALLROOM:
            return "Ballroom";
        case ENTRANCE:
            return "Entrance";
        case STAIR_UP:
            return "Stairs up";
        case STAIR_DOWN:
            return "Stairs down";
        case STAIR_UP_DOWN:
            return "Stairs up and down";
        case HALL:
            return "Hall";
        case ROOM_EMPTY:
            return "Empty room";
        default:
            GP_WARN("No valid room");
            return "None";
    }
}

list<Floor*> Room::getFloor() {
    return floor;
}

void Room::setFloor(list<Floor*> floor) {
    for (Floor * floorTile : this->floor) {
        floorTile->setRoom(NULL);
    }
    this->floor = floor;
    for (Floor * floorTile : this->floor) {
        floorTile->setRoom(this);
    }
}

list<Wall*> Room::getWalls() {
    return walls;
}

bool Room::contains(Floor* floorTile) {
    for (Floor * tile : floor) {
        if (tile == floorTile) {
            return true;
        }
    }
    return false;
}

int Room::getSize() {
    return floor.size();
}

list<Floor*> Room::getCorners() {
    list<Floor*> corners;
    for (Floor * floorTile : floor) {
        if (floorTile->getWall(FloorDirection::LEFT) + floorTile->getWall(FloorDirection::TOP) + floorTile->getWall(FloorDirection::RIGHT) + floorTile->getWall(FloorDirection::BOTTOM) >= 2) {
            // 2 or more walls, it's a corner
            corners.push_back(floorTile);
        }
    }
    return corners;
}


list<Floor*> Room::getLine(Floor* startTile, FloorDirection::Direction dir) {
    return getLine(startTile, dir, 0);
}

list<Floor*> Room::getLine(Floor* startTile, FloorDirection::Direction dir, unsigned int maxLength) {
    list<Floor*> line;
    line.push_back(startTile);
    Floor* nextTile = startTile;
    while (--maxLength && !nextTile->getWall(dir)) {
        nextTile = house->getFloorInDirection(nextTile->getId(), dir);
        line.push_back(nextTile);
    }
    return line;
}


void Room::calculateSize() {
    maxLine = 0;
    bool* tiles = new bool[house->getWidth() * house->getHeight()];
    memset(tiles, false, sizeof(bool) * house->getWidth() * house->getHeight());
    
    maxAreaRect = 0;
    maxAreaSquare = 0;
    
    for (Floor * tile : floor) {
        tiles[tile->getId()] = true;
    }
    
    list<Floor*> corners = getCorners();
    for (Floor * corner : corners) {
        for (int dir = FloorDirection::TOP; dir <= FloorDirection::RIGHT; dir++) {
            maxLine = max((unsigned int)maxLine, (!corner->getWall((FloorDirection::Direction)dir) ? getLine(corner, (FloorDirection::Direction)dir).size() : 1));
        }
    }
    
    for (Floor * tile : floor) {
        int x = house->getXById(tile->getId());
        int y = house->getYById(tile->getId());
        int i = 0;
        
        
        // Loop until square is disrupted
        bool keepLooping = true;
        while (keepLooping) {
            //expand corner first
            int next = house->getIdByXY(x + i, y + i);
            if (next == -1 || !tiles[next]) {
                break;
            }
            
            //Then check the edges
            for (int k = 0; k < i; k++) {
                //in both the x
                int next = house->getIdByXY(x + i, y + k);
                if (next == -1 || !tiles[next]) {
                    keepLooping = false;
                    break;
                }
                // and the y direction
                next = house->getIdByXY(x + k, y + i);
                if (next == -1 || !tiles[next]) {
                    keepLooping = false;
                    break;
                }
            }
            // if the square is succesfully expanded, calculate new maxsize
            if (keepLooping) {
                i++;
                if (i * i > maxAreaSquare) {
                    maxAreaSquare = i * i;
                    maxSquareId = tile->getId();
                    if (maxAreaSquare > maxAreaRect) {
                        maxAreaRect = maxAreaSquare;
                        maxRectangleId = maxSquareId;
                        maxRectDir = XDIR;
                    }
                }
            }
        }
        
        {
            // expand the square into a rectangle in the x direction
            int maxY = i;
            int iX = i;
            keepLooping = true;
            while (keepLooping) {
                for (int k = 0; k < maxY; k++) {
                    int next = house->getIdByXY(x + iX, y + k);
                    if (next == -1 || !tiles[next]) {
                        maxY = k;
                        if (k == 0) {
                            keepLooping = false;
                        }
                        break;
                        break;
                    }
                }
                //if succeded, update max
                if (keepLooping) {
                    iX++;
                    if (iX * maxY > maxAreaRect) {
                        maxAreaRect = iX * maxY;
                        maxRectangleId = tile->getId();
                        maxRectDir = XDIR;
                    }
                }
            }
        }
        
        {
            // expand the square into a rectangle in the y direction
            int maxX = i;
            int iY = i;
            keepLooping = true;
            while (keepLooping) {
                for (int k = 0; k < maxX; k++) {
                    int next = house->getIdByXY(x + k, y + iY);
                    if (next == -1 || !tiles[next]) {
                        maxX = k;
                        if (k == 0) {
                            keepLooping = false;
                        }
                        break;
                    }
                }
                //if succeded, update max
                if (keepLooping) {
                    iY++;
                    if (iY * maxX > maxAreaRect) {
                        maxAreaRect = iY * maxX;
                        maxRectangleId = tile->getId();
                        maxRectDir = YDIR;
                    }
                }
            }
        }
    }
    
    SAFE_DELETE_ARRAY(tiles);
}

int Room::getMaxLine() {
    if (maxLine == -1) {
        calculateSize();
    }
    return maxLine;
}

int Room::getMaxSquareId() {
    if (maxSquareId == -1) {
        calculateSize();
    }
    return maxSquareId;
}

int Room::getMaxRectangleId() {
    if (maxRectangleId == -1) {
        calculateSize();
    }
    return maxRectangleId;
}

int Room::getMaxAreaSquare() {
    if (maxAreaSquare == -1) {
        calculateSize();
    }
    return maxAreaSquare;
}

int Room::getMaxAreaRect() {
    if (maxAreaRect == -1) {
        calculateSize();
    }
    return maxAreaRect;
}

Room::Direction Room::getMaxRectDir() {
    if (maxRectDir == NONE) {
        calculateSize();
    }
    return maxRectDir;
}
