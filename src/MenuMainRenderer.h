#ifndef MenuMainRenderer_H_
#define MenuMainRenderer_H_

#include "gameplay.h"
#include "Renderer.h"

using namespace gameplay;

class MenuMainRenderer: public Renderer, Control::Listener {
public:
	MenuMainRenderer();
	MenuMainRenderer(Scene* scene);
	~MenuMainRenderer();
	Renderers update(float elapsedTime);
	void render(float elapsedTime);

	void controlEvent(Control* control, Control::Listener::EventType evt);

private:
	void initialize();
	Form* mainMenuForm;
	Renderers nextRenderer;
};


#endif