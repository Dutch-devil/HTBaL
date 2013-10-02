#ifndef HouseRenderer_H_
#define HouseRenderer_H_

#include "gameplay.h"
#include "Renderer.h"
#include "House.h"
#include "Floor.h"
#include "Wall.h"
#include <list>

using namespace gameplay;
using namespace std;

class HouseRenderer: public Renderer {
public:
	HouseRenderer(Scene* scene, Rectangle viewport, float aspectRatio);
	~HouseRenderer();

	void initialize();
	void createHouse();
	void createRoom();
	
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	Renderers update(float elapsedTime);
	void render(float elapsedTime);

protected:
	House* house;
	RenderState::StateBlock* stateBlock;
	Floor** floorTiles;
	Rectangle viewport;
	float aspectRatio;

	Floor* prevFloor;
};


#endif