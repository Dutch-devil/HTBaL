#ifndef House_H_
#define House_H_

class House;

#include <list>
#include "Room.h"

using namespace std;

class House {
public:
	House(int, int);
    ~House(void);
	void addRoom(Room* room);

	int getWidth();
	int getHeight();

	int getIdByXY(int x, int y);
	int getXById(int id);
	int getYById(int id);

	list<Room*> getRooms();

protected:
	int height;
	int width;
	list<Room*> rooms;
};

#endif