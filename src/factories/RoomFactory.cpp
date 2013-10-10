#include "RoomFactory.h"

Room* RoomFactory::createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles) {
	return createRoomFromFloor(scene, house, roomTiles, Room::Type::ROOM_EMPTY);
}

Room* RoomFactory::createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles, Room::Type roomType) {
    Floor** floorTiles = new Floor*[house->getWidth() * house->getHeight()];
    memset(floorTiles, NULL, sizeof(Floor*) * house->getWidth() * house->getHeight());

    for (Floor* floor : roomTiles) {
        floorTiles[floor->getId()] = floor;
    }

    list<Wall*> walls;

    for (Floor* floorTile : roomTiles) {
        int i = floorTile->getId();

		Floor* floor = house->getFloorInDirection(i, Floor::Direction::LEFT);
        if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, Floor::Direction::LEFT));
        }

		floor = house->getFloorInDirection(i, Floor::Direction::TOP);
		if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, Floor::Direction::TOP));
        }

		floor = house->getFloorInDirection(i, Floor::Direction::RIGHT);
		if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, Floor::Direction::RIGHT));
        }

		floor = house->getFloorInDirection(i, Floor::Direction::BOTTOM);
		if (!floor || !floorTiles[floor->getId()]) {
            walls.push_back(createWall(scene, floorTile, Floor::Direction::BOTTOM));
        }
    }

    SAFE_DELETE_ARRAY(floorTiles);
    return new Room(house, roomTiles, walls, roomType);
}

Wall* RoomFactory::createWall(Scene* scene, Floor* floorTile, Floor::Direction dir) {
	floorTile->setWall(dir, true);
    Wall* wall = new Wall(!(dir == Floor::Direction::LEFT || dir == Floor::Direction::BOTTOM), floorTile->getDoor(dir));
    Node* wallNode = scene->addNode();
	floorTile->getModel()->getNode()->addChild(wallNode);
	switch (dir) {
		case Floor::Direction::LEFT: wallNode->rotateZ(MATH_PI); break;
		case Floor::Direction::BOTTOM: wallNode->rotateZ(MATH_PI / 2); break;
		case Floor::Direction::TOP: wallNode->rotateZ(-MATH_PI / 2); break;
	}
	Model* model = wall->getModel();
    wallNode->setModel(model);
	return wall;
}