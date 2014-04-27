#include "MenuWheel.h"


MenuWheel::MenuWheel() {}

MenuWheel::~MenuWheel() {
	for (MenuWheelPart* part : wheelParts) {
		SAFE_DELETE(part);
	}
	SAFE_DELETE(spriteBatch);
}

MenuWheel* MenuWheel::create(const char* id, Theme::Style* style) {
	MenuWheel* menuWheel = new MenuWheel();
	menuWheel->_id = id ? id : "";
	menuWheel->initialize("MenuWheel", style, NULL);
	return menuWheel;
}

Control* MenuWheel::create(Theme::Style* style, Properties* properties) {
	MenuWheel* menuWheel = new MenuWheel();
	menuWheel->initialize("MenuWheel", style, properties);
	return menuWheel;
}

void MenuWheel::initialize(const char* typeName, Theme::Style* style, Properties* properties) {
	Control::initialize(typeName, style, properties);

	unsigned int radius = 0;
	if (properties) {
		// we need a radius for our menuwheel
		radius = abs(properties->getInt("radius"));
		//initAngle = properties->getInt("initAngle");
		
		const char* orientationVal = properties->getString("orientation", "TOP_LEFT");
		if (!strcmpnocase("top_left", orientationVal)) {
			orientation = Orientation::TOP_LEFT;
		} else if (!strcmpnocase("top_right", orientationVal)) {
			orientation = Orientation::TOP_RIGHT;
		} else if (!strcmpnocase("bottom_left", orientationVal)) {
			orientation = Orientation::BOTTOM_LEFT;
		} else if (!strcmpnocase("bottom_right", orientationVal)) {
			orientation = Orientation::BOTTOM_RIGHT;
		} else {
			GP_ERROR("Invalid orientation given");
		}

		// create the parts from all child controls
		int id = 0;
		Properties* wheelPartNamespace = properties->getNextNamespace();
		while (wheelPartNamespace != NULL) {
			const char* controlName = wheelPartNamespace->getNamespace();
			if (strcmpnocase(controlName, "wheelpart")) {
				GP_WARN("Only wheelparts can be added to a menu wheel");
			} else {
				MenuWheelPart* menuWheelPart = new MenuWheelPart();
				menuWheelPart->setId(id);
				menuWheelPart->setTitle(wheelPartNamespace->getString("title", "UNKOWN"));
				menuWheelPart->rotate(WHEEL_ANGLE * id);	// set initial rotation of each wheel part
				wheelParts.push_back(menuWheelPart);
				id++;
			}
			wheelPartNamespace = properties->getNextNamespace();
		}
	}
	if (wheelParts.empty()) {
		GP_ERROR("At least one Wheel part is required");
	}

	spriteBatch = SpriteBatch::create("res/img/MenuWheelPart.png");

	prevPart = clickPart = NULL;
	startAngle = 0;

	initAngle = 0;
	switch (orientation) {
		case Orientation::BOTTOM_LEFT: initAngle = MATH_PI * 3 / 2; break;
		case Orientation::BOTTOM_RIGHT: initAngle = MATH_PI; break;
		case Orientation::TOP_RIGHT: initAngle = MATH_PI / 2; break;
	}
	minRotation = -WHEEL_ANGLE * (wheelParts.size() - 1) + initAngle;
	maxRotation = -MATH_PI / 3 + initAngle;
	rotation = (minRotation + maxRotation) / 2;


	if (radius == 0) {
		scale = 1;
	} else {
		scale = radius / MESH_RADIUS;
	}
	spriteWidth = MESH_WIDTH * scale;
	spriteHeight = MESH_HEIGHT * scale;

	font = Font::create("res/arial14.gpb");

	// make it input ready
	setConsumeInputEvents(true);
	setCanFocus(true);
	setSize(radius, radius);

	switch (orientation) {
		case TOP_LEFT: shiftX = shiftY = 0; break;
		case TOP_RIGHT: shiftX = getWidth(); shiftY = 0; break;
		case BOTTOM_RIGHT: shiftX = shiftY = getWidth(); break;
		case BOTTOM_LEFT: shiftX = 0; shiftY = getHeight(); break;
	}
}

