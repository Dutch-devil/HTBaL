#ifndef Renderer_H_
#define Renderer_H_

#define LEFT_BUTTON 1
#define RIGHT_BUTTON 2
#define MIDDLE_BUTTON 4

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
	virtual ~Renderer();

	bool leftButtonDown();
	bool rightButtonDown();
	bool middleButtonDown();
	
	virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData);
	virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging);
	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
	virtual void keyEvent(Keyboard::KeyEvent evt, int key);
	
	virtual void resizeEvent(unsigned int width, unsigned int height);

    virtual Renderers update(float elapsedTime) = 0;

    virtual void render(float elapsedTime) = 0;
protected:
	Rectangle viewport;
	short buttonDown;
};


#endif