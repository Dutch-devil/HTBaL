#include "HouseMenuWheel.h"

HouseMenuWheel::HouseMenuWheel(Vector2 middle, vector<MenuWheelPart*> parts, float radius, float initAngle): MenuWheel(middle, parts, radius, initAngle) {
	//this->addListener(this);
}


void HouseMenuWheel::menuWheelEvent(MenuWheelPart* clickedPart) {
	
}

void HouseMenuWheel::draw(float elapsedTime) {
	MenuWheel::draw(elapsedTime);
	if (prevPart != NULL) {
		Renderer::drawText(Vector4::one(), Rectangle(middle.x - 205, middle.y - 55, 200, 20), prevPart->getTitle(), Font::Justify::ALIGN_RIGHT);
	}
}

HouseMenuWheel* HouseMenuWheel::create(Rectangle viewport) {
	unsigned int size = Room::NONE;
	vector<MenuWheelPart*> parts = vector<MenuWheelPart*>(size);
	for (unsigned int i = 0; i < parts.size(); i++) {
		int index = size - i * 2 - 1;
		parts[i] = new MenuWheelPart();
		parts[i]->setId(index >= 0?index:-index - 1);
		parts[i]->setTitle(Room::getRoomTypeString((Room::Type)(parts[i]->getId())));
	}
	return new HouseMenuWheel(Vector2(viewport.width, viewport.height), parts, 300, MATH_PI);
}

