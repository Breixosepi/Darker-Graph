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
        void showGameOverScreen(int score);
        void updateInputTexture(const std::string& text, SDL_Color color) ;
        void saveHighScore(const std::string& name, int score) ;
    
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        TexturePtr inputTexture;
        HelperPtr helper;

        TTF_Font* font;
        std::vector<std::unique_ptr<Widget>> widgets;
        Widget* head;
        Widget* tail;
        Widget* current;
        MyGraph creator;
        Level level;
        EnemyManager enemyManager;
        std::queue<Level> levels;
        std::vector<std::pair<std::string, int>> highScores;


        int windowWidth;
        int windowHeight;
        
        void showHighScores();
        void setMainMenu(MenuSystem* menu);
        void addWidget(const std::string& name, const std::string& label, std::function<void()> action);
        void navigateUp();
        void navigateDown();
        void executeCurrent();
};