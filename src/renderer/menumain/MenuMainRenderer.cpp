#include "MenuMainRenderer.h"

MenuMainRenderer::MenuMainRenderer(Rectangle viewport) : Renderer(viewport) {
	initialize();
}

void MenuMainRenderer::initialize() {
	nextRenderer = KEEP;
	mainMenuForm = Form::create("res/menu/main.form");
	Control* roomButton = mainMenuForm->getControl("roomButton");
	roomButton->addListener(this, Control::Listener::CLICK);
	Control* menuWheel = mainMenuForm->getControl("mainMenuWheel");
	menuWheel->addListener(this, Control::Listener::CLICK);

	
    scene = Scene::create("MainMenuScene");
	Camera* camera = Camera::createOrthographic(viewport.width, viewport.height, viewport.width / viewport.height, 0.25, 600);

    Node* cameraNode = scene->addNode();
    cameraNode->setCamera(camera);
    scene->setActiveCamera(camera);
	cameraNode->translateForward(-100);
	cameraNode->translateX(viewport.width / 2);
	cameraNode->translateY(-viewport.height / 2);

	SAFE_RELEASE(camera);
}

MenuMainRenderer::~MenuMainRenderer() {
	Control* roomButton = mainMenuForm->getControl("roomButton");
	roomButton->removeListener(this);
	SAFE_RELEASE(mainMenuForm);
	SAFE_RELEASE(scene);
}

void MenuMainRenderer::resizeEvent(unsigned int width, unsigned int height) {
	Renderer::resizeEvent(width, height);
}

void MenuMainRenderer::update(float elapsedTime) {
	mainMenuForm->update(elapsedTime);
}

Renderers MenuMainRenderer::getNextRenderer() {
	if (nextRenderer == KEEP) {
		//nextRenderer = menuWheel->getNextRenderer();
	}
	return nextRenderer;
}

void MenuMainRenderer::render(float elapsedTime) {
	mainMenuForm->draw();
}


bool MenuMainRenderer::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging, bool clicked) {
	return false;
}

bool MenuMainRenderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked) {
	return false;
}

bool MenuMainRenderer::keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags) {
	return false;
}

void MenuMainRenderer::controlEvent(Control* control, Control::Listener::EventType evt) {
	if (!strcmp("roomButton", control->getId())) {
		nextRenderer = HOUSE;
	} else if (!strcmp("mainMenuWheel", control->getId())) {
		MenuWheel* menuWheel = (MenuWheel*) control;
		MenuWheelPart* activePart = menuWheel->getActivePart();
		if (activePart != NULL) {
			nextRenderer = getRendererFromTitle(activePart->getTitle());
		}
	}
}

Renderers MenuMainRenderer::getRendererFromTitle(const char* title) {
	if (!strcmpnocase("Homes", title)) {
		return Renderers::HOUSE;
	} else if (!strcmpnocase("World map", title)) {
		return Renderers::MAP;
	}
	return Renderers::KEEP;
}