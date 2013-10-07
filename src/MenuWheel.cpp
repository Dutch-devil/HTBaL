#include "MenuWheel.h"

MenuWheel::MenuWheel(Scene* scene, Rectangle viewport, unsigned int parts): viewport(viewport) {
	node = scene->addNode();
	MenuWheelPart::setViewport(viewport);
	wheelParts = vector<MenuWheelPart*>(parts);
	for (unsigned int i = 0; i < parts; i++) {
		wheelParts[i] = new MenuWheelPart();
		Node* partNode = scene->addNode();
		node->addChild(partNode);
		partNode->setModel(wheelParts[i]->getModel());
		partNode->rotateZ(WHEEL_ANGLE * i);
	}

	prevPart = NULL;
	rotation = 0;
	startAngle = -1;
	maxRotation = -WHEEL_ANGLE * parts + MATH_PI / 2;
}

MenuWheel::~MenuWheel() {
	for (MenuWheelPart* part : wheelParts) {
		SAFE_DELETE(part);
	}
	MenuWheelPart::releaseMesh();
}

void MenuWheel::update(float elapsedTime) {

}

void MenuWheel::draw(float elapsedTime) {
	for (vector<MenuWheelPart*>::iterator itr = wheelParts.end() - 1; itr >= wheelParts.begin(); itr--) {
		(*itr)->getModel()->draw();
	}
	if (prevPart != NULL) {
		char* buf = "UNDEFINED";
		switch (prevPartIndex) {
			case 0: buf = "To home"; break;
		}
		Renderer::drawText(Vector4::one(), 5, 65, buf);
	}
}

bool MenuWheel::hover(int x, int y) {
	startAngle = atan((float)x / y);
	float meshHeight = MenuWheelPart::getMeshHeight();
	float dist = Vector2::zero().distanceSquared(Vector2(x, y));
	if (x < 0 || y < 0 || x > meshHeight || y > meshHeight || dist > meshHeight * meshHeight  || dist < INNER_DIST * meshHeight * meshHeight) {
		// outside wheel, stop here
		if (prevPart != NULL) {
			prevPart->setHover(false);
			prevPart = NULL;
		}
		return false;
	}
	unsigned int partIndex = (int)((atan((float)x / y) - rotation) / WHEEL_ANGLE);
	if (partIndex < wheelParts.size() && prevPart != wheelParts[partIndex]) {
		// other part hover, make it hovered
		if (prevPart != NULL) {
			prevPart->setHover(false);
		}
		prevPartIndex = partIndex;
		prevPart = wheelParts[partIndex];
		prevPart->setHover(true);
	}
	return true;
}

bool MenuWheel::drag(int x, int y) {
	if (prevPart == NULL) {
		return false;
	}
	float newAngle = atan((float)x / y);
	rotation = max(min(0.0f, rotation + newAngle - startAngle), maxRotation);
	for (unsigned int i = 0; i < wheelParts.size(); i++) {
		wheelParts[i]->getModel()->getNode()->setRotation(Vector3(0, 0, 1), WHEEL_ANGLE * i + rotation);
	}
	startAngle = newAngle;
	return true;
}

Renderers MenuWheel::click(int x, int y) {
	if (prevPart == NULL) {
		return KEEP;
	}
	switch (prevPartIndex) {
		case 0: return HOUSE;
		default: return KEEP;
	}
}
