#include "Room.h"

int Room::i = 0;

Room::Room(int x, int y, list<Wall*> walls, Scene* scene) : x(x), y(y), walls(walls), scene(scene) {
	scene->addRef();
}


Room::~Room(void) {
    while(!walls.empty()) {
		scene->removeNode(walls.back()->getNode());
        SAFE_DELETE(walls.back());
		walls.pop_back();
    }
	SAFE_RELEASE(scene);
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
    return new Room(0, 0, walls, scene);
}

Wall* Room::createWall(bool door, Scene* scene, float x, float y, float rot) {
	char* buf = new char[30];
	sprintf(buf, "wall %d", i++);
    Node* wallNode = scene->addNode(buf);
	Wall* wall = new Wall(wallNode, door);
	delete[] buf;
    wallNode->translateX(x);
    wallNode->translateY(y);
    wallNode->rotateZ(rot);
    wallNode->setModel(wall->getModel());
	return wall;
}


