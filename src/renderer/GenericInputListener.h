#pragma once

#include "gameplay.h"
#include "../KeyFlags.h"

using namespace gameplay;

class GenericInputListener {
public:
	GenericInputListener();
	~GenericInputListener();
    
    bool leftButtonDown();
    bool rightButtonDown();
    bool middleButtonDown();

	bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData);
	bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
	bool keyEvent(Keyboard::KeyEvent evt, int key);

	virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta, bool dragging, bool clicked) = 0;
	virtual bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked) = 0;
	virtual bool keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags) = 0;

protected:
	Flags* getMouseFlags();
	KeyFlags* getKeyFlags();
	Vector2* getClickStarts();

private:
	short buttonDown;
	Flags* mouseFlags;
	KeyFlags* keyFlags;
	Vector2* clickStarts;
};

