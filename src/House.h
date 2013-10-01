#ifndef House_H_
#define House_H_

#include <list>
#include "Room.h"

using namespace std;

class House {
public:
	House(int, int);
	House(int, int, list<Room>);
    ~House(void);
	void addRoom(Room room);

	int getWidth();
	int getHeight();

protected:
	int height;
	int width;
	list<Room> rooms;
};

#endif