#pragma once

#include <Room.hpp>

class Level
{
    public:
        Level();
        ~Level();

        Designar::Graph<Room> getMap();
        Designar::Graph<Room> getShortestPath();
        Designar::Graph<Room> getEulerianPath();
        std::vector<std::vector<int>> getMatrix();
        std::vector<std::vector<int>> getReducedMatrix();

        void setMap(Designar::Graph<Room> value);
        void setShortestPath(Designar::Graph<Room> value);
        void setEulerianPath(Designar::Graph<Room> value);
        void setMatrix(std::vector<std::vector<int>> value);
    
    private:
        Designar::Graph<Room> map;
        Designar::Graph<Room> shortestPath;
        Designar::Graph<Room> eulerianPath;
        std::vector<std::vector<int>> matrix;
        std::vector<std::vector<int>> reducedMatrix;
};