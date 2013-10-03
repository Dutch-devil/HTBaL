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
	Room(int, int, list<Wall*>);
    ~Room(void);

	list<Wall*> getWalls();
	
	static Room* createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles);

protected:
	int x;
	int y;
	list<Wall*> walls;
};

#endif