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
#include "../GenericInputListener.h"

#define MESH_HEIGHT	160.0f
#define MESH_WIDTH	190.0f
#define MESH_RADIUS	400.0f

using namespace gameplay;
using namespace std;

class MenuWheel : public Control, GenericInputListener {
	friend class Container;
	friend class CustomControls;

	enum Orientation {
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT
	};

public:

	/**
	* Creates a new label.
	*
	* @param id The label id.
	* @param style The label style (optional).
	*
	* @return The new label.
	* @script{create}
	*/
	static MenuWheel* create(const char* id, Theme::Style* style = NULL);

	MenuWheelPart* getActivePart();

protected:
	SpriteBatch* spriteBatch;

	vector<MenuWheelPart*> wheelParts;
	MenuWheelPart* prevPart, *clickPart;
	unsigned int prevPartIndex;

	Orientation orientation;
	float rotation, startAngle, minRotation, maxRotation, initAngle;
	float scale;
	float spriteWidth, spriteHeight;

	float shiftX, shiftY;

	Font* font;

	MenuWheel();
	virtual ~MenuWheel();

	/**
	* Create a button with a given style and properties.
	*
	* @param style The style to apply to this button.
	* @param properties A properties object containing a definition of the button (optional).
	*
	* @return The new button.
	*/
	static Control* create(Theme::Style* style, Properties* properties = NULL);

	/**
	* @see Control::initialize
	*/
	void initialize(const char* typeName, Theme::Style* style, Properties* properties);

	bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta, bool dragging, bool clicked);
	bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked);
	bool keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags);

	bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);
	bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);


	/**
	* @see Control::drawText
	*/
	virtual unsigned int drawText(Form* form, const Rectangle& clip);

	/**
	* @see Control::drawImages
	*/
	unsigned int drawImages(Form* form, const Rectangle& clip);
	
	bool hover(int x, int y);
	bool drag(int x, int y);
	bool click(int x, int y);

	/**
	* Translate the menu wheel with x and y
	*/
	void translate(float x, float y);

	/**
	* Get the arctan angle of x and y. Correct it with the placement
	* of the menuWheel
	*/
	float getCorrectedAngle(float x, float y);

	/**
	* @see Control::getType
	*/
	const char* getType() const;
};

#endif