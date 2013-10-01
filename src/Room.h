#ifndef Room_H_
#define Room_H_

#include <list>
#include "gameplay.h"

using namespace std;
using namespace gameplay;

class Room {
public:
    Room(void);
	Room(int, int, list<Vector2>);
    ~Room(void);

protected:
	int x;
	int y;
	list<Vector2> walls;
};

#endif