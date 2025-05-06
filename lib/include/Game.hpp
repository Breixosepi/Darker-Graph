#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <stdexcept>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

struct SDL_Deleter 
{
    void operator()(SDL_Window* window) const 
    { 
        if (window) SDL_DestroyWindow(window); 
    }
    void operator()(SDL_Renderer* renderer) const 
    {
        if (renderer) SDL_DestroyRenderer(renderer); 
    }
};

using WindowPtr = std::unique_ptr<SDL_Window, SDL_Deleter>;
using RendererPtr = std::unique_ptr<SDL_Renderer, SDL_Deleter>;

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

