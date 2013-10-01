#include "House.h"



House::House(void) : width(0), height(0) {
	throw "Invalid house size (has to be bigger than 0)";
}

House::House(int w, int h): width(w), height(h) {
	if(width <= 0 || height <= 0) {
		throw "Invalid house size (has to be bigger than 0)";
	}
}

House::House(int w, int h, list<Room> rooms): width(w), height(h), rooms(rooms) {
	if(width <= 0 || height <= 0) {
		throw "Invalid house size (has to be bigger than 0)";
	}
}

void House::addRoom(Room room) {
	rooms.push_back(room);
}

House::~House(void) {
}
