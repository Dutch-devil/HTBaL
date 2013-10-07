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

enum RoomType {
	//HALL_START = "Hall start",
	//HALL = "Hall",
	//ROOM_EMPTY = "Empty room",
	HALL_START,
	HALL,
	ROOM_EMPTY,
};

class Room {
public:
	Room(House* house, list<Floor*> floor, list<Wall*> walls);
	Room(House* house, list<Floor*> floor, list<Wall*> walls, RoomType roomType);
    ~Room();

	RoomType getRoomType();
	const char* getRoomTypeString();

	list<Floor*> getFloor();
	list<Wall*> getWalls();

	bool contains(Floor* floorTile);

	int getSize();
	int getMaxRectangle();
	int getMaxLine();

	list<Floor*> getCorners();
	list<Floor*> getLine(Floor* startTile, Direction dir);
	list<Floor*> getLine(Floor* startTile, Direction dir, unsigned int maxLength);
	
	static Room* createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles);
	static Room* createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles, RoomType roomType);

protected:
	House* house;
	RoomType roomType;
	list<Floor*> floor;
	list<Wall*> walls;
	
	static Wall* createWall(Scene* scene, Floor* floorTile, Direction dir);
	Room(int, int, list<Wall*>, Scene* scene);


	static Wall* createWall(bool door, Scene* scene, float x, float y, float rot);
};

#endif