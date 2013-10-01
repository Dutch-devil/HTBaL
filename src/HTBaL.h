#ifndef HTBaL_H_
#define HTBaL_H_

#include "gameplay.h"
#include "Renderer.h"

using namespace gameplay;

/**
 * Main game class.
 */

class HTBaL: public Game {
public:

    HTBaL();

protected:

    void initialize();

    void finalize();

	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void update(float elapsedTime);

    void render(float elapsedTime);

	void setActiveRenderer(Renderers renderer);

	void drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps);

private:

    Scene* scene;
	Renderer* activeRenderer;
};

#endif