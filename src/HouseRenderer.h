#ifndef HouseRenderer_H_
#define HouseRenderer_H_

#include "gameplay.h"
#include "Renderer.h"
#include "House.h"
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
	list<Model*> tileModels;
	list<Model*> wallModels;
	Rectangle viewport;
	float aspectRatio;

	float tileWidth, tileHeight;
};


#endif