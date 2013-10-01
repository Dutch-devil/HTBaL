#include "HTBaL.h"
#include "MenuMainRenderer.h"
#include "HouseRenderer.h"
#include "MapRenderer.h"

// Declare our game instance
HTBaL game;

HTBaL::HTBaL() : scene(NULL) {}

void HTBaL::initialize() {
	activeRenderer = new MenuMainRenderer(scene);
}

void HTBaL::finalize() {
    SAFE_RELEASE(scene);
}

void HTBaL::update(float elapsedTime) {
	setActiveRenderer(activeRenderer->update(elapsedTime));
}

void HTBaL::render(float elapsedTime) {
	// Clear the color and depth buffers.
    clear(CLEAR_COLOR, Vector4::one(), 1.0f, 0);
	activeRenderer->render(elapsedTime);
}

void HTBaL::setActiveRenderer(Renderers renderer) {
	if (renderer == KEEP) {
		return;
	}
	delete activeRenderer;
	activeRenderer = NULL;
	switch (renderer) {
		case MAIN_MENU:
			activeRenderer = new MenuMainRenderer(scene);
			break;
		case HOUSE:
			activeRenderer = new HouseRenderer(scene);
			break;
		case MAP:
			activeRenderer = new MapRenderer(scene);
			break;
		default:
			throw "This renderer is not implemented yet";
	}
}
