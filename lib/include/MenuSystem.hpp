#pragma once 
#include <Widget.hpp>
#include <Level.hpp>
#include <MyGraph.hpp>
#include <Player.hpp>
#include <Enemy.hpp>

class MenuSystem 
{
    public:
        MenuSystem(const RendererPtr& render, const FontPtr& fonts, const WindowPtr& wind);
        ~MenuSystem() = default;
        MenuSystem(const MenuSystem&) = delete;
        MenuSystem& operator=(const MenuSystem&) = delete;
        static std::unique_ptr<MenuSystem> createMainMenu(const RendererPtr& renderer,const FontPtr& font, const WindowPtr& wind); 
        void addWidget(const std::string& name, const std::string& label, std::function<void()> action);
        void handleEvent(const SDL_Event& event);
        void render() const;
        void navigateUp();
        void setupLevel(Level& level, Player& player, Enemy& enemy);
        void navigateDown();
        void executeCurrent();
        void playGameLoop(Level& level, Player& player, Enemy& enemy);
    
    private:
        WindowPtr window;
        RendererPtr renderer;
        FontPtr font;
        std::vector<std::unique_ptr<Widget>> widgets;
        Widget* head;
        Widget* tail;
        Widget* current;
};