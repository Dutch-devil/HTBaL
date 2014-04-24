#include "MenuWheelPart.h"


MenuWheelPart::MenuWheelPart(): title("UNDEFINED"), id(-1) {
	realColor = MENU_WHEEL_WHITE;
	color = Vector3(*realColor);
	rotation = 0;
}

MenuWheelPart::~MenuWheelPart() {
	SAFE_DELETE(realColor);
}


void MenuWheelPart::setHover(bool hover) {
	this->hover = hover;
	updateColor();
}

void MenuWheelPart::updateColor() {
	if (hover) {
		Vector3* blend = MENU_WHEEL_HOVER;
		color = blendColors(realColor, blend);
		SAFE_DELETE(blend);
	}else {
        color = Vector3(*realColor);
    }
}

Vector3 MenuWheelPart::blendColors(Vector3* color1, Vector3* color2) {
    return .5 * (*color1) + .5 * (*color2);
}

int MenuWheelPart::getId() {
	return id;
}

void MenuWheelPart::setId(int id) {
	this->id = id;
}
const char* MenuWheelPart::getTitle() {
	return title;
}

void MenuWheelPart::setTitle(const char* title) {
	this->title = title;
}

void MenuWheelPart::rotate(float angle) {
	rotation += angle;
}

float MenuWheelPart::getRotation() {
	return rotation;
}

Vector4 MenuWheelPart::getColor() {
	return Vector4(color.x, color.y, color.z, 1);
}