#include "MenuMainRenderer.h"

MenuMainRenderer::MenuMainRenderer(Rectangle viewport) : Renderer(viewport) {
	initialize();
}

void MenuMainRenderer::initialize() {
	nextRenderer = KEEP;
	mainMenuForm = Form::create("res/menu/main.form");
	Control* roomButton = mainMenuForm->getControl("roomButton");
	roomButton->addListener(this, Control::Listener::CLICK);

	
    scene = Scene::create("MainMenuScene");
	Camera* camera = Camera::createOrthographic(viewport.width, viewport.height, viewport.width / viewport.height, 0.25, 600);

    Node* cameraNode = scene->addNode();
    cameraNode->setCamera(camera);
    scene->setActiveCamera(camera);
	cameraNode->translateForward(-100);
	cameraNode->translateX(viewport.width / 2);
	cameraNode->translateY(-viewport.height / 2);

	SAFE_RELEASE(camera);

	menuWheel = MainMenuWheel::create();
}

MenuMainRenderer::~MenuMainRenderer() {
	Control* roomButton = mainMenuForm->getControl("roomButton");
	roomButton->removeListener(this);
	SAFE_RELEASE(mainMenuForm);
	SAFE_DELETE(menuWheel);
	SAFE_RELEASE(scene);
}

void MenuMainRenderer::resizeEvent(unsigned int width, unsigned int height) {
	Renderer::resizeEvent(width, height);
	menuWheel->resizeEvent(width, height);
}

void MenuMainRenderer::update(float elapsedTime) {
	mainMenuForm->update(elapsedTime);
	menuWheel->update(elapsedTime);
}

Renderers MenuMainRenderer::getNextRenderer() {
	if (nextRenderer == KEEP) {
		nextRenderer = menuWheel->getNextRenderer();
	}
	return nextRenderer;
}

void MenuMainRenderer::render(float elapsedTime) {
	mainMenuForm->draw();
	menuWheel->draw(elapsedTime);
}


bool MenuMainRenderer::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging, bool clicked) {
	if (evt == Mouse::MouseEvent::MOUSE_MOVE) {
		// moved mouse
		if (!dragging) {
			return menuWheel->hover(x, y);
		}
	}
	return false;
}

void MenuMainRenderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked) {
	if (evt == Touch::TouchEvent::TOUCH_MOVE) {
		menuWheel->drag(x, y);
	}else if (clicked) {
		menuWheel->click(x, y);
	}
}

void MenuMainRenderer::controlEvent(Control* control, Control::Listener::EventType evt) {
	if (!strcmp("roomButton", control->getId())) {
		nextRenderer = HOUSE;
	}
}