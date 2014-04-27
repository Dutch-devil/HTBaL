#include "MenuWheel.h"

MenuWheel::MenuWheel(Vector2 middle, vector<MenuWheelPart*> parts, float radius) : middle(middle), wheelParts(parts) {
	initialize(radius);
}

MenuWheel::MenuWheel(Vector2 middle, vector<MenuWheelPart*> parts, float radius, float initAngle): middle(middle), wheelParts(parts), initAngle(initAngle) {
	initialize(radius);
}

MenuWheel::~MenuWheel() {
	for (MenuWheelPart* part : wheelParts) {
		SAFE_DELETE(part);
	}
	SAFE_DELETE(spriteBatch);
}

void MenuWheel::initialize(float radius) {
	spriteBatch = SpriteBatch::create("res/img/MenuWheelPart.png");
	for (unsigned int i = 0; i < wheelParts.size(); i++) {
		wheelParts[i]->rotate(WHEEL_ANGLE * i);
	}

	prevPart = NULL;
	startAngle = 0;
	minRotation = -WHEEL_ANGLE * (wheelParts.size() - 1) + initAngle;
	maxRotation = -WHEEL_ANGLE * 2 + initAngle;
	rotation = (minRotation + maxRotation) / 2;

	scale = radius / MESH_HEIGHT;
	width = MESH_WIDTH * scale;
	height = MESH_HEIGHT * scale;
}

void MenuWheel::addListener(Listener* listener) {
	this->listeners.push_back(listener);
}

void MenuWheel::resizeEvent(unsigned int width, unsigned int height) {
	Matrix projectionMatrix;
	Matrix::createOrthographicOffCenter(0, width, height, 0, 0, 1, &projectionMatrix);
	spriteBatch->setProjectionMatrix(projectionMatrix);
}

void MenuWheel::update(float elapsedTime) {

}

void MenuWheel::draw(float elapsedTime) {
	int startIndex = min((unsigned int)((-(rotation - maxRotation)) / WHEEL_ANGLE), (unsigned int)wheelParts.size() - 1);
	spriteBatch->start();
	for (int index = min(startIndex + 3, (int) wheelParts.size() - 1); index >= startIndex; index--) {
		spriteBatch->draw(middle.x, middle.y, 0, width, height, 0, 0, 1, 1, wheelParts[index]->getColor(), Vector2::zero(), wheelParts[index]->getRotation() + rotation);
	}
	spriteBatch->finish();
}

bool MenuWheel::hover(int x, int y) {
	startAngle = atan((float)(middle.x - x) / (middle.y - y));
	float dist = middle.distanceSquared(Vector2(x, y));
	if (x < middle.x - height || y < middle.y - height || x > middle.x + height || y > middle.y + height ||
		dist > height * height || dist < INNER_DIST * height * height) {
		// outside wheel, stop here
		if (prevPart != NULL) {
			prevPart->setHover(false);
			prevPart = NULL;
		}
		return false;
	}
	unsigned int partIndex = wheelParts.size() + (int) ((minRotation + maxRotation - startAngle - rotation - initAngle) / WHEEL_ANGLE) + 1;
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
	rotation = max(min(maxRotation, rotation - newAngle + startAngle), minRotation);
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

void MenuWheel::translate(float x, float y) {
	middle.x += x;
	middle.y += y;
}
