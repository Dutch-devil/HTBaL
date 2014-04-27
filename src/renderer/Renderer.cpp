#include "Renderer.h"

Font* Renderer::font = NULL;

Renderer::Renderer(Rectangle viewport) : GenericInputListener() {
    this->viewport = viewport;
}
Renderer::~Renderer() {
}

void Renderer::finalize() {
    SAFE_RELEASE(font);
}

Renderers Renderer::getNextRenderer() {
    return KEEP;
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