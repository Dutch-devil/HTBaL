#ifndef Floor_H_
#define Floor_H_

// all different colors
#define FLOOR_WHITE new Vector3(1, 1, 1)
#define FLOOR_RED new Vector3(1, 0, 0)
#define FLOOR_GREEN new Vector3(0, 1, 0)
#define FLOOR_BLUE new Vector3(0, 0, 1)
#define FLOOR_YELLOW new Vector3(1, 1, 0)
#define FLOOR_PURPLE new Vector3((1, 0, 1)
#define FLOOR_BLACK new Vector3(0, 0, 0)
#define FLOOR_TEAL new Vector3(.5, .5, 1)

#define FLOOR_UNSELECTED FLOOR_WHITE
#define FLOOR_SELECTED FLOOR_RED
#define FLOOR_HOVER FLOOR_GREEN
#define FLOOR_HOVER_SELECTED blendColors(FLOOR_SELECTED, FLOOR_HOVER)

#define FLOOR_EMPTY FLOOR_UNSELECTED
#define FLOOR_HALL_START FLOOR_YELLOW
#define FLOOR_HALL FLOOR_BLUE
#define FLOOR_ROOM FLOOR_TEAL

class Floor;

#include <list>
#include "gameplay.h"

using namespace gameplay;
using namespace std;
 
enum WallDir {
	TOP, LEFT, BOTTOM, RIGHT, NONE
};
struct WalledTile {
	WalledTile(int id): id(id), dir(NONE) {}
	WalledTile(int id, WallDir dir): id(id), dir(dir) {}
	int id;
	WallDir dir;
};

class Floor {
public:
	static float width, height;

	Floor(int id, float x, float y);
    ~Floor();
	
	Mesh* getMesh();
	int getId();
	
	Floor* setDoor(WallDir dir);
	WallDir getDoor();

	float getX();
	float getY();
	Model* getModel();

	Vector3* getColor();
	bool isColor(Vector3*);
	Floor* setColor(Vector3*);
	void updateColor();
	void toggleColor(Vector3* first, Vector3* second);
	Vector3 blendColors(Vector3* color1, Vector3* color2);
	
	bool getSelected();
	Floor* setSelected(bool selected);
	Floor* toggleSelect();
	Floor* setHover(bool hover);

protected:
	int id;
	float x, y;
	Model* model;
	WallDir doorDir;
	Vector3* color;
	Vector3* realColor;
	bool selected, hover;
};

#endif