#include "House.h"


House::House(int w, int h): width(w), height(h) {
	if(width <= 0 || height <= 0) {
		throw "Invalid house size (has to be bigger than 0)";
	}
}

House::~House(void) {
}

void House::addRoom(Room* room) {
	rooms.push_back(room);
}

int House::getWidth() {
	return width;
}

int House::getHeight() {
	return height;
}

list<Room*> House::getRooms() {
	return rooms;
}