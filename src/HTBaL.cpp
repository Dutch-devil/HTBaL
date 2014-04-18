
#include "HTBaL.h"

// Declare our game instance
HTBaL game;


// Our constructor :)
HTBaL::HTBaL() {}
HTBaL::~HTBaL() {}

// Initialize by setting the defaultrenderer as active.
void HTBaL::initialize() {
    StateManagerFactory::initialize();
	achievementRenderer = new AchievementRenderer();

	deleting = false;
    activeRenderer = NULL;
	setActiveRenderer(MAIN_MENU);
    //this->exit();
}

// We don't have to do anything when the game closes;
void HTBaL::finalize() {
    deleting = true;
	SAFE_DELETE(activeRenderer);
	SAFE_DELETE(achievementRenderer);
    Renderer::finalize();
    MaterialManager::finalize();
    StateManagerFactory::finalize();
}

bool HTBaL::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData) {
    return activeRenderer->mouseEvent(evt, x, y, wheelData);
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
    if (deleting) {
        return;
    }
    activeRenderer->update(elapsedTime);
    setActiveRenderer(activeRenderer->getNextRenderer());
}

void HTBaL::render(float elapsedTime) {
    if (deleting) {
        return;
    }
    // Clear the color and depth buffers.
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);
    
    // We let the active renderer update
    activeRenderer->render(elapsedTime);
	achievementRenderer->render(elapsedTime);
    
    // Draw the fps
    drawFrameRate(Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

void HTBaL::drawFrameRate(const Vector4& color, unsigned int x, unsigned int y, unsigned int fps) {
    char buffer[10];                                        // Make a char buffer
    sprintf(buffer, "%u", fps);                             // Print the fps to the buffer
    Font* font = Font::create("res/arial40.gpb");
    font->start();                                          // start using the font
    font->drawText(buffer, x, y, color, font->getSize());   // Writing the fps to the screen
    font->finish();                                         // stop using the font
    SAFE_RELEASE(font);
}

void HTBaL::setActiveRenderer(Renderers renderer) {
    if (renderer == KEEP) {                                 // If the renderer needs to stay the same
        return;                                             // Do nothing
    }
    // Else
    SAFE_DELETE(activeRenderer);                            // Delete the active renderer that was used before
    switch (renderer) {                                     // Switch over the posible renderers, and create the correct one.
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
            throw "This renderer is not implemented yet";   // Oops, we don't know what renderer we need to start.
    }
}