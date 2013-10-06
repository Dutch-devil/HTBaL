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
#define FLOOR_HOVER_SELECTED new Vector3(blendColors(FLOOR_SELECTED, FLOOR_HOVER))

#define FLOOR_NONE FLOOR_BLACK
#define FLOOR_EMPTY FLOOR_UNSELECTED
#define FLOOR_HALL_START FLOOR_YELLOW
#define FLOOR_HALL FLOOR_BLUE
#define FLOOR_ROOM FLOOR_TEAL

class Floor;

#include <list>
#include "gameplay.h"
#include "Wall.h"
#include "Flags.h"

using namespace gameplay;
using namespace std;
 
enum Direction {
	TOP = 1,	// y - 1
	LEFT = 2,	// x - 1
	BOTTOM = 3, // y + 1
	RIGHT = 4,	// x + 1
	NONE = 5	// no wall
};

struct WalledTile {
	WalledTile(int id): id(id), dir(NONE) {}
	WalledTile(int id, Direction dir): id(id), dir(dir) {}
	int id;
	Direction dir;
};

class Floor {
public:
	Floor(int id);
    ~Floor();

	static void setHeightWidth(float height, float width);
	static float getHeight();
	static float getWidth();
	
	static Mesh* getMesh();
	int getId();
	
	Floor* setDoor(Direction dir, bool door);
	bool getDoor(Direction dir);
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

	
	static void releaseMesh();

protected:
	static Mesh* mesh;
	static float width, height;
	static void calculateMesh();

	int id;
	Model* model;
	Vector3* color;
	Vector3* realColor;
	bool selected, hover;

	Flags* doors;
};

#endif