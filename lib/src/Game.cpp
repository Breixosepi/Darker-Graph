#include <Game.hpp>

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr)
{

}
Game::~Game()
{}

void Game::initialize(const char* title,int x_pos,int y_pos, int width, int height, bool fullscreen)
{
    Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        WindowPtr winPtr(SDL_CreateWindow(title, x_pos, y_pos, width, height, flags));
        RendererPtr RenderPtr(SDL_CreateRenderer(window, -1, 0));  
        window = winPtr.release();
        renderer = RenderPtr.release();
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
}
void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}
void Game::update()
{
}
void Game::render()
{
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
void Game::cleanup()
{
    if (renderer) 
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) 
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
    isRunning = false;
}

bool Game::running()
{
    return isRunning;

}