#include "Room.h"

Room::Room(int x, int y, list<Wall*> walls) : x(x), y(y), walls(walls) {

}


Room::~Room(void) {
}

list<Wall*> Room::getWalls() {
    return walls;
}

Room* Room::createRoomFromFloor(Scene* scene, House* house, RenderState::StateBlock* stateBlock, list<Floor*> roomTiles) {
    Floor** floorTiles = new Floor*[house->getWidth() * house->getHeight()];
    memset(floorTiles, NULL, sizeof(Floor*) * house->getWidth() * house->getHeight());

    for (Floor* floor : roomTiles) {
        floorTiles[floor->getId()] = floor;
    }

    list<Wall*> walls;

    for (Floor* floorTile : roomTiles) {
		int i = floorTile->getId();
        floorTile->setColor(FLOOR_BLUE);

        int nextTile = house->getIdByXY(house->getXById(i)-1,house->getYById(i));
        if (nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(new Wall(stateBlock, new Vector2(floorTile->getX() - Floor::width / 2, floorTile->getY() - Floor::height / 2),
                                     new Vector2(floorTile->getX() - Floor::width / 2, floorTile->getY() + Floor::height / 2)));
        }

        nextTile = house->getIdByXY(house->getXById(i),house->getYById(i)+1);
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(new Wall(stateBlock, new Vector2(floorTile->getX() - Floor::width / 2, floorTile->getY() + Floor::height / 2),
                                     new Vector2(floorTile->getX() + Floor::width / 2, floorTile->getY() + Floor::height / 2)));
        }

        nextTile = house->getIdByXY(house->getXById(i)+1,house->getYById(i));
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(new Wall(stateBlock, new Vector2(floorTile->getX() + Floor::width / 2, floorTile->getY() + Floor::height / 2),
                                     new Vector2(floorTile->getX() + Floor::width / 2, floorTile->getY() - Floor::height / 2)));
        }

        nextTile = house->getIdByXY(house->getXById(i),house->getYById(i)-1);
        if(nextTile == -1 || floorTiles[nextTile] == NULL) {
            walls.push_back(new Wall(stateBlock, new Vector2(floorTile->getX() + Floor::width / 2, floorTile->getY() - Floor::height / 2),
                                     new Vector2(floorTile->getX() - Floor::width / 2, floorTile->getY() - Floor::height / 2)));
        }
    }

    for (Wall* wall : walls) {
        Node* wallNode = scene->addNode();
        wallNode->setModel(wall->getModel());
    }

    return new Room(0, 0, walls);
}



Wall* Room::getDuplicateWall(Wall* wall, list<Wall*> walls) {
    for (Wall* other : walls) {
        // if walls are flipped against eachother, they are duplicates for the same item
        if (wall->getStart()->x == other->getEnd()->x && wall->getStart()->y == other->getEnd()->y &&
                wall->getEnd()->x == other->getStart()->x && wall->getEnd()->y == other->getStart()->y) {
            return other;
        }
    }
    return NULL;
}
