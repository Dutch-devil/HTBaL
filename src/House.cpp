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

int House::getIdByXY(int x, int y) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return -1;
	}
	return y*width+x;
}

int House::getXById(int id) {
	return id % width;
}

int House::getYById(int id) {
	return id / width;
}