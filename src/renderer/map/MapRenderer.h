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

	bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging, bool clicked);
	bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked);
	bool keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags);
};


#endif