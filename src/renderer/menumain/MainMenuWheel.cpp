#include "MainMenuWheel.h"

MainMenuWheel::MainMenuWheel(Vector2 middle, vector<MenuWheelPart*> parts, float radius, float initAngle): MenuWheel(middle, parts, radius, initAngle) {
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

MainMenuWheel* MainMenuWheel::create() {
	vector<MenuWheelPart*> parts = vector<MenuWheelPart*>(5);
	for (unsigned int i = 0; i < parts.size(); i++) {
		parts[i] = new MenuWheelPart();
		parts[i]->setId(i);
		parts[i]->setTitle(roomNames[i]);
	}
	return new MainMenuWheel(Vector2::zero(), parts, 300, 0);
}




const char* MainMenuWheel::roomNames[] = {
	"World map",
	"Findings",
	"Homes",
	"Achievements",
	"UNDEFINED",
};