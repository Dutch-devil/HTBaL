#include "Room.h"

Room::Room(House* house, list<Floor*> floor, list<Wall*> walls): house(house), floor(floor), walls(walls), roomType(ROOM_EMPTY) {}
Room::Room(House* house, list<Floor*> floor, list<Wall*> walls, Room::Type roomType): house(house), floor(floor), walls(walls), roomType(roomType) {}

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
	this->roomType = roomType;
}

const char* Room::getRoomTypeString() {
	return getRoomTypeString(roomType);
}

const char* Room::getRoomTypeString(Room::Type roomType) {
	switch (roomType) {
		case LIVING_QUARTERS: return "Living quarters";
		case BEDROOM: return "Bedroom";
		case DINING_ROOM: return "Dining room";
		case STORAGE_ROOM: return "Storage room";
		case LIBARY: return "Library";
		case LABORATORIUM: return "Laboratorium";
		case ARBORETUM: return "Arboretum";
		case WORKSHOP: return "Workshop";
		case SMITHY: return "Smithy";
		case STINKY_SEWERS: return "Stinky sewers";
		case DIRTPLOT: return "Dirtplot";
		case STABLES: return "Stables";
		case ARSENAL_ROOM: return "Arsenal room";
		case AVIARY: return "Aviary";
		case POOL: return "Pool";
		case ART_ROOM: return "Art room";
		case BALLROOM: return "Ballroom";
		case ENTRANCE: return "Entrance";
		case HALL: return "Hall";
		case ROOM_EMPTY: return "Empty room";
		default: 
			GP_WARN("No valid room");
			return "None";
	}
}

list<Floor*> Room::getFloor() {
	return floor;
}

list<Wall*> Room::getWalls() {
    return walls;
}

bool Room::contains(Floor* floorTile) {
	for (Floor* tile : floor) {
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
	for (Floor* corner : corners) {
		for (int dir = TOP; dir <= RIGHT; dir++) {
			maxLine = max(maxLine, (!corner->getWall((Direction)dir)?getLine(corner, (Direction)dir).size():1));
		}
	}
	return maxLine;
}

list<Floor*> Room::getCorners() {
	list<Floor*> corners;
	for (Floor* floorTile : floor) {
		if (floorTile->getWall(LEFT) + floorTile->getWall(TOP) + floorTile->getWall(RIGHT) + floorTile->getWall(BOTTOM) >= 2) {
			// 2 or more walls, it's a corner
			corners.push_back(floorTile);
		}
	}
	return corners;
}


list<Floor*> Room::getLine(Floor* startTile, Direction dir) {
	return getLine(startTile, dir, 0);
}

list<Floor*> Room::getLine(Floor* startTile, Direction dir, unsigned int maxLength) {
	list<Floor*> line;
	line.push_back(startTile);
	Floor* nextTile = startTile;
	while (--maxLength && !nextTile->getWall(dir)) {
		nextTile = house->getFloorInDirection(nextTile->getId(), dir);
		line.push_back(nextTile);
	}
	return line;
}



Room* Room::createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles) {
	return createRoomFromFloor(scene, house, roomTiles, ROOM_EMPTY);
}

Room* Room::createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles, Room::Type roomType) {
    Floor** floorTiles = new Floor*[house->getWidth() * house->getHeight()];
    memset(floorTiles, NULL, sizeof(Floor*) * house->getWidth() * house->getHeight());

    for (Floor* floor : roomTiles) {
        floorTiles[floor->getId()] = floor;
    }

    list<Wall*> walls;

    for (Floor* floorTile : roomTiles) {
        int i = floorTile->getId();

		Floor* floor = house->getFloorInDirection(i, LEFT);
        if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, LEFT));
        }

		floor = house->getFloorInDirection(i, TOP);
		if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, TOP));
        }

		floor = house->getFloorInDirection(i, RIGHT);
		if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, RIGHT));
        }

		floor = house->getFloorInDirection(i, BOTTOM);
		if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, BOTTOM));
        }
    }

    SAFE_DELETE_ARRAY(floorTiles);
    return new Room(house, roomTiles, walls, roomType);
}

Wall* Room::createWall(Scene* scene, Floor* floorTile, Direction dir) {
	floorTile->setWall(dir, true);
    Wall* wall = new Wall(!(dir == LEFT || dir == BOTTOM), floorTile->getDoor(dir));
    Node* wallNode = scene->addNode();
	floorTile->getModel()->getNode()->addChild(wallNode);
	switch (dir) {
		case LEFT: wallNode->rotateZ(MATH_PI); break;
		case BOTTOM: wallNode->rotateZ(MATH_PI / 2); break;
		case TOP: wallNode->rotateZ(-MATH_PI / 2); break;
	}
	Model* model = wall->getModel();
    wallNode->setModel(model);
	return wall;
}


