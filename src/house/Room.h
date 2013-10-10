#ifndef Room_H_
#define Room_H_

class Room;

#include <list>
#include "gameplay.h"
#include "Floor.h"
#include "House.h"

using namespace std;
using namespace gameplay;

class Room {
public:
	enum Type {
		LIVING_QUARTERS,
		BEDROOM,
		DINING_ROOM,
		STORAGE_ROOM,
		LIBARY,
		LABORATORIUM,
		ARBORETUM,
		WORKSHOP,
		SMITHY,
		STINKY_SEWERS,
		DIRTPLOT,
		STABLES,
		ARSENAL_ROOM,
		AVIARY,
		POOL,
		ART_ROOM,
		BALLROOM,
		ENTRANCE,
		HALL,
		ROOM_EMPTY,
		NONE,
	};
	Room(House* house, list<Floor*> floor, list<Wall*> walls);
	Room(House* house, list<Floor*> floor, list<Wall*> walls, Room::Type roomType);
    ~Room();

	Room::Type getRoomType();
	void setRoomType(Room::Type roomType);
	const char* getRoomTypeString();
	static const char* getRoomTypeString(Room::Type roomType);

	list<Floor*> getFloor();
	list<Wall*> getWalls();

	bool contains(Floor* floorTile);

	int getSize();
	int getMaxRectangle();
	int getMaxLine();

	list<Floor*> getCorners();
	list<Floor*> getLine(Floor* startTile, Floor::Direction dir);
	list<Floor*> getLine(Floor* startTile, Floor::Direction dir, unsigned int maxLength);

protected:
	House* house;
	Room::Type roomType;
	list<Floor*> floor;
	list<Wall*> walls;
	
	Room(int, int, list<Wall*>, Scene* scene);


};

#endif