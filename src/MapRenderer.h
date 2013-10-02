#ifndef MapRenderer_H_
#define MapRenderer_H_

class MapRenderer;

#include "gameplay.h"
#include "Renderer.h"

using namespace gameplay;

class MapRenderer: public Renderer {
public:
	MapRenderer(Rectangle viewport);
	~MapRenderer();
	Renderers update(float elapsedTime);
	void render(float elapsedTime);
};


#endif