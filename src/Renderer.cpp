#include "Renderer.h"

Renderer::Renderer(Rectangle viewport) {
	this->viewport = viewport;
}
Renderer::~Renderer() {}

void Renderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {}


void Renderer::keyEvent(Keyboard::KeyEvent evt, int key) {}
void Renderer::resizeEvent(unsigned int width, unsigned int height) {
	this->viewport.width = width;
	this->viewport.height = height;
}