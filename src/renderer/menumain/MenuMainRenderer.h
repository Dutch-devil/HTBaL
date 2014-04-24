#ifndef MenuMainRenderer_H_
#define MenuMainRenderer_H_

class MenuMainRenderer;

#include "gameplay.h"
#include "../Renderer.h"
#include "MainMenuWheel.h"

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
	void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked);
	void controlEvent(Control* control, Control::Listener::EventType evt);

protected:
	void initialize();
	Form* mainMenuForm;
	Renderers nextRenderer;

	Scene* scene;
	MainMenuWheel* menuWheel;
};


#endif