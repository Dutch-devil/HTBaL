#ifndef Room_H_
#define Room_H_

class Room;

#include <list>

using namespace std;
#include "gameplay.h"
using namespace gameplay;

#include "Floor.h"
#include "Wall.h"
#include "House.h"

class Room {
public:
	Room(int, int, list<Wall*>);
    ~Room(void);

	list<Wall*> getWalls();

	static Room* createRoomFromFloor(Scene* scene, House* house, RenderState::StateBlock* stateBlock, Floor** floorTiles, int tileCount);
	static Wall* getDuplicateWall(Wall* wall, list<Wall*> walls);

protected:
	int x;
	int y;
	list<Wall*> walls;
};

#endif