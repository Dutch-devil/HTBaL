#ifndef MainMenuWheel_H_
#define MainMenuWheel_H_

#include "gameplay.h"
#include "../Renderer.h"

using namespace gameplay;

class MainMenuWheel : public MenuWheel, MenuWheel::Listener {
public:
	MainMenuWheel(Vector2 middle, vector<MenuWheelPart*> parts, float radius, float initAngle);
	
	Renderers getNextRenderer();

	void menuWheelEvent(MenuWheelPart* clickedPart);
	void draw(float elapsedTime);

	static MainMenuWheel* create();

protected:
	Renderers nextRenderer;

private:
	static const char* roomNames[];
};

#endif