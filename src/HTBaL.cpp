#include "HTBaL.h"
#include "MenuMainRenderer.h"
#include "HouseRenderer.h"
#include "MapRenderer.h"

// Declare our game instance
HTBaL game;

// Our constructor :)
HTBaL::HTBaL() {}

// Initialize by setting the defaultrenderer as active.
void HTBaL::initialize() {
	activeRenderer = new HouseRenderer(getViewport());
}

// We don't have to do anything when the game closes;
void HTBaL::finalize() {
}

// When we receive a touch event, we send it to the active renderer.
void HTBaL::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {
	activeRenderer->touchEvent(evt, x, y, contactIndex);
}

// When we receive a key event, we send it to the active renderer.
void HTBaL::keyEvent(Keyboard::KeyEvent evt, int key) {
	activeRenderer->keyEvent(evt, key);
}

void HTBaL::resizeEvent(unsigned int width, unsigned int height) {
	Rectangle viewport = getViewport();
	viewport.width = width;
	viewport.height = height;
	setViewport(viewport);
	activeRenderer->resizeEvent(width, height);
}

// We let the activerenderer update, and, with the return value of the
// function we determine if the activerenderer has to change.
void HTBaL::update(float elapsedTime) {
	setActiveRenderer(activeRenderer->update(elapsedTime));
}

void HTBaL::render(float elapsedTime) {
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

	// We let the active renderer update
	activeRenderer->render(elapsedTime);

    // Load font
    Font* _font = Font::create("res/arial40.gpb");
	// Draw the fps
	drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void HTBaL::drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps)
{
    char buffer[10];										// Make a char buffer
    sprintf(buffer, "%u", fps);								// Print the fps to the buffer
    font->start();											// start using the font
    font->drawText(buffer, x, y, color, font->getSize());	// Writing the fps to the screen
    font->finish();											// stop using the font
}

void HTBaL::setActiveRenderer(Renderers renderer) {
	if (renderer == KEEP) {									// If the renderer needs to stay the same
		return;												// Do nothing
	}
															// Else
	delete activeRenderer;									// Delete the active renderer that was used before
	activeRenderer = NULL;									// set it to NULL to be sure nothing can go wrong
	switch (renderer) {										// Switch over the posible renderers, and create the correct one.
		case MAIN_MENU:
			activeRenderer = new MenuMainRenderer(getViewport());
			break;
		case HOUSE:
			activeRenderer = new HouseRenderer(getViewport());
			break;
		case MAP:
			activeRenderer = new MapRenderer(getViewport());
			break;
		default:
			throw "This renderer is not implemented yet";	// Oops, we don't know what renderer we need to start.
	}
}