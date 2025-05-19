#include <Game.hpp>

Game *game = nullptr;

int main(int argc , const char * argv[])
{
    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->initialize("Darker Graph", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);
    
    while(game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }
    game->cleanup();
    return 0;
}