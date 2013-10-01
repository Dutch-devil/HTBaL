#include "HTBaL.h"
#include "MenuMainRenderer.h"
#include "HouseRenderer.h"
#include "MapRenderer.h"

// Declare our game instance
HTBaL game;

HTBaL::HTBaL() : scene(NULL) {
}

void HTBaL::initialize() {
	scene = Scene::create("Main");
	activeRenderer = new HouseRenderer(scene);
	scene->getActiveCamera()->setAspectRatio(getAspectRatio());
}

void HTBaL::finalize() {
    SAFE_RELEASE(scene);
}

void HTBaL::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {
	activeRenderer->touchEvent(evt, x, y, contactIndex);
}

void HTBaL::update(float elapsedTime) {
	setActiveRenderer(activeRenderer->update(elapsedTime));
}

void HTBaL::render(float elapsedTime) {
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

	activeRenderer->render(elapsedTime);
    // Draw the fps
    // Load font
    Font* _font = Font::create("res/arial40.gpb");
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void HTBaL::drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps)
{
    char buffer[10];
    sprintf(buffer, "%u", fps);
    font->start();
    font->drawText(buffer, x, y, color, font->getSize());
    font->finish();
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