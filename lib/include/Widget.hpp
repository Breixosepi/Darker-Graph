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
    virtual void render(const RendererPtr& renderer, const FontPtr& font, bool isActive) const;
    virtual void execute() const;
    void setAction(std::function<void()> action);
    void setPosition(int x, int y);
    void setLabel(const std::string& label);
    Widget* getNext(); 
    Widget* getPrev();
    void setNext(Widget* n); 
    void setPrev(Widget* p); 

private:
    std::string name;
    int x, y;
    std::string label;
    std::function<void()> action;
    Widget* next;
    Widget* prev; 

};