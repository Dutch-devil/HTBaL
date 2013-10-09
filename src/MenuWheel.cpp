#include "MenuWheel.h"

MenuWheel::MenuWheel(Scene* scene, Rectangle viewport, Vector2 middle, vector<MenuWheelPart*> parts): viewport(viewport), middle(middle), wheelParts(parts), initAngle(0) {
	initialize(scene);
}

MenuWheel::MenuWheel(Scene* scene, Rectangle viewport, Vector2 middle, vector<MenuWheelPart*> parts, float initAngle): viewport(viewport), middle(middle), wheelParts(parts), initAngle(initAngle) {
	initialize(scene);
}

MenuWheel::~MenuWheel() {
	for (MenuWheelPart* part : wheelParts) {
		SAFE_DELETE(part);
	}
	MenuWheelPart::releaseMesh();
}

void MenuWheel::initialize(Scene* scene) {
	node = scene->addNode();
	for (unsigned int i = 0; i < wheelParts.size(); i++) {
		Node* partNode = scene->addNode();
		node->addChild(partNode);
		partNode->setModel(wheelParts[i]->getModel());
		partNode->rotateZ(WHEEL_ANGLE * i);
		partNode->translateZ(i);
	}

	prevPart = NULL;
	rotation = initAngle - .5 * wheelParts.size() * WHEEL_ANGLE + MATH_PI / 4;
	node->rotateZ(rotation);
	startAngle = -1;
	minRotation = -WHEEL_ANGLE * wheelParts.size() + MATH_PI / 2 + initAngle;
}

void MenuWheel::addListener(Listener* listener) {
	this->listeners.push_back(listener);
}

Node* MenuWheel::getNode() {
	return node;
}

void MenuWheel::update(float elapsedTime) {

}

void MenuWheel::draw(float elapsedTime) {
	int startIndex = min((unsigned int)((-(rotation - initAngle)) / WHEEL_ANGLE), (unsigned int)wheelParts.size() - 1);
	for (int index = startIndex; index <= min(startIndex + 3, (int)wheelParts.size() - 1); index++) {
		wheelParts[index]->getModel()->draw();
	}
}

bool MenuWheel::hover(int x, int y) {
	startAngle = atan((float)(middle.x - x) / (middle.y - y));
	float meshHeight = MenuWheelPart::getMeshSize(viewport.height);
	float dist = middle.distanceSquared(Vector2(x, y));
	if (x < middle.x - meshHeight || y < middle.y - meshHeight || x > middle.x + meshHeight || y > middle.y + meshHeight || 
				dist > meshHeight * meshHeight  || dist < INNER_DIST * meshHeight * meshHeight) {
		// outside wheel, stop here
		if (prevPart != NULL) {
			prevPart->setHover(false);
			prevPart = NULL;
		}
		return false;
	}
	unsigned int partIndex = (int)((startAngle - (rotation - initAngle)) / WHEEL_ANGLE);
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
	float newAngle = atan((float)(middle.x - x) / (middle.y - y));
	rotation = max(min(initAngle, rotation + newAngle - startAngle), minRotation);
	node->setRotation(Vector3(0, 0, 1), rotation);
	startAngle = newAngle;
	return true;
}

bool MenuWheel::click(int x, int y) {
	if (prevPart != NULL) {
		for (Listener* listener : listeners) {
			listener->menuWheelEvent(prevPart);
		}
		return true;
	}
	return false;
}
