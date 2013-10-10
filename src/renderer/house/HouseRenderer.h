#ifndef HouseRenderer_H_
#define HouseRenderer_H_

#define SCROLL_SPEED .12
#define ZOOM_SPEED .1

class HouseRenderer;

#include "gameplay.h"
#include <list>
#include "../../house/House.h"
#include "../../factories/HouseFactory.h"
#include "../Renderer.h"
#include "HouseMenuWheel.h"

using namespace gameplay;
using namespace std;

class HouseRenderer: public Renderer, Control::Listener, MenuWheel::Listener {
public:
	HouseRenderer(Rectangle viewport);
	~HouseRenderer();

	void createMenu(float menuWidth);
	void createHouse(bool random, bool rooms);
	void removeHouse();
	
	void checkHover(int x, int y);
	
	bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging, bool clicked);
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked);
	void keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags);
	void resizeEvent(unsigned int width, unsigned int height);
	void controlEvent(Control* control, Control::Listener::EventType evt);
	void menuWheelEvent(MenuWheelPart* clickedPart);

	void update(float elapsedTime);
	Renderers getNextRenderer();
	void render(float elapsedTime);


protected:
	Rectangle renderViewPort;
	Vector3* initialTranslate;
	Vector3* curTranslate;
	float zoomLevel;

	Scene* scene;
	House* house;
	float aspectRatio;
	float menuWidth;
	float renderHeight;

	Room* hoverRoom, *prevRoom;
	Floor* prevFloor;
	Floor* prevHover;

	Form* houseRendererForm;
	Renderers nextRenderer;

	HouseMenuWheel* roomTypes;
	
	void initialize();
	void resize();
	void setCamera();
	int getViewTileId(int x, int y);
};


#endif