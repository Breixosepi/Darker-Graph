#pragma once
#include <stdexcept>
#include <Utilities.hpp>
#include <MenuSystem.hpp>


class Game
{
public:
    Game();
    ~Game();
    void initialize(const char* title,int x_pos,int y_pos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void cleanup();
    bool running();

private:
    bool isRunning;
    WindowPtr window;
    RendererPtr renderer;
    FontPtr font;
    std::unique_ptr<MenuSystem> mainMenu;

};

