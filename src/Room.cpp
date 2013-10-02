#include "Room.h"

using namespace std;

Room::Room(int x, int y, list<Vector2*> walls) : x(x), y(y), walls(walls) {

}


Room::~Room(void) {
}

list<Vector2*> Room::getWalls() {
	return walls;
}
