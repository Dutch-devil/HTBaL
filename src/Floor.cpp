
#include "Floor.h"
#include "MaterialManager.h"


float Floor::width, Floor::height;

Floor::Floor(int id, float x, float y): id(id), x(x), y(y) {
    this->model = Model::create(getMesh());

    realColor = color = FLOOR_UNSELECTED;
    hover = selected = false;

    model->setMaterial(MaterialManager::getMaterial(FLOOR));
    setColor(color);
}

Floor::~Floor() {
	SAFE_DELETE(color);
	SAFE_DELETE(realColor);
	SAFE_RELEASE(model);
}
 
Mesh* Floor::getMesh() {
    return Mesh::createQuad(Vector3(-Floor::width / 2, -Floor::height / 2, 0),
                            Vector3(Floor::width / 2, -Floor::height / 2, 0),
                            Vector3(-Floor::width / 2, Floor::height / 2, 0),
                            Vector3(Floor::width / 2, Floor::height / 2, 0));
}

Floor* Floor::setDoor(WallDir dir) {
	this->doorDir = dir;
	return this;
}

WallDir Floor::getDoor() {
	return doorDir;
}

int Floor::getId() {
	return id;
}

float Floor::getX() {
	return x;
}
float Floor::getY() {
	return y;
}

Model* Floor::getModel() {
    return model;
}

Vector3* Floor::getColor() {
    return color;
}

bool Floor::isColor(Vector3* other) {
    return realColor->x == other->x && realColor->y == other->y && realColor->z == other->z;
}

Floor* Floor::setColor(Vector3* color) {
    this->realColor = color;
	if (!getSelected()) {
		this->color = realColor;
	}
	updateColor();
	return this;
}

void Floor::updateColor() {
	if (selected && hover) {
        color = &FLOOR_HOVER_SELECTED;
    } else if (selected) {
		color = FLOOR_SELECTED;
	}else if (hover) {
		color = FLOOR_HOVER;
	}else {
        color = realColor;
    }
    model->getMaterial()->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(blendColors(color, realColor));
}

Vector3 Floor::blendColors(Vector3* color1, Vector3* color2) {
    return .5 * (*color1) + .5 * (*color2);
}

void Floor::toggleColor(Vector3* first, Vector3* second) {
    // toggle the color
    if (!this->isColor(first)) {
        this->setColor(first);
    } else {
        this->setColor(second);
    }
}

bool Floor::getSelected() {
	return selected;
}

Floor* Floor::setSelected(bool selected) {
	this->selected = selected;
	updateColor();
	return this;
}

Floor* Floor::toggleSelect() {
    selected = !selected;
	updateColor();
	return this;
}

Floor* Floor::setHover(bool hover) {
	this->hover = hover;
	updateColor();
	return this;
}