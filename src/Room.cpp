#include "Room.h"

Room::Room(list<Wall*> walls): walls(walls) {}

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

list<Wall*> Room::getWalls() {
    return walls;
}

Room* Room::createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles) {
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
    return new Room(walls);
}

Wall* Room::createWall(Scene* scene, Floor* floorTile, Direction dir) {
    Wall* wall = new Wall(floorTile->getDoor(dir));
    Node* wallNode = scene->addNode();
	floorTile->getModel()->getNode()->addChild(wallNode);
	switch (dir) {
		case LEFT: wallNode->rotateZ(MATH_PI); break;
		case BOTTOM: wallNode->rotateZ(MATH_PI / 2); break;
		case TOP: wallNode->rotateZ(-MATH_PI / 2); break;
	}
	Model* model = wall->getModel();
    wallNode->setModel(model);
}


