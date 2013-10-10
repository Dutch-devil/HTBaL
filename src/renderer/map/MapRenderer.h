#ifndef MapRenderer_H_
#define MapRenderer_H_

class MapRenderer;

#include "gameplay.h"
#include "../Renderer.h"

using namespace gameplay;

class MapRenderer: public Renderer {
public:
	MapRenderer(Rectangle viewport);
	~MapRenderer();
	void update(float elapsedTime);
	Renderers getNextRenderer();
	void render(float elapsedTime);
};


#endif