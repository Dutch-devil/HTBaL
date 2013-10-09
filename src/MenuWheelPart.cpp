#include "MenuWheelPart.h"


float MenuWheelPart::size;
Mesh* MenuWheelPart::mesh = NULL;

MenuWheelPart::MenuWheelPart(): model(NULL), title("UNDEFINED"), id(-1) {
	color = NULL;
	realColor = MENU_WHEEL_WHITE;
}

MenuWheelPart::~MenuWheelPart() {
	SAFE_RELEASE(model);
	//SAFE_DELETE_ARRAY(title);
}

void MenuWheelPart::setSize(float size) {
	MenuWheelPart::size = size;
    SAFE_RELEASE(mesh);
	mesh = Mesh::createQuad(Vector3::zero(),
						Vector3(0, size * -.4f, 0),
						Vector3(size * .19f, 0, 0),
						Vector3(size * .19f, size * -.4f, 0));
}

Mesh* MenuWheelPart::getMesh() {
	if (mesh == NULL) {
		setSize(0);
	}
	return mesh;
}

float MenuWheelPart::getMeshSize(float relativeSize) {
	return relativeSize * .4f;
}

void MenuWheelPart::releaseMesh() {
	SAFE_RELEASE(mesh);
}

Node* MenuWheelPart::getNode(){
	return model->getNode();
}

Model* MenuWheelPart::getModel() {
    if (model == NULL) {
        model = Model::create(getMesh());

        // Create the ground material
		Material* material = MaterialManager::getMaterial(MENU_WHEEL);
        model->setMaterial(material);
		SAFE_RELEASE(material);
    }
    return model;
}


void MenuWheelPart::setHover(bool hover) {
	this->hover = hover;
	updateColor();
}

void MenuWheelPart::updateColor() {
	SAFE_DELETE(color);
	if (hover) {
		color = MENU_WHEEL_HOVER;
	}else {
        color = new Vector3(*realColor);
    }
    model->getMaterial()->getTechnique()->getPass("0")->getParameter("u_ambientColor")->setVector3(blendColors(color, realColor));
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
