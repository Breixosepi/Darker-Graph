#pragma once

#include <Room.hpp>
#include <unordered_map>
using PosShape = std::tuple<int,int,int>; //Get<0>=X, Get<1>=Y, Get<2>=typeShape 

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
        const std::vector<Designar::Graph<Room>::Node*>* getRoomsReference();

        void setMap(const Designar::Graph<Room>& value);
        void setShortestPath(const Designar::Graph<Room>& value);
        void setEulerianPath(const Designar::Graph<Room>& value);
        void setMatrix(const std::vector<std::vector<int>>& value);
        void setShapesMap(const int& width, const int& height);
        void setTileSet(const char* value, SDL_Renderer* renderer);
        void insertOriginShape(const int& x, const int& y, const int& w, const int& h, const int& numOfShape);
        void setBackground(const char* value, SDL_Renderer* renderer);
        void setOriginBackground(const int& x, const int& y, const int& w, const int& h);

        void printMapConsole();
        void drawMap(SDL_Renderer* renderer);
        void drawRoom(const int& index, const int& width, const int& height, const double& div, SDL_Renderer* renderer, const bool& centered);
    
    private:
        Designar::Graph<Room> map;
        Designar::Graph<Room> shortestPath;
        Designar::Graph<Room> eulerianPath;
        std::vector<std::vector<int>> matrix;
        double rows;
        double columns;
        std::vector<Designar::Graph<Room>::Node*> roomsReference;
        SDL_Rect originBackground;
        //0:Tile, 1:Portal, 2:Path, 3:Wall, 4:Column
        std::unordered_map<int,SDL_Rect> originShapes;
        std::vector<PosShape> posShapes;
        std::unordered_map<int,int> dimensionShapes;
        SDL_Surface* surfTileSet;
        SDL_Texture* textTileSet;
        SDL_Surface* surfBackground;
        SDL_Texture* textBackground;

        void getRowsColumns();
        void reduceMatrix();
};