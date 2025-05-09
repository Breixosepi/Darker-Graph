#include "Widget.hpp"

Widget::Widget(const std::string& name, int x, int y, const std::string& label) : name(name), x(x), y(y), label(label), next(nullptr), prev(nullptr) 
{
    boundingBox = {0, 0, 0, 0};
}

void Widget::setRect(const SDL_Rect& rect) {
    boundingBox = rect;
}

bool Widget::containsPoint(int x, int y) const {
    return (x >= boundingBox.x && x <= boundingBox.x + boundingBox.w &&
            y >= boundingBox.y && y <= boundingBox.y + boundingBox.h);
}
void Widget::render(const RendererPtr& renderer, const FontPtr& font, bool isActive) const 
{
    SDL_Color color = isActive ? SDL_Color{0, 255, 0, 255} : SDL_Color{255, 255, 255, 255};

    SurfacePtr surface(TTF_RenderText_Blended(font.get(), label.c_str(), color));
    if (!surface) 
    {
        throw std::runtime_error("Error en la superficie");
    }
    
    TexturePtr texture(SDL_CreateTextureFromSurface(renderer.get(), surface.get()));
    if (!texture) 
    {
        throw std::runtime_error("Error en la textura");
    }

    int screenWidth, screenHeight;
    SDL_GetRendererOutputSize(renderer.get(), &screenWidth, &screenHeight);

    SDL_Rect rect
    {
        (screenWidth - surface->w) / 2,  
        (screenHeight / 100) + y,         
        surface->w,                    
        surface->h                      
    };
    const_cast<Widget*>(this)->boundingBox = rect;
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &rect);
    
    if (isActive) 
    {
        SurfacePtr arrowSurface(TTF_RenderText_Blended(font.get(), ">", color));
        if (arrowSurface) 
        {
            TexturePtr arrowTexture(SDL_CreateTextureFromSurface(renderer.get(), arrowSurface.get()));
            if (arrowTexture) 
            {
                SDL_Rect arrowRect
                {
                    rect.x - arrowSurface->w - 10,  
                    rect.y,                         
                    arrowSurface->w,              
                    arrowSurface->h                 
                };
                SDL_RenderCopy(renderer.get(), arrowTexture.get(), nullptr, &arrowRect);
            }
        }
    }
}

void Widget::execute() const 
{
    if (action) action();
}

void Widget::setAction(std::function<void()> action) 
{
    this->action = action;
}

void Widget::setPosition(int x, int y) 
{
    this->x = x;
    this->y = y;
}

void Widget::setLabel(const std::string& label) 
{
    this->label = label;
}

Widget* Widget::getNext() 
{
    return next;
}
Widget* Widget::getPrev() 
{
    return prev;
}
void Widget::setNext(Widget* n) 
{
    next = n;
}
void Widget::setPrev(Widget* p) 
{
    prev = p;
}