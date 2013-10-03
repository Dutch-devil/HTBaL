#ifndef House_H_
#define House_H_

class House;

#include <list>
#include <cmath>
#include "Room.h"

using namespace std;

class House {
public:
	House(int, int);
    ~House(void);
	void addFloor(Scene* scene, RenderState::StateBlock* stateBlock, float screenSize);

	void addRoom(Room* room);

	int getWidth();
	int getHeight();

	int getIdByXY(int x, int y);
	int getXById(int id);
	int getYById(int id);

	Floor** getFloorTiles();
	Floor* getFloorTile(int id);
	Floor* getFloorTile(int x, int y);

	list<Room*> getRooms();
	House* addRandomRooms(Scene* scene, RenderState::StateBlock* stateBlock);
	void pushAllHallAround(vector<int>* ids, int x, int y);
	bool floorTouchesSide(int x, int y);
	bool floorHasNeighbours(int x, int y);

protected:
	int height;
	int width;
	list<Room*> rooms;
	Floor** floorTiles;
};

#endif