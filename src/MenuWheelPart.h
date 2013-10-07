#ifndef MenuWheelPart_H_
#define MenuWheelPart_H_

// all different colors
#define MENU_WHEEL_WHITE new Vector3(1, 1, 1)
#define MENU_WHEEL_RED new Vector3(1, 0, 0)
#define MENU_WHEEL_GREEN new Vector3(0, 1, 0)
#define MENU_WHEEL_BLUE new Vector3(0, 0, 1)
#define MENU_WHEEL_YELLOW new Vector3(1, 1, 0)
#define MENU_WHEEL_PURPLE new Vector3((1, 0, 1)
#define MENU_WHEEL_BLACK new Vector3(0, 0, 0)
#define MENU_WHEEL_TEAL new Vector3(.5, .5, 1)

#define MENU_WHEEL_HOVER MENU_WHEEL_GREEN

#include "gameplay.h"
#include "MaterialManager.h"

using namespace gameplay;

class MenuWheelPart {
public:
	MenuWheelPart();
    ~MenuWheelPart();
	
	static void setViewport(Rectangle viewport);
	static Mesh* getMesh();
	static void releaseMesh();
	static float getMeshHeight();

	Node* getNode();
	Model* getModel();
	
	void setHover(bool hover);

protected:
	static Rectangle viewport;
	static Mesh* mesh;

	Model* model;
	bool hover;
	Vector3* color;
	Vector3* realColor;

	void updateColor();
	Vector3 blendColors(Vector3* color1, Vector3* color2);
};

#endif