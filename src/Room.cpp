#include "Room.h"

using namespace std;

Room::Room(void) : x(0), y(0), walls(0) {
	throw "no arguments specified to make room";
}

Room::Room(int x, int y, list<Vector2*> walls) : x(x), y(y), walls(walls) {
}


Room::~Room(void) {
}

list<Vector2*> Room::getWalls() {
	return walls;
}
