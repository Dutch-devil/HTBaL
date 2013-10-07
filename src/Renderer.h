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

    virtual void update(float elapsedTime) = 0;
	virtual Renderers getNextRenderer();
    virtual void render(float elapsedTime) = 0;

	bool leftButtonDown();
	bool rightButtonDown();
	bool middleButtonDown();
	
	virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData);
	virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging, bool clicked);
	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
	virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked);
	virtual void keyEvent(Keyboard::KeyEvent evt, int key);
	virtual void keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags);
	virtual void resizeEvent(unsigned int width, unsigned int height);

	static void drawText(const Vector4& color, unsigned int x, unsigned int y, char* text);
protected:
	Rectangle viewport;
	short buttonDown;

	Flags* getMouseFlags();
	KeyFlags* getKeyFlags();
	Vector2* getClickStarts();

private:
	Flags* mouseFlags;
	KeyFlags* keyFlags;
	Vector2* clickStarts;
};


#endif