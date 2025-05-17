#pragma once

#include <Room.hpp>
#include <unordered_map>

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

        void setMap(const Designar::Graph<Room>& value);
        void setShortestPath(const Designar::Graph<Room>& value);
        void setEulerianPath(const Designar::Graph<Room>& value);
        void setMatrix(const std::vector<std::vector<int>>& value);
        void setTileSet(const char* value, SDL_Renderer* renderer);
        void insertSourceShape(const int& x, const int& y, const int& w, const int& h, const int& numOfShape);
        void setBackground(const char* value, SDL_Renderer* renderer);
        void setSourceBackground(const int& x, const int& y, const int& w, const int& h);
        double setWindowSize(const int& width, const int& height);
        void setCurrentIndex(const int& value);

        void printMapConsole();
        
        void drawMap(SDL_Renderer* renderer);
        void drawRoom(SDL_Renderer* renderer);
        void drawRoomLastFrame(SDL_Renderer* renderer);
    
    private:
        Designar::Graph<Room> map;
        Designar::Graph<Room> shortestPath;
        Designar::Graph<Room> eulerianPath;
        std::vector<std::vector<int>> matrix;
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
        SDL_Surface* surfTileSet;
        SDL_Texture* textTileSet;
        SDL_Surface* surfBackground;
        SDL_Texture* textBackground;

        void getRowsColumns();
        void reduceMatrix();
        void setShapesMap();
        double setDesignRoom(const bool& centered);
        void draw(const std::vector<PosShape>& shapes, const std::unordered_map<int,std::pair<int,int>>& dimensions, SDL_Renderer* renderer);
        void drawDoors(SDL_Renderer* renderer);
        SDL_Rect fillRect(const PosShape& shape, const std::unordered_map<int,std::pair<int,int>>& dimensions);
};