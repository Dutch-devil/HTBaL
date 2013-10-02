#ifndef HTBaL_H_
#define HTBaL_H_

class HTBaL;

#include "gameplay.h"
#include "Renderer.h"

using namespace gameplay;

/**
 * Main game class.
 */

class HTBaL: public Game {
public:

	/** The main constructor */
    HTBaL();

protected:
	/** 
	  * Initialize callback that is called just before the first frame when the game starts.
	  * This loads everything and installs the default renderer.
	  */
    void initialize();

	/**
	  * Finalize callback that is called when the game on exits.
	  * This cleans up everything. 
	  */
    void finalize();

    /**
     * Touch callback on touch events.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @see gameplay::Touch::TouchEvent
     */
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Update callback for handling update routines.
     *
     * Called just before render, once per frame when game is running.
     * Ideal for non-render code and game logic such as input and animation.
     *
     * @param elapsedTime The elapsed game time.
     */
    void update(float elapsedTime);

    /**
     * Render callback for handling rendering routines.
     *
     * Called just after update, once per frame when game is running.
     * Ideal for all rendering code.
     *
     * @param elapsedTime The elapsed game time.
     */
    void render(float elapsedTime);

	/**
	  * Set the active renderer to a specific renderer.
	  * This will exit the current renderer and create a new one of the type specified.
	  *
	  * @param renderer The new renderer to load.
	  */
	void setActiveRenderer(Renderers renderer);

	/**
	  * Helper function to draw the framerate of the game.
	  *
	  * @param font The font used.
	  * @param color The color used.
	  * @param x The x position the framerate will be drawn at.
	  * @param y The y position the framerate will be drawn at.
	  * @param fps The actual framerate to draw.
	  *
	  * @see gameplay::Game:getFrameRate()
	  */
	void drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps);

private:

	Renderer* activeRenderer;			/// The active renderer.
};

#endif