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
    ~Room(void);
	
	static Room* createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles);
	list<Wall*> getWalls();

protected:
	Room(int, int, list<Wall*>, Scene* scene);
	static Wall* createWall(bool door, Scene* scene, float x, float y, float rot);

	static int i;
	int x;
	int y;
	list<Wall*> walls;
	Scene* scene;
};

#endif