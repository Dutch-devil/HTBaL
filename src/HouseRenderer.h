#ifndef HouseRenderer_H_
#define HouseRenderer_H_

class HouseRenderer;

#include "gameplay.h"
#include <list>
#include "Room.h"
#include "Renderer.h"
#include "House.h"
#include "Floor.h"
#include "Wall.h"

using namespace gameplay;
using namespace std;

class HouseRenderer: public Renderer, Control::Listener {
public:
	HouseRenderer(Rectangle viewport);
	~HouseRenderer();


	void createMenu(float menuWidth);
	void createHouse(float renderHeight);
	void createRoom();
	
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
	
	void keyEvent(Keyboard::KeyEvent evt, int key);

	Renderers update(float elapsedTime);
	void render(float elapsedTime);

	void controlEvent(Control* control, Control::Listener::EventType evt);

protected:
	void initialize();
	Rectangle renderViewPort;

	Scene* scene;
	House* house;
	RenderState::StateBlock* stateBlock;
	Floor** floorTiles;
	list<Model*> wallModels;
	float aspectRatio;
	float menuWidth;
	float renderHeight;

	Floor* prevFloor;

	Form* houseRendererForm;
	Renderers nextRenderer;
};


#endif