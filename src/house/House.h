#ifndef House_H_
#define House_H_

class House;

#include "Floor.h"
#include <list>
#include <cmath>
#include "../MaterialManager.h"
#include "Wall.h"
#include "Room.h"

using namespace std;

class House {
public:
	House(int, int);
    ~House(void);
	void addFloor(Scene* scene, float screenSize);

	void addRoom(Room* room);

	int getWidth();
	int getHeight();

	int getIdByXY(int x, int y);
	int getXById(int id);
	int getYById(int id);

	Floor** getFloorTiles();
	Floor* getFloorTile(int id);
	Floor* getFloorTile(int x, int y);
	Floor* getFloorInDirection(int id, Floor::Direction dir);
	Floor* getFloorInDirection(int x, int y, Floor::Direction dir);
	int getFloorIdInDirection(int id, Floor::Direction dir);
	int getFloorIdInDirection(int x, int y, Floor::Direction dir);

	list<Room*> getRooms();
	Room* getRoom(Floor* tile);

protected:
	int height;
	int width;
	list<Room*> rooms;
	Floor** floorTiles;
};

#endif