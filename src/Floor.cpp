
#include "Floor.h"


float Floor::width, Floor::height;

Floor::Floor(int id, RenderState::StateBlock* stateBlock, float x, float y): id(id), x(x), y(y) {
    this->model = Model::create(getMesh());

    realColor = color = FLOOR_UNSELECTED;
    selected = false;

    model->setMaterial("res/house/floor.material")->setStateBlock(stateBlock);
    setColor(color);
}
 
Mesh* Floor::getMesh() {
    return Mesh::createQuad(Vector3(-Floor::width / 2, -Floor::height / 2, 0),
                            Vector3(Floor::width / 2, -Floor::height / 2, 0),
                            Vector3(-Floor::width / 2, Floor::height / 2, 0),
                            Vector3(Floor::width / 2, Floor::height / 2, 0));
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
    return color->x == other->x && color->y == other->y && color->z == other->z;
}

void Floor::setColor(Vector3* color) {
    this->realColor = color;
	updateColor();
}

void Floor::updateColor() {
    model->getMaterial()->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(blendColors(color, realColor));
}

Vector3 Floor::blendColors(Vector3* color1, Vector3* color2) {
    return .5 * (*color1) + .5 * (*color2);
}

bool Floor::getSelected() {
	return selected;
}

Floor* Floor::setSelected(bool selected) {
	if (getSelected() != selected) {
		toggleSelect();
	}
	return this;
}

void Floor::toggleColor(Vector3* first, Vector3* second) {
    // toggle the color
    if (!this->isColor(first)) {
        this->setColor(first);
    } else {
        this->setColor(second);
    }
}

void Floor::toggleSelect() {
    selected = !selected;
    if (selected) {
        color = FLOOR_SELECTED;
    } else {
        color = FLOOR_UNSELECTED;
    }
	updateColor();
}