#include "MainMenuWheel.h"

MainMenuWheel::MainMenuWheel(Scene* scene, Rectangle viewport, Vector2 middle, vector<MenuWheelPart*> parts, float initAngle): MenuWheel(scene, viewport, middle, parts, initAngle) {
	this->addListener(this);
	nextRenderer = KEEP;
}


Renderers MainMenuWheel::getNextRenderer() {
	return nextRenderer;
}

void MainMenuWheel::menuWheelEvent(MenuWheelPart* clickedPart) {
	switch (clickedPart->getId()) {
		case 2: nextRenderer = HOUSE;
	}
}

void MainMenuWheel::draw(float elapsedTime) {
	MenuWheel::draw(elapsedTime);
	if (prevPart != NULL) {
		Renderer::drawText(Vector4::one(), middle.x + 5, middle.y + 65, prevPart->getTitle());
	}
}

MainMenuWheel* MainMenuWheel::create(Scene* scene, Rectangle viewport) {
	vector<MenuWheelPart*> parts = vector<MenuWheelPart*>(5);
	for (unsigned int i = 0; i < parts.size(); i++) {
		parts[i] = new MenuWheelPart();
		parts[i]->setId(i);
		parts[i]->setTitle(roomNames[i]);
	}
	return new MainMenuWheel(scene, viewport, Vector2::zero(), parts, 0);
}




const char* MainMenuWheel::roomNames[] = {
	"World map",
	"Findings",
	"Homes",
	"Achievements",
	"UNDEFINED",
};