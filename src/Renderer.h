#ifndef Renderer_H_
#define Renderer_H_

#include "gameplay.h"

using namespace gameplay;

enum Renderers {
	KEEP,
	MAIN_MENU,
	HOUSE,
	MAP
};

class Renderer {
public:
	Renderer(Rectangle viewport);
	~Renderer();

	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    virtual Renderers update(float elapsedTime) = 0;

    virtual void render(float elapsedTime) = 0;

protected:
	Rectangle viewport;
};


#endif