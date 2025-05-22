#pragma once 
#include <Widget.hpp>
#include <Level.hpp>
#include <MyGraph.hpp>
#include <EnemyManager.hpp>

class MenuSystem 
{
    public:
        MenuSystem();
        MenuSystem(SDL_Renderer* render, TTF_Font* fonts, SDL_Window* wind);
        ~MenuSystem() = default;
        MenuSystem(const MenuSystem&) = delete;
        MenuSystem& operator=(const MenuSystem&) = delete;  
        void handleEvent(const SDL_Event& event);
        void render();
        void setupLevel(Level& level, Player& player);
        void playGameLoop(Level& level, Player& player);
    
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        TTF_Font* font;
        std::vector<std::unique_ptr<Widget>> widgets;
        Widget* head;
        Widget* tail;
        Widget* current;

        HelperPtr helper;
        MyGraph creator;
        std::queue<Level> levels;
        EnemyManager enemyManager;

        int windowWidth;
        int windowHeight;

        void setMainMenu(MenuSystem* menu);
        void addWidget(const std::string& name, const std::string& label, std::function<void()> action);
        void navigateUp();
        void navigateDown();
        void executeCurrent();
};