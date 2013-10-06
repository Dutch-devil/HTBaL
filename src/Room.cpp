#include "Room.h"

Room::Room(list<Wall*> walls): walls(walls) {

}


Room::~Room(void) {
    for (Wall* wall : walls) {
		SAFE_DELETE(wall);
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

        int nextTile = house->getIdByXY(house->getXById(i) - 1, house->getYById(i));
        if (nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(createWall(floorTile->getDoor(LEFT), scene, floorTile, MATH_PI));
        }

        nextTile = house->getIdByXY(house->getXById(i), house->getYById(i) + 1);
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(createWall(floorTile->getDoor(TOP), scene, floorTile, MATH_PI / 2));
        }

        nextTile = house->getIdByXY(house->getXById(i) + 1, house->getYById(i));
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(createWall(floorTile->getDoor(RIGHT), scene, floorTile, 0));
        }

        nextTile = house->getIdByXY(house->getXById(i), house->getYById(i) - 1);
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(createWall(floorTile->getDoor(BOTTOM), scene, floorTile, -MATH_PI / 2));
        }
    }

    SAFE_DELETE_ARRAY(floorTiles);
    return new Room(walls);
}

Wall* Room::createWall(bool door, Scene* scene, Floor* floorTile, float rot) {
    Wall* wall = new Wall(door);
    Node* wallNode = scene->addNode();
	floorTile->getModel()->getNode()->addChild(wallNode);
    wallNode->rotateZ(rot);
	Model* model = wall->getModel();
    wallNode->setModel(model);
	return wall;
}


