#ifndef HouseMenuWheel_H_
#define HouseMenuWheel_H_

#include "gameplay.h"
#include "../Renderer.h"
#include "../../house/House.h"

using namespace gameplay;

class HouseMenuWheel : public MenuWheel, MenuWheel::Listener {
public:
	HouseMenuWheel(Vector2 middle, vector<MenuWheelPart*> parts, float radius, float initAngle);
	
	void menuWheelEvent(MenuWheelPart* clickedPart);
	void draw(float elapsedTime);

	static HouseMenuWheel* create(Rectangle viewport);
};

#endif