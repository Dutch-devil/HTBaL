#include "Room.h"

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

        int nextTile = house->getIdByXY(house->getXById(i)-1,house->getYById(i));
        if (nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(new Wall(new Vector2(floorTile->getX() - Floor::getWidth() / 2, floorTile->getY() - Floor::getHeight() / 2),
                                     new Vector2(floorTile->getX() - Floor::getWidth() / 2, floorTile->getY() + Floor::getHeight() / 2), floorTile->getDoor() == LEFT));
        }

        nextTile = house->getIdByXY(house->getXById(i),house->getYById(i)+1);
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
			walls.push_back(new Wall(new Vector2(floorTile->getX() - Floor::getWidth() / 2, floorTile->getY() + Floor::getHeight() / 2),
                                     new Vector2(floorTile->getX() + Floor::getWidth() / 2, floorTile->getY() + Floor::getHeight() / 2), floorTile->getDoor() == TOP));
        }

        nextTile = house->getIdByXY(house->getXById(i)+1,house->getYById(i));
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(new Wall(new Vector2(floorTile->getX() + Floor::getWidth() / 2, floorTile->getY() + Floor::getHeight() / 2),
                                     new Vector2(floorTile->getX() + Floor::getWidth() / 2, floorTile->getY() - Floor::getHeight() / 2), floorTile->getDoor() == RIGHT));
        }

        nextTile = house->getIdByXY(house->getXById(i),house->getYById(i)-1);
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(new Wall(new Vector2(floorTile->getX() + Floor::getWidth() / 2, floorTile->getY() - Floor::getHeight() / 2),
                                     new Vector2(floorTile->getX() - Floor::getWidth() / 2, floorTile->getY() - Floor::getHeight() / 2), floorTile->getDoor() == BOTTOM));
        }
    }

    for (Wall* wall : walls) {
        Node* wallNode = scene->addNode();
        wallNode->setModel(wall->getModel());
		SAFE_RELEASE(wallNode);
    }

	SAFE_DELETE_ARRAY(floorTiles);
    return new Room(0, 0, walls);
}
