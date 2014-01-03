#include "Renderer.h"

Font* Renderer::font = NULL;

Renderer::Renderer(Rectangle viewport) {
    this->viewport = viewport;
    buttonDown = 0;
    keyFlags = NULL;
    mouseFlags = NULL;
    clickStarts = NULL;
}
Renderer::~Renderer() {
    SAFE_DELETE(mouseFlags);
    SAFE_DELETE(keyFlags);
    SAFE_DELETE_ARRAY(clickStarts);
}

void Renderer::finalize() {
    SAFE_RELEASE(font);
}

Renderers Renderer::getNextRenderer() {
    return KEEP;
}

bool Renderer::leftButtonDown() {
    return getMouseFlags()->getFlag(Mouse::MouseEvent::MOUSE_PRESS_LEFT_BUTTON / 2);
}

bool Renderer::rightButtonDown() {
    return getMouseFlags()->getFlag(Mouse::MouseEvent::MOUSE_PRESS_RIGHT_BUTTON / 2);
}

bool Renderer::middleButtonDown() {
    return getMouseFlags()->getFlag(Mouse::MouseEvent::MOUSE_PRESS_MIDDLE_BUTTON / 2);
}

bool Renderer::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData) {
    int btn = evt / 2;
    bool clicked = false;
    if (btn < 3) {
        bool press = (evt + 1) % 2;
        getMouseFlags()->forceFlag(btn, press);
        Vector2* starts = getClickStarts();
        if (press) {
            starts[btn].x = x;
            starts[btn].y = y;
        } else {
            clicked = starts[btn].x == x && starts[btn].y == y;
        }
    } else if (clickStarts != NULL) {
        memset(clickStarts, 0, sizeof(Vector2) * 3);
    }
    return mouseEvent(evt, x, y, wheelData, evt == Mouse::MOUSE_MOVE && (leftButtonDown() || rightButtonDown() || middleButtonDown()), clicked);
}

bool Renderer::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelData, bool dragging, bool clicked) {
    return false;
}

void Renderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex) {
    bool clicked = false;
    Vector2* starts = getClickStarts();
    if (evt == Touch::TouchEvent::TOUCH_PRESS) {
        starts[0].x = x;
        starts[0].y = y;
    } else if (evt == Touch::TouchEvent::TOUCH_RELEASE) {
        clicked = starts[0].x == x && starts[0].y == y;
    } else if (clickStarts != NULL) {
        memset(clickStarts, 0, sizeof(Vector2) * 3);
    }
    touchEvent(evt, x, y, contactIndex, clicked);
}

void Renderer::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool clicked) {}

void Renderer::keyEvent(Keyboard::KeyEvent evt, int key) {
    if (evt == Keyboard::KeyEvent::KEY_PRESS) {
        getKeyFlags()->setFlag(key);
    } else if (evt == Keyboard::KeyEvent::KEY_RELEASE) {
        getKeyFlags()->resetFlag(key);
    }
    keyEvent(evt, key, getKeyFlags());
}
void Renderer::keyEvent(Keyboard::KeyEvent evt, int key, KeyFlags* flags) {}


Flags* Renderer::getMouseFlags() {
    if (mouseFlags == NULL) {
        mouseFlags = new Flags(4);
    }
    return mouseFlags;
}

KeyFlags* Renderer::getKeyFlags() {
    if (keyFlags == NULL) {
        keyFlags = new KeyFlags();
    }
    return keyFlags;
}

Vector2* Renderer::getClickStarts() {
    if (clickStarts == NULL) {
        clickStarts = new Vector2[3];
        memset(clickStarts, 0, sizeof(Vector2) * 3);
    }
    return clickStarts;
}

void Renderer::resizeEvent(unsigned int width, unsigned int height) {
    this->viewport.width = width;
    this->viewport.height = height;
}


Font* Renderer::getFont() {
    if (font == NULL) {
        font = Font::create("res/arial14.gpb");
    }
    return font;
}

void Renderer::drawText(const Vector4& color, unsigned int x, unsigned int y, const char* text) {
    drawText(color, Rectangle(x, y, 1000, 1000), text, Font::Justify::ALIGN_LEFT);
}

void Renderer::drawText(const Vector4& color, Rectangle rect, const char* text, Font::Justify justify) {
    Font* font = getFont();
    font->start();                                          // start using the font
    font->drawText(text, rect, color, font->getSize(), justify);    // Writing the fps to the screen
    font->finish();                                         // stop using the font
}