#ifndef Renderer_H_
#define Renderer_H_

class Renderer;

enum Renderers {
	KEEP,
	MAIN_MENU,
	HOUSE,
	MAP
};

#include "KeyFlags.h"
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
	virtual void keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags);
	
	virtual void resizeEvent(unsigned int width, unsigned int height);

    virtual Renderers update(float elapsedTime) = 0;

    virtual void render(float elapsedTime) = 0;
protected:
	Rectangle viewport;
	short buttonDown;

	Flags* getMouseFlags();
	KeyFlags* getKeyFlags();

private:
	Flags* mouseFlags;
	KeyFlags* keyFlags;
};


#endif