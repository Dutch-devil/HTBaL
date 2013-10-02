#include "Room.h"

using namespace std;

Room::Room(int x, int y, list<Vector2*> walls) : x(x), y(y), walls(walls) {

}


Room::~Room(void) {
}

list<Vector2*> Room::getWalls() {
	return walls;
}

//Room* Room::createRoomFromFloor(Scene* scene, House* house, RenderState::StateBlock* stateBlock, Floor** floorTiles, int tileCount) {
//	float startX = -(float)house->getWidth() / 2 * Floor::width;
//    float startY = -(float)house->getHeight() / 2 * Floor::height;
//    list<Wall*> walls;
//
//	for (; tileCount > 0; tileCount--) {
//		Floor* floorTile = *floorTiles;
//		floorTile->setColor(FLOOR_BLUE);
//		floorTiles++;
//
//		walls.push_back(new Wall(stateBlock, new Vector2(floorTile->getX() - Floor::width / 2, floorTile->getY() - Floor::height / 2), 
//											 new Vector2(floorTile->getX() - Floor::width / 2, floorTile->getY() + Floor::height / 2)));
//
//		walls.push_back(new Wall(stateBlock, new Vector2(floorTile->getX() - Floor::width / 2, floorTile->getY() + Floor::height / 2), 
//											 new Vector2(floorTile->getX() + Floor::width / 2, floorTile->getY() + Floor::height / 2)));
//
//		walls.push_back(new Wall(stateBlock, new Vector2(floorTile->getX() + Floor::width / 2, floorTile->getY() + Floor::height / 2), 
//											 new Vector2(floorTile->getX() + Floor::width / 2, floorTile->getY() - Floor::height / 2)));
//
//		walls.push_back(new Wall(stateBlock, new Vector2(floorTile->getX() + Floor::width / 2, floorTile->getY() - Floor::height / 2), 
//											 new Vector2(floorTile->getX() - Floor::width / 2, floorTile->getY() - Floor::height / 2)));
//	}
//
//
//    for (list<Wall*>::iterator itr = walls.begin(); itr != walls.end();) {
//		Wall* duplicate = getDuplicateWall(*itr, walls);
//		if (duplicate == NULL) {
//			itr++;
//			continue;
//		}
//		walls.remove(duplicate);
//		itr = walls.erase(itr);
//    }
//   /* house->addRoom(Room(0, 0, walls));
//
//    walls = list<Vector2*>();
//
//    for (int x = 0; x < house->getWidth() + 1 ; x++) {
//        walls.push_back(new Vector2(startX + Floor::width * x, -startY - Floor::height));
//    }
//    house->addRoom(Room(0, 0, walls));
//
//    for (Room room : house->getRooms()) {
//        Vector2* prevWall = NULL;
//        for (Vector2* wall : room.getWalls()) {
//            if (prevWall == NULL) {
//                prevWall = wall;
//                continue;
//            }
//			
//            Node* wallNode = scene->addNode();
//            wallNode->setModel(wallModel);
//            wallModels.push_back(wallModel);
//
//            prevWall = wall;
//        }
//    }*/
//
//
//	return NULL;
//}



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
