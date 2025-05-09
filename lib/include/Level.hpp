#pragma once

#include <Room.hpp>

class Level
{
    public:
        Level();
        Level(std::tuple<Designar::Graph<Room>,std::vector<Designar::Graph<Room>::Node*>,std::vector<std::vector<int>>>);
        ~Level();

        const Designar::Graph<Room>* getMap();
        const Designar::Graph<Room>* getShortestPath();
        const Designar::Graph<Room>* getEulerianPath();
        const std::vector<std::vector<int>>* getMatrix();
        const std::pair<int,int>* getReducedMatrix();
        const std::vector<Designar::Graph<Room>::Node*>* getRoomsReference();

        void setMap(const Designar::Graph<Room>& value);
        void setShortestPath(const Designar::Graph<Room>& value);
        void setEulerianPath(const Designar::Graph<Room>& value);
        void setMatrix(const std::vector<std::vector<int>>& value);

        void printMapConsole();
        void getRowsColumns();
        void reduceMatrix();
        std::vector<std::pair<std::pair<int,int>,int>> shapesToDrawMap(const int& width, const int& height, int& square);
    
    private:
        Designar::Graph<Room> map;
        Designar::Graph<Room> shortestPath;
        Designar::Graph<Room> eulerianPath;
        std::vector<std::vector<int>> matrix;
        std::pair<int,int> rowsColumns;
        std::vector<Designar::Graph<Room>::Node*> roomsReference;
};