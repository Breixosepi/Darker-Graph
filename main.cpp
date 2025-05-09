#include <Game.hpp>

Game *game = nullptr;

int main(int argc , const char * argv[])
{
    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->initialize("Darker Graph", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, false);
    
    while(game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }
    game->cleanup();
    
    return 0;
    /*if(SDL_Init(SDL_INIT_VIDEO)<0){std::cout<<"Error Init SDL"<<std::endl;};
    int imgFlags = IMG_INIT_PNG;
    if((IMG_Init(imgFlags)&imgFlags)!=imgFlags){std::cout<<"Error Init IMG"<<std::endl;};*/

    // MyGraph creator;
    // std::vector<Level> levels;
    // bool exit = false;
    // while(!exit)
    // {
    //     std::pair<Designar::Graph<Room>,std::vector<std::vector<int>>> pair;
    //     pair = creator.createMap(false);
    //     Level newLevel;
    //     newLevel.setMap(pair.first);
    //     newLevel.setMatrix(pair.second);
    //     levels.push_back(newLevel);
    //     std::cout<<"Grafo creado!"<<std::endl;
    //     std::cout<<"Cantidad de mapas Creados: "<<levels.size()<<std::endl;
    //     std::cout<<"Desea salir? Si(1), No(0) ";
    //     std::cin>>exit;
    // }
    // return 0;
}