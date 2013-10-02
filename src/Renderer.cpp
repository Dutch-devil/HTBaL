#include "Renderer.h"

Renderer::Renderer(Rectangle viewport) {
	this->viewport = viewport;
}
Renderer::~Renderer() {}

void Renderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {}


void Renderer::keyEvent(Keyboard::KeyEvent evt, int key) {}