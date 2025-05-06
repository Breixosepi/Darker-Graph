#pragma once
#include <SDL2/SDL.h>

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
    SDL_Window* window;
    SDL_Renderer* renderer;
};

