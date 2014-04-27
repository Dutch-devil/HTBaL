#include "GenericInputListener.h"


GenericInputListener::GenericInputListener() {
	buttonDown = 0;
	keyFlags = NULL;
	mouseFlags = NULL;
	clickStarts = NULL;
}

GenericInputListener::~GenericInputListener() {
	SAFE_DELETE(mouseFlags);
	SAFE_DELETE(keyFlags);
	SAFE_DELETE_ARRAY(clickStarts);
}



bool GenericInputListener::leftButtonDown() {
	return getMouseFlags()->getFlag(Mouse::MouseEvent::MOUSE_PRESS_LEFT_BUTTON / 2);
}

bool GenericInputListener::rightButtonDown() {
	return getMouseFlags()->getFlag(Mouse::MouseEvent::MOUSE_PRESS_RIGHT_BUTTON / 2);
}

bool GenericInputListener::middleButtonDown() {
	return getMouseFlags()->getFlag(Mouse::MouseEvent::MOUSE_PRESS_MIDDLE_BUTTON / 2);
}

bool GenericInputListener::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData) {
	int btn = evt / 2;
	bool clicked = false;
	if (btn < 3) {
		bool press = (evt + 1) % 2;
		getMouseFlags()->forceFlag(btn, press);
		Vector2* starts = getClickStarts();
		if (press) {
			starts[btn].x = x;
			starts[btn].y = y;
		} else {
			clicked = starts[btn].x == x && starts[btn].y == y;
		}
	} else if (clickStarts != NULL) {
		memset(clickStarts, 0, sizeof(Vector2) * 3);
	}
	return mouseEvent(evt, x, y, wheelData, evt == Mouse::MOUSE_MOVE && (leftButtonDown() || rightButtonDown() || middleButtonDown()), clicked);
}

bool GenericInputListener::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {
	bool clicked = false;
	Vector2* starts = getClickStarts();
	if (evt == Touch::TouchEvent::TOUCH_PRESS) {
		starts[0].x = x;
		starts[0].y = y;
	} else if (evt == Touch::TouchEvent::TOUCH_RELEASE) {
		clicked = starts[0].x == x && starts[0].y == y;
	} else if (clickStarts != NULL) {
		memset(clickStarts, 0, sizeof(Vector2) * 3);
	}
	return touchEvent(evt, x, y, contactIndex, clicked);
}

bool GenericInputListener::keyEvent(Keyboard::KeyEvent evt, int key) {
	if (evt == Keyboard::KeyEvent::KEY_PRESS) {
		getKeyFlags()->setFlag(key);
	} else if (evt == Keyboard::KeyEvent::KEY_RELEASE) {
		getKeyFlags()->resetFlag(key);
	}
	return keyEvent(evt, key, getKeyFlags());
}

Flags* GenericInputListener::getMouseFlags() {
	if (mouseFlags == NULL) {
		mouseFlags = new Flags(4);
	}
	return mouseFlags;
}

KeyFlags* GenericInputListener::getKeyFlags() {
	if (keyFlags == NULL) {
		keyFlags = new KeyFlags();
	}
	return keyFlags;
}

Vector2* GenericInputListener::getClickStarts() {
	if (clickStarts == NULL) {
		clickStarts = new Vector2[3];
		memset(clickStarts, 0, sizeof(Vector2) * 3);
	}
	return clickStarts;
}
