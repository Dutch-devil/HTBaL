#include "Renderer.h"

Renderer::Renderer(Rectangle viewport) {
	this->viewport = viewport;
	buttonDown = 0;
}
Renderer::~Renderer() {}

bool Renderer::leftButtonDown() {
	return buttonDown & LEFT_BUTTON;
}

bool Renderer::rightButtonDown() {
	return buttonDown & RIGHT_BUTTON;
}

bool Renderer::middleButtonDown() {
	return buttonDown & MIDDLE_BUTTON;
}

bool Renderer::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData) {
	switch (evt) {
		case Mouse::MOUSE_PRESS_LEFT_BUTTON:
			buttonDown |= LEFT_BUTTON;
			break;
		case Mouse::MOUSE_PRESS_RIGHT_BUTTON:
			buttonDown |= RIGHT_BUTTON;
			break;
		case Mouse::MOUSE_PRESS_MIDDLE_BUTTON:
			buttonDown |= MIDDLE_BUTTON;
			break;
		case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
			buttonDown &= RIGHT_BUTTON | MIDDLE_BUTTON;
			break;
		case Mouse::MOUSE_RELEASE_RIGHT_BUTTON:
			buttonDown &= LEFT_BUTTON | MIDDLE_BUTTON;
			break;
		case Mouse::MOUSE_RELEASE_MIDDLE_BUTTON:
			buttonDown &= LEFT_BUTTON | RIGHT_BUTTON;
			break;
	}
	return mouseEvent(evt, x, y, wheelData, evt == Mouse::MOUSE_MOVE && buttonDown != 0);
}

bool Renderer::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging) {
	return false;
}

void Renderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {}


void Renderer::keyEvent(Keyboard::KeyEvent evt, int key) {}
void Renderer::resizeEvent(unsigned int width, unsigned int height) {
	this->viewport.width = width;
	this->viewport.height = height;
}