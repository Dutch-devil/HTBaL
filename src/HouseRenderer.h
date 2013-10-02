#ifndef HouseRenderer_H_
#define HouseRenderer_H_

#include "gameplay.h"
using namespace gameplay;
#include <list>
using namespace std;
#include "Room.h"
#include "Renderer.h"
#include "House.h"
#include "Floor.h"
#include "Wall.h"

class HouseRenderer: public Renderer {
public:
	HouseRenderer(Rectangle viewport);
	~HouseRenderer();

	void initialize();
	void createHouse();
	void createRoom();
	
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	Renderers update(float elapsedTime);
	void render(float elapsedTime);

protected:
	Scene* scene;
	House* house;
	RenderState::StateBlock* stateBlock;
	Floor** floorTiles;
	list<Model*> wallModels;
	float aspectRatio;

	Floor* prevFloor;
};


#endif