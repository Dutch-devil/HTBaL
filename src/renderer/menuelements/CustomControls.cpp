#include "CustomControls.h"


void CustomControls::initialize() {
	ControlFactory* controlFactory = ControlFactory::getInstance();
	controlFactory->registerCustomControl("MENUWHEEL", &MenuWheel::create);
}

void CustomControls::finalize() {
	ControlFactory* controlFactory = ControlFactory::getInstance();
	controlFactory->unregisterCustomControl("MENUWHEEL");
}
