#include "Room.h"

int Room::i = 0;

Room::Room(int x, int y, list<Wall*> walls) : x(x), y(y), walls(walls) {

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
            walls.push_back(createWall(floorTile->getDoor() == LEFT, scene, floorTile->getX(), floorTile->getY(), MATH_PI));
        }

        nextTile = house->getIdByXY(house->getXById(i), house->getYById(i) + 1);
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(createWall(floorTile->getDoor() == TOP, scene, floorTile->getX(), floorTile->getY(), MATH_PI / 2));
        }

        nextTile = house->getIdByXY(house->getXById(i) + 1, house->getYById(i));
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(createWall(floorTile->getDoor() == RIGHT, scene, floorTile->getX(), floorTile->getY(), 0));
        }

        nextTile = house->getIdByXY(house->getXById(i), house->getYById(i) - 1);
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(createWall(floorTile->getDoor() == BOTTOM, scene, floorTile->getX(), floorTile->getY(), -MATH_PI / 2));
        }
    }

    SAFE_DELETE_ARRAY(floorTiles);
    return new Room(0, 0, walls);
}

Wall* Room::createWall(bool door, Scene* scene, float x, float y, float rot) {
    Wall* wall = new Wall(door);
	char* buf = new char[30];
	sprintf(buf, "wall %d", i++);
    Node* wallNode = scene->addNode(buf);
	delete[] buf;
    wallNode->translateX(x);
    wallNode->translateY(y);
    wallNode->rotateZ(rot);
    wallNode->setModel(wall->getModel());
	//SAFE_RELEASE(wallNode);
	return wall;
}


