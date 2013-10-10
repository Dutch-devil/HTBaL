#ifndef MenuWheel_H_
#define MenuWheel_H_

#define WHEEL_ANGLE (MATH_PI / 6)		// what is the angle of a part of the wheel?
#define INNER_DIST ((float)2 / 4)				// what part of the inner side of the wheel shouldn't count?

class MenuWheel;

#include "gameplay.h"
#include <vector>
#include <cmath>
#include "../Renderer.h"
#include "MenuWheelPart.h"

using namespace gameplay;
using namespace std;

class MenuWheel {
public:
	class Listener {
		public: virtual void menuWheelEvent(MenuWheelPart*) = 0;
	};

	MenuWheel(Scene* scene, Rectangle viewport, Vector2 middle, vector<MenuWheelPart*> parts);
	MenuWheel(Scene* scene, Rectangle viewport, Vector2 middle, vector<MenuWheelPart*> parts, float initAngle);
    ~MenuWheel();
	
	void initialize(Scene* scene);
	void addListener(Listener* listener);
	
	Node* getNode();

	void update(float elapsedTime);
	virtual void draw(float elapsedTime);

	bool hover(int x, int y);
	bool drag(int x, int y);
	bool click(int x, int y);

protected:
	Rectangle viewport;
	Vector2 middle;
	list<Listener*> listeners;
	Node* node;
	vector<MenuWheelPart*> wheelParts;
	MenuWheelPart* prevPart;
	unsigned int prevPartIndex;

	float initAngle, rotation, startAngle, minRotation;
};

#endif