#include "Renderer.h"

Renderer::Renderer(Rectangle viewport) {
	this->viewport = viewport;
	buttonDown = 0;
	keyFlags = NULL;
	mouseFlags = NULL;
}
Renderer::~Renderer() {
	SAFE_DELETE(mouseFlags);
	SAFE_DELETE(keyFlags);
}

bool Renderer::leftButtonDown() {
	return getMouseFlags()->getFlag(Mouse::MouseEvent::MOUSE_PRESS_LEFT_BUTTON / 2);
}

bool Renderer::rightButtonDown() {
	return getMouseFlags()->getFlag(Mouse::MouseEvent::MOUSE_PRESS_RIGHT_BUTTON / 2);
}

bool Renderer::middleButtonDown() {
	return getMouseFlags()->getFlag(Mouse::MouseEvent::MOUSE_PRESS_MIDDLE_BUTTON / 2);
}

bool Renderer::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData) {
	getMouseFlags()->forceFlag(evt / 2, (evt + 1) % 2);
	return mouseEvent(evt, x, y, wheelData, evt == Mouse::MOUSE_MOVE && (leftButtonDown() || rightButtonDown() || middleButtonDown()));
}

bool Renderer::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging) {
	return false;
}

void Renderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {}

void Renderer::keyEvent(Keyboard::KeyEvent evt, int key) {
	if (evt == Keyboard::KeyEvent::KEY_PRESS) {
		getKeyFlags()->setFlag(key);
	}else if (evt == Keyboard::KeyEvent::KEY_RELEASE) {
		getKeyFlags()->resetFlag(key);
	}
	keyEvent(evt, key, getKeyFlags());
}
void Renderer::keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags) {}


Flags* Renderer::getMouseFlags() {
	if (mouseFlags == NULL) {
		mouseFlags = new Flags(4);
	}
	return mouseFlags;
}

KeyFlags* Renderer::getKeyFlags() {
	if (keyFlags == NULL) {
		keyFlags = new KeyFlags();
	}
	return keyFlags;
}

void Renderer::resizeEvent(unsigned int width, unsigned int height) {
	this->viewport.width = width;
	this->viewport.height = height;
}

void Renderer::drawText(const Vector4& color, unsigned int x, unsigned int y, char* text) {
	Font* font = Font::create("res/arial14.gpb");
    font->start();											// start using the font
    font->drawText(text, x, y, color, font->getSize());	// Writing the fps to the screen
    font->finish();											// stop using the font
	SAFE_RELEASE(font);
}