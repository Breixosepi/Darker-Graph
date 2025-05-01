#include <Level.hpp>

Level::Level(){}

Level::~Level(){}

Designar::Graph<Room> Level::getMap(){return map;}
        
Designar::Graph<Room> Level::getShortestPath(){return shortestPath;}
        
Designar::Graph<Room> Level::getEulerianPath(){return eulerianPath;}
        
std::vector<std::vector<int>> Level::getMatrix(){return matrix;}
        
std::vector<std::vector<int>> Level::getReducedMatrix(){return reducedMatrix;}

void Level::setMap(Designar::Graph<Room> value){map = value;}

void Level::setShortestPath(Designar::Graph<Room> value){shortestPath = value;}
        
void Level::setEulerianPath(Designar::Graph<Room> value){eulerianPath = value;}

void Level::setMatrix(std::vector<std::vector<int>> value)
{
    matrix = value;
}