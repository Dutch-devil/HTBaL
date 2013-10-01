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

    void update(float elapsedTime);

    void render(float elapsedTime);

	void setActiveRenderer(Renderers renderer);

private:

    Scene* scene;
	Renderer* activeRenderer;
};

#endif
