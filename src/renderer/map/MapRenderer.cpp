#include "MapRenderer.h"

MapRenderer::MapRenderer(Rectangle viewport) : Renderer(viewport) {
}

MapRenderer::~MapRenderer() {

}

void MapRenderer::update(float elapsedTime) {
	
}

Renderers MapRenderer::getNextRenderer() {
	return KEEP;
}

bool MapRenderer::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging, bool clicked) {
	return false;
}

bool MapRenderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked) {
	return false;
}

bool MapRenderer::keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags) {
	return false;
}

void MapRenderer::render(float elapsedTime) {
}