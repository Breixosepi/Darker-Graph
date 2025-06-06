#pragma once
#include <Utilities.hpp>
#include <string>
#include <functional>
#include <stdexcept>

class Widget 
{
public:

    Widget(const std::string& name, int x, int y, const std::string& label);
    virtual ~Widget() = default;
    virtual void render(SDL_Renderer* renderer, TTF_Font* font, bool isActive) const;
    virtual void execute() const;
    void setAction(std::function<void()> action);
    void setPosition(int x, int y);
    void setLabel(const std::string& label);
    void setRect(const SDL_Rect& rect);
    bool containsPoint(int x, int y) const;
    Widget* getNext(); 
    Widget* getPrev();
    void setNext(Widget* n); 
    void setPrev(Widget* p); 

private:
    std::string name;
    int x, y;
    SDL_Rect boundingBox;
    std::string label;
    std::function<void()> action;
    Widget* next;
    Widget* prev; 

};