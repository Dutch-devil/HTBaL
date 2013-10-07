#ifndef MenuWheel_H_
#define MenuWheel_H_

#define WHEEL_ANGLE (MATH_PI / 6)		// what is the angle of a part of the wheel?
#define INNER_DIST ((float)2 / 4)				// what part of the inner side of the wheel shouldn't count?

#include "gameplay.h"
#include <vector>
#include <cmath>
#include "MaterialManager.h"
#include "MenuWheelPart.h"
#include "Renderer.h"

using namespace gameplay;
using namespace std;

class MenuWheel {
public:
	MenuWheel(Scene* scene, Rectangle viewport, unsigned int parts);
    ~MenuWheel();

	void update(float elapsedTime);
	void draw(float elapsedTime);

	bool hover(int x, int y);
	bool drag(int x, int y);
	Renderers click(int x, int y);

protected:
	Rectangle viewport;
	Node* node;
	vector<MenuWheelPart*> wheelParts;
	MenuWheelPart* prevPart;
	unsigned int prevPartIndex;

	float rotation, startAngle, maxRotation;
};

#endif