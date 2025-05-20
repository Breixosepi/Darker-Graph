#pragma once

#include <Room.hpp>
#include <unordered_map>
#include <AnimatedFigure.hpp>

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
        Room* getCurrentRoom();
        std::pair<int,int> getPosNeighbor(const int& direction);
        const int* getIndexNeighbor(const int& direction);

        void setMap(const Designar::Graph<Room>& value);
        void setShortestPath(const Designar::Graph<Room>& value);
        void setEulerianPath(const Designar::Graph<Room>& value);
        void setMatrix(const std::vector<std::vector<int>>& value);
        SDL_Texture* setTexture(const char* value, SDL_Renderer* renderer);
        void insertSourceShape(const int& x, const int& y, const int& w, const int& h, const int& numOfShape);
        void setSourceBackground(const int& x, const int& y, const int& w, const int& h);
        std::pair<double,double> setWindowSize(const int& width, const int& height);
        void setCurrentIndex(const int& value);

        void printMapConsole();
        void initializeResources(SDL_Renderer* renderer);
        std::pair<int,int> verifyPassRoom(int direction, const SDL_Rect& rectPlayer);
        std::pair<int,int> passRoom(int direction, const SDL_Rect& rectPlayer);
        
        void renderMap(SDL_Renderer* renderer);
        void renderRoom(SDL_Renderer* renderer);
        void renderRoomLastFrame(SDL_Renderer* renderer);
    
    private:
        Designar::Graph<Room> map;
        Designar::Graph<Room> shortestPath;
        Designar::Graph<Room> eulerianPath;
        std::vector<std::vector<int>> matrix;
        std::vector<std::vector<int>> matrixReduced;
        int windowWidth;
        int windowHeight;
        double rows;
        double columns;
        std::vector<Designar::Graph<Room>::Node*> roomsReference;
        int currentIndex;
        SDL_Rect sourceBackground;
        //0:Tile, 1:Portal, 2:Path, 3:WallHorizontal, 4:WallVertical, 5:Column, 6:Door, 7:Darkness
        std::unordered_map<int,SDL_Rect> sourceShapes;
        std::vector<PosShape> shapesMap;
        std::vector<PosShape> shapesRoom;
        std::vector<PosShape> doors;
        std::vector<PosShape> lowerFrameRoom;
        std::unordered_map<int,std::pair<int,int>> dimensionsMap;
        std::unordered_map<int,std::pair<int,int>> dimensionsRoom;
        SDL_Texture* textTileSet;
        SDL_Texture* textBackground;
        AnimatedFigure animated;

        void getRowsColumns();
        void reduceMatrix();
        void setShapesMap();
        std::pair<double,double> setDesignRoom(const bool& centered);
        void draw(const std::vector<PosShape>& shapes, const std::unordered_map<int,std::pair<int,int>>& dimensions, SDL_Renderer* renderer);
        void drawDoors(SDL_Renderer* renderer);
        SDL_Rect fillRect(const PosShape& shape, const std::unordered_map<int,std::pair<int,int>>& dimensions);
};