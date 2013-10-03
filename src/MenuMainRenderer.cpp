#include "MenuMainRenderer.h"

MenuMainRenderer::MenuMainRenderer(Rectangle viewport) : Renderer(viewport) {
	initialize();
}

void MenuMainRenderer::initialize() {
	nextRenderer = KEEP;
	mainMenuForm = Form::create("res/menu/main.form");
	Control* roomButton = mainMenuForm->getControl("roomButton");
	roomButton->addListener(this, Control::Listener::CLICK);
}

MenuMainRenderer::~MenuMainRenderer() {
	print("destruct menu \n");
	Control* roomButton = mainMenuForm->getControl("roomButton");
	roomButton->removeListener(this);
	SAFE_RELEASE(mainMenuForm);
}

Renderers MenuMainRenderer::update(float elapsedTime) {
	mainMenuForm->update(elapsedTime);
	return nextRenderer;
}

void MenuMainRenderer::render(float elapsedTime) {
	mainMenuForm->draw();
}

void MenuMainRenderer::controlEvent(Control* control, Control::Listener::EventType evt) {
	if (!strcmp("roomButton", control->getId())) {
		nextRenderer = HOUSE;
	}
}