unsigned int MenuWheel::drawText(Form* form, const Rectangle& clip) {
	float x, y;
	Font::Justify align;
	switch (orientation) {
		case TOP_LEFT:
			x = getX() + 5;
			y = getY() + 45;
			align = Font::ALIGN_LEFT;
			break;
		case BOTTOM_LEFT:
			x = getX() + 5;
			y = getY() + getHeight() - 45 - font->getSize();
			align = Font::ALIGN_LEFT;
			break;
		case TOP_RIGHT:
			x = getX() + 5;
			y = getY() + 45;
			align = Font::ALIGN_RIGHT;
			break;
		case BOTTOM_RIGHT:
			x = getX() + 5;
			y = getY() + getHeight() - 45 - font->getSize();
			align = Font::ALIGN_RIGHT;
			break;
	}
	// Draw the text.
	if (prevPart != NULL) {
		SpriteBatch* batch = font->getSpriteBatch(font->getSize());
		startBatch(form, batch);
		font->drawText(prevPart->getTitle(), Rectangle(x, y, getWidth() - 10, 20), Vector4::one(), font->getSize(), align);
		finishBatch(form, batch);
		return 1;
	}
	return 0;
}

unsigned int MenuWheel::drawImages(Form* form, const Rectangle& clip) {
	int startIndex = min((unsigned int) ((-(rotation - maxRotation)) / WHEEL_ANGLE), (unsigned int) wheelParts.size() - 1);
	SpriteBatch* batch = _style->getTheme()->getSpriteBatch();
	//startBatch(form, batch);
	spriteBatch->start();
	for (int index = min(startIndex + 3, (int) wheelParts.size() - 1); index >= startIndex; index--) {
		spriteBatch->draw(getX() + shiftX, getY() + shiftY + (MESH_RADIUS - MESH_HEIGHT) * scale, 0, spriteWidth, spriteHeight, 0, 0, 1, 1, wheelParts[index]->getColor(), Vector2(0, -(MESH_RADIUS - MESH_HEIGHT) / (MESH_HEIGHT)), wheelParts[index]->getRotation() + rotation);
	}
	spriteBatch->finish();
	//finishBatch(form, batch);
	return min(startIndex + 4, (int) wheelParts.size()) - startIndex;
}


bool MenuWheel::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta, bool dragging, bool clicked) {
	if (evt == Mouse::MouseEvent::MOUSE_MOVE) {
		// moved mouse
		if (!dragging) {
			return hover(x, y);
		}
	}
	return false;
}

bool MenuWheel::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked) {
	if (evt == Touch::TouchEvent::TOUCH_MOVE) {
		return drag(x, y);
	} else if (clicked) {
		return click(x, y);
	}
	return false;
}

bool MenuWheel::keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags) {
	return false;
}

bool MenuWheel::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta) {
	return GenericInputListener::mouseEvent(evt, x, y, wheelDelta);
}

bool MenuWheel::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {
	return GenericInputListener::touchEvent(evt, x, y, contactIndex);
}

bool MenuWheel::hover(int x, int y) {
	startAngle = getCorrectedAngle(x, y);
	x = shiftX - x;
	y = shiftY - y;
	float dist = x * x + y * y;
	if (dist > getHeight() * getHeight() || dist < INNER_DIST * getHeight() * getHeight()) {
		// outside wheel, stop here
		if (prevPart != NULL) {
			prevPart->setHover(false);
			prevPart = NULL;
		}
		return false;
	}
	unsigned int partIndex = (int) ((WHEEL_ANGLE - rotation - startAngle) / WHEEL_ANGLE);
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
	float newAngle = getCorrectedAngle(x, y);
	rotation = max(min(maxRotation, rotation - newAngle + startAngle), minRotation);
	startAngle = newAngle;
	return true;
}

bool MenuWheel::click(int x, int y) {
	if (prevPart != NULL) {
		clickPart = prevPart;
		notifyListeners(Listener::EventType::CLICK);
		return true;
	}
	return false;
}

MenuWheelPart* MenuWheel::getActivePart() {
	return clickPart;
}

void MenuWheel::translate(float x, float y) {
	setPosition(getX() + x, getY() + y);
}

float MenuWheel::getCorrectedAngle(float x, float y) {
	float correctedY = shiftY - y;
	float correctedX = shiftX - x;
	float angle = atan(correctedX / correctedY);
	if (orientation == BOTTOM_LEFT || orientation == BOTTOM_RIGHT && correctedY >= 0) {
		angle = angle - MATH_PI;
	}
	// correct angle once more, when we have an overflow
	if (correctedY >= 0) {
		if (orientation == TOP_LEFT) {
			angle = angle + MATH_PI;
		} else if (orientation == TOP_RIGHT) {
			angle = angle - MATH_PI;
		}
	} else if (orientation == BOTTOM_LEFT) {
		angle = angle - MATH_PI;
	}
	return angle;
}


const char* MenuWheel::getType() const {
	return "menuwheel";
}
