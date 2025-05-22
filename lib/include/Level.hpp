#pragma once

#include <Room.hpp>
#include <unordered_map>
#include <AnimatedFigure.hpp>
#include <RenderHelper.hpp>

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
        const std::pair<int,int> getMatrixSize();

        void setMap(const Designar::Graph<Room>& value);
        void setShortestPath(const Designar::Graph<Room>& value);
        void setEulerianPath(const Designar::Graph<Room>& value);
        void setMatrix(const std::vector<std::vector<int>>& value);
        void setRenderHelper(HelperPtr value);

        void handleResizeWindow();
        std::pair<int,int> verifyPassRoom(int direction, const SDL_Rect& rectPlayer);

        void printMapConsole();
        
        void renderMap(SDL_Renderer* renderer);
        void renderRoom(SDL_Renderer* renderer);
        void renderRoomLastFrame(SDL_Renderer* renderer);
    
    private:
        Designar::Graph<Room> map;
        Designar::Graph<Room> shortestPath;
        Designar::Graph<Room> eulerianPath;
        std::vector<std::vector<int>> matrix;

        std::vector<Designar::Graph<Room>::Node*> roomsReference;
        int currentIndex;
        
        std::vector<PosShape> shapesMap;
        std::vector<PosShape> shapesRoom;
        std::vector<PosShape> doors;
        std::vector<PosShape> lowerFrameRoom;

        SDL_Rect playerInMap;
        std::unordered_map<std::string,std::pair<int,int>> dimensionsMap;
        std::unordered_map<std::string,std::pair<int,int>> dimensionsRoom;

        AnimatedFigure animated;
        HelperPtr helper;

        void calcPosInMap();
        void setSources();
        void setShapesMap();
        void setDesignRoom();
        void setCurrentIndex(const int& value);
        void draw(const std::vector<PosShape>& shapes, const std::unordered_map<std::string,std::pair<int,int>>& dimensions, SDL_Renderer* renderer);
        void drawDoors(SDL_Renderer* renderer);
        const std::pair<int,int> getPosNeighbor(const int& direction);
        const int getIndexNeighbor(const int& direction);
        std::pair<int,int> passRoom(int direction, const SDL_Rect& rectPlayer);
        SDL_Rect fillRect(const PosShape& shape, const std::unordered_map<std::string,std::pair<int,int>>& dimensions);
};