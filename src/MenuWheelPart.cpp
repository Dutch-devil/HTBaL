#include "MenuWheelPart.h"


Rectangle MenuWheelPart::viewport;
Mesh* MenuWheelPart::mesh = NULL;

MenuWheelPart::MenuWheelPart(): model(NULL) {
	color = NULL;
	realColor = MENU_WHEEL_WHITE;
}

MenuWheelPart::~MenuWheelPart() {
	SAFE_RELEASE(model);
}

void MenuWheelPart::setViewport(Rectangle viewport) {
	MenuWheelPart::viewport = viewport;
    SAFE_RELEASE(mesh);
	mesh = Mesh::createQuad(Vector3::zero(),
						Vector3(0, viewport.height * -.4f, 0),
						Vector3(viewport.height * .19f, 0, 0),
						Vector3(viewport.height * .19f, viewport.height * -.4f, 0));
}

Mesh* MenuWheelPart::getMesh() {
	if (mesh == NULL) {
		setViewport(Rectangle());
	}
	return mesh;
}

float MenuWheelPart::getMeshHeight() {
	return viewport.height * .4f;
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
