#ifndef Room_H_
#define Room_H_

#include "House.h"
#include "Floor.h"
#include "Wall.h"

#include <list>
#include "gameplay.h"

using namespace std;
using namespace gameplay;

class Room {
public:
	House* mock;
	Room(int, int, list<Vector2*>);
    ~Room(void);

	list<Vector2*> getWalls();

	//Room* createRoomFromFloor(Scene* scene, House* house, RenderState::StateBlock* stateBlock, Floor** floorTiles, int tileCount);
	Wall* getDuplicateWall(Wall* wall, list<Wall*> walls);

protected:
	int x;
	int y;
	list<Vector2*> walls;
};

#endif