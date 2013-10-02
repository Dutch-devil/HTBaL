#ifndef Renderer_H_
#define Renderer_H_

class Renderer;

enum Renderers {
	KEEP,
	MAIN_MENU,
	HOUSE,
	MAP
};

#include "gameplay.h"

using namespace gameplay;

class Renderer {
public:
	Renderer(Rectangle viewport);
	~Renderer();

	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
	
	virtual void keyEvent(Keyboard::KeyEvent evt, int key);

    virtual Renderers update(float elapsedTime) = 0;

    virtual void render(float elapsedTime) = 0;

protected:
	Rectangle viewport;
};


#endif