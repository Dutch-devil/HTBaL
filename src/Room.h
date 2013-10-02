#ifndef Room_H_
#define Room_H_

#include <list>
#include "gameplay.h"

using namespace std;
using namespace gameplay;

class Room {
public:
	Room(int, int, list<Vector2*>);
    ~Room(void);

	list<Vector2*> getWalls();

protected:
	int x;
	int y;
	list<Vector2*> walls;
};

#endif