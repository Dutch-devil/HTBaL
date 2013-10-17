#include "Room.h"

Room::Room(House* house, list<Floor*> floor, list<Wall*> walls): house(house), floor(floor), walls(walls), roomType(ROOM_EMPTY) {
    for (Floor * floorTile : floor) {
        floorTile->setRoom(this);
    }
}

Room::Room(House* house, list<Floor*> floor, list<Wall*> walls, Room::Type roomType): house(house), floor(floor), walls(walls), roomType(roomType) {
    for (Floor * floorTile : floor) {
        floorTile->setRoom(this);
    }
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

int Room::getMaxRectangle() {
    return -1;
}

int Room::getMaxLine() {
    list<Floor*> corners = getCorners();
    unsigned int maxLine = 0;
    for (Floor * corner : corners) {
        for (int dir = FloorDirection::TOP; dir <= FloorDirection::RIGHT; dir++) {
            maxLine = max(maxLine, (!corner->getWall((FloorDirection::Direction)dir) ? getLine(corner, (FloorDirection::Direction)dir).size() : 1));
        }
    }
    return maxLine;
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