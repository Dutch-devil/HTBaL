#ifndef Floor_H_
#define Floor_H_


// all different colors
#define FLOOR_WHITE new Vector3(1, 1, 1)
#define FLOOR_RED new Vector3(1, 0, 0)
#define FLOOR_GREEN new Vector3(0, 1, 0)
#define FLOOR_BLUE new Vector3(0, 0, 1)

#define FLOOR_UNSELECTED FLOOR_WHITE
#define FLOOR_SELECTED FLOOR_RED



#include <list>
#include "Room.h"
#include "gameplay.h"

using namespace std;
using namespace gameplay;
 
class Floor {
public:
	static int width, height;

	Floor(RenderState::StateBlock* stateBlock);
    ~Floor();
	
	Mesh* getMesh();
	Model* getModel();

	Vector3* getColor();
	bool isColor(Vector3*);
	void setColor(Vector3*);
	void toggleColor(Vector3* first, Vector3* second);
	Vector3 blendColors(Vector3* color1, Vector3* color2);
	
	void toggleSelect();

protected:
	Model* model;
	Vector3* color;
	Vector3* realColor;
	bool selected;
};

#endif