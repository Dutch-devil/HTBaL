#ifndef MainMenuWheel_H_
#define MainMenuWheel_H_

#include "gameplay.h"
#include "../Renderer.h"

using namespace gameplay;

class MainMenuWheel : public MenuWheel, MenuWheel::Listener {
public:
	MainMenuWheel(Scene* scene, Rectangle viewport, Vector2 middle, vector<MenuWheelPart*> parts, float initAngle);
	
	Renderers getNextRenderer();

	void menuWheelEvent(MenuWheelPart* clickedPart);
	void draw(float elapsedTime);

	static MainMenuWheel* create(Scene* scene, Rectangle viewport);

protected:
	Renderers nextRenderer;

private:
	static const char* roomNames[];
};

#endif