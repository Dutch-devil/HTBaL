#ifndef House_H_
#define House_H_

class House;

#include <list>
#include <cmath>
#include "Room.h"
#include "Floor.h"

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

	list<Room*> getRooms();
	void addRandomRooms(Scene* scene);
	
	void setDoor(list<Floor*> tiles, WalledTile adjacent);

	void clearAllAround(vector<WalledTile>* ids, int x, int y);
	void pushAllHallAround(vector<WalledTile>* ids, int x, int y);
	bool canBeHallway(int x, int y);
	bool floorTouchesSide(int x, int y);
	bool floorHasNeighbours(int x, int y);

	void pushAllRoomAround(vector<int>* ids, int x, int y);
	void pushAllRoomAround(vector<WalledTile>* ids, int x, int y);
	bool canBeRoom(int x, int y);
	void removeId(vector<int>* ids, int id);
	void removeId(vector<WalledTile>* ids, int id);
	list<Floor*> getGaps(vector<WalledTile>* toCheck);
	list<Floor*> getGaps(vector<WalledTile>* toCheck, unsigned int maxSize);
	bool getEnclosed(int startId, list<Floor*>* others);

protected:
	int height;
	int width;
	list<Room*> rooms;
	Floor** floorTiles;
};

#endif