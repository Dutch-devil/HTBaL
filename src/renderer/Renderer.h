#ifndef Renderer_H_
#define Renderer_H_

class Renderer;

enum Renderers {
    KEEP,
    MAIN_MENU,
    HOUSE,
    MAP
};

#include "../MaterialManager.h"
#include "menuelements/MenuWheel.h"
#include "GenericInputListener.h"
#include "gameplay.h"

using namespace gameplay;

class Renderer : public GenericInputListener {
public:
    Renderer(Rectangle viewport);
    virtual ~Renderer();
    
    virtual void update(float elapsedTime) = 0;
    virtual Renderers getNextRenderer();
    virtual void render(float elapsedTime) = 0;
    
    virtual void resizeEvent(unsigned int width, unsigned int height);
    
    static void finalize();
    
    static void drawText(const Vector4& color, unsigned int x, unsigned int y, const char* text);
    static void drawText(const Vector4& color, Rectangle rect, const char* text, Font::Justify justify);
protected:
    Rectangle viewport;
    
private:
    static Font* font;
    static Font* getFont();
};


#endif