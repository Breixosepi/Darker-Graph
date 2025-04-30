#include <MyGraph.hpp>

int main()
{
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