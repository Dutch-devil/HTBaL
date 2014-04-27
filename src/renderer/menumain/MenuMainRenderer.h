#ifndef MenuMainRenderer_H_
#define MenuMainRenderer_H_

class MenuMainRenderer;

#include "gameplay.h"
#include "../Renderer.h"

using namespace gameplay;

class MenuMainRenderer: public Renderer, Control::Listener {
public:
	MenuMainRenderer(Rectangle viewport);
	~MenuMainRenderer();
	void resizeEvent(unsigned int width, unsigned int height);
	void update(float elapsedTime);
	Renderers MenuMainRenderer::getNextRenderer();
	void render(float elapsedTime);
	
	bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging, bool clicked);
	bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked);
	bool keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags);
	void controlEvent(Control* control, Control::Listener::EventType evt);

protected:
	void initialize();
	Form* mainMenuForm;
	Renderers nextRenderer;

	Scene* scene;

	Renderers getRendererFromTitle(const char* title);
};


#endif