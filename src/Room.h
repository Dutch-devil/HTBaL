#ifndef Room_H_
#define Room_H_

class Room;

#include <list>
#include "gameplay.h"
#include "Floor.h"
#include "Wall.h"
#include "House.h"

using namespace std;
using namespace gameplay;

class Room {
public:
	Room(list<Wall*>);
    ~Room();

	list<Wall*> getWalls();
	
	static Room* createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles);

protected:
	list<Wall*> walls;
	
	static Wall* createWall(bool door, Scene* scene, Floor* floorTile, float rot);
};

#endif