#pragma once

#include <Room.hpp>
using shape = std::tuple<int,int,int,int,int>; //Get<0>=X, Get<1>=Y, Get<2>=Width, Get<3>=Height, Get<4>=typeShape 

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
        const std::vector<shape>* getShapesMap();

        void setMap(const Designar::Graph<Room>& value);
        void setShortestPath(const Designar::Graph<Room>& value);
        void setEulerianPath(const Designar::Graph<Room>& value);
        void setMatrix(const std::vector<std::vector<int>>& value);
        void setShapesMap(const int& width, const int& height);

        void printMapConsole();
        void getRowsColumns();
        void reduceMatrix();
    
    private:
        Designar::Graph<Room> map;
        Designar::Graph<Room> shortestPath;
        Designar::Graph<Room> eulerianPath;
        std::vector<std::vector<int>> matrix;
        std::pair<int,int> rowsColumns;
        std::vector<Designar::Graph<Room>::Node*> roomsReference;
        std::vector<shape> shapes;
};