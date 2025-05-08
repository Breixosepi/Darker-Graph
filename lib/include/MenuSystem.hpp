#pragma once 
#include <Widget.hpp>

class MenuSystem 
{
    public:

        MenuSystem( const RendererPtr& render, const FontPtr& fonts);
        ~MenuSystem() = default;
        MenuSystem(const MenuSystem&) = delete;
        MenuSystem& operator=(const MenuSystem&) = delete;
        static std::unique_ptr<MenuSystem> createMainMenu(const RendererPtr& renderer,const FontPtr& font); 
        void addWidget(const std::string& name, const std::string& label, std::function<void()> action);
        void handleEvent(const SDL_Event& event);
        void render() const;
        void navigateUp();
        void navigateDown();
        void executeCurrent();
    
    private:
        RendererPtr renderer;
        FontPtr font;
        std::vector<std::unique_ptr<Widget>> widgets;
        Widget* head;
        Widget* tail;
        Widget* current;
    };