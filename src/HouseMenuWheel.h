#ifndef HouseMenuWheel_H_
#define HouseMenuWheel_H_

#include "gameplay.h"
#include "MenuWheel.h"
#include "Room.h"

using namespace gameplay;

class HouseMenuWheel : public MenuWheel, MenuWheel::Listener {
public:
	HouseMenuWheel(Scene* scene, Rectangle viewport, Vector2 middle, vector<MenuWheelPart*> parts, float initAngle);
	
	void menuWheelEvent(MenuWheelPart* clickedPart);
	void draw(float elapsedTime);

	static HouseMenuWheel* create(Scene* scene, Rectangle viewport);
};

#endif