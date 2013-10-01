#ifndef MapRenderer_H_
#define MapRenderer_H_

#include "gameplay.h"
#include "Renderer.h"

using namespace gameplay;

class MapRenderer: public Renderer {
public:
	MapRenderer();
	MapRenderer(Scene* scene);
	~MapRenderer();
	Renderers update(float elapsedTime);
	void render(float elapsedTime);
};


#endif