#include <MyGraph.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int main()
{
    /*if(SDL_Init(SDL_INIT_VIDEO)<0){std::cout<<"Error Init SDL"<<std::endl;};
    int imgFlags = IMG_INIT_PNG;
    if((IMG_Init(imgFlags)&imgFlags)!=imgFlags){std::cout<<"Error Init IMG"<<std::endl;};*/
    MyGraph creator;
    std::vector<Designar::Graph<Room>> levels;
    bool exit = false;
    while(!exit)
    {
        levels.push_back(creator.createMap(false));
        std::cout<<"Grafo creado!"<<std::endl;
        std::cout<<"Cantidad de mapas Creados: "<<levels.size()<<std::endl;
        std::cout<<"Desea salir? Si(1), No(0) ";
        std::cin>>exit;
    }
    return 0;
}