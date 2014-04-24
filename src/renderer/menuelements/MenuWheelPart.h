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

class MenuWheelPart;

#include "gameplay.h"
#include "MenuWheel.h"

using namespace gameplay;

class MenuWheelPart {
public:
	MenuWheelPart();
    ~MenuWheelPart();
	
	static void setSize(float size);
	//static Mesh* getMesh();
	//static void releaseMesh();
	//static float getMeshSize(float relativeSize);

	//Node* getNode();
	//Model* getModel();
	
	void setHover(bool hover);

	int getId();
	void setId(int id);
	const char* getTitle();
	void setTitle(const char* title);

	void rotate(float angle);
	float getRotation();

	Vector4 getColor();

protected:
	bool hover;

	float rotation;

	Vector3 color;
	Vector3* realColor;
	void updateColor();
	Vector3 blendColors(Vector3* color1, Vector3* color2);

	int id;
	const char* title;
};

#endif