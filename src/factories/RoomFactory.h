#ifndef RoomFactory_H_
#define RoomFactory_H_

#include "../house/House.h"

class RoomFactory {
public:
	static Room* createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles);
	static Room* createRoomFromFloor(Scene* scene, House* house, list<Floor*> roomTiles, Room::Type roomType);

protected:
	static Wall* createWall(Scene* scene, Floor* floorTile, Floor::Direction dir);
};

#endif