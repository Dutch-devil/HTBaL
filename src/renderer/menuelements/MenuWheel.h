#ifndef MenuWheel_H_
#define MenuWheel_H_

#define WHEEL_ANGLE (MATH_PI / 6)		// what is the angle of a part of the wheel?
#define INNER_DIST ((float)2 / 4)		// what part of the inner side of the wheel shouldn't count?

class MenuWheel;

#include "gameplay.h"
#include <vector>
#include <cmath>
#include "../Renderer.h"
#include "MenuWheelPart.h"

#define MESH_HEIGHT	400.0f
#define MESH_WIDTH	190.0f

using namespace gameplay;
using namespace std;

class MenuWheel {
public:
	class Listener {
		public: virtual void menuWheelEvent(MenuWheelPart*) = 0;
	};

	MenuWheel(Vector2 middle, vector<MenuWheelPart*> parts, float radius);
	MenuWheel(Vector2 middle, vector<MenuWheelPart*> parts, float radius, float initAngle);
    ~MenuWheel();
	
	void initialize(float radius);
	void addListener(Listener* listener);

	virtual void resizeEvent(unsigned int width, unsigned int height);
	virtual void update(float elapsedTime);
	virtual void draw(float elapsedTime);

	bool hover(int x, int y);
	bool drag(int x, int y);
	bool click(int x, int y);

	void translate(float x, float y);

protected:
	SpriteBatch* spriteBatch;

	Vector2 middle;
	list<Listener*> listeners;
	vector<MenuWheelPart*> wheelParts;
	MenuWheelPart* prevPart;
	unsigned int prevPartIndex;

	float rotation, startAngle, minRotation, maxRotation, initAngle;
	float scale;
	float width, height;
};

#endif