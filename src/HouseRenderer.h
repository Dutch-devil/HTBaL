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
	HouseRenderer(Scene* scene);
	~HouseRenderer();

	void initialize();
	void createHouse(House);

	Renderers update(float elapsedTime);
	void render(float elapsedTime);

protected:
	RenderState::StateBlock* stateBlock;
	list<Model*> tileModels;
};


#endif