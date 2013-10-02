#ifndef House_H_
#define House_H_

#include <list>

using namespace std;
#include "Room.h"

class House {
public:
	House(int, int);
    ~House(void);
	void addRoom(Room room);

	int getWidth();
	int getHeight();

	list<Room> getRooms();

protected:
	int height;
	int width;
	list<Room> rooms;
};

#endif