#pragma once
#include <Utilities.hpp>
#include <unordered_map>
#include <iostream>

//Get<0>=WidthTile, Get<1>=HeightTile, Get<2>=ShrinkX, Get<3>=ShrinkY, Get<4>=ResizeX, Get<5>=ResizeY 
using Measures = std::tuple<double,double,double,double,double,double>; 

class RenderHelper
{
    public:
        RenderHelper();
        ~RenderHelper();

        SDL_Texture* getTexture(const std::string& path, SDL_Renderer* renderer);
        SDL_Rect* getSource(const std::string& figure);
        const Measures& getMeasuresMap();
        const Measures& getMeasuresRoom();
        const Measures& getMeasuresRoom(const int& numDivisions);
        const int& getMiddlePointInX();
        const int& getMiddlePointInY();
        const int& getWindowWitdth();
        const int& getWindowHeight();
        void handleWindowResize(const int& width, const int& height, const std::pair<int,int>& matrix);
        void setSource(const std::string& figure, const int& x, const int& y, const int& w, const int& h);

    private:
        std::unordered_map<std::string,SDL_Texture*> textures;
        std::unordered_map<std::string,SDL_Rect> sources;
        std::unordered_map<int,Measures> measuresRoom;
        std::unordered_map<int,std::pair<int,int>> middlePoints;
        Measures measuresMap;
        int windowWidth;
        int windowHeight;
        int lastDivisions;
        void calcMap(const std::pair<int,int>& matrix);
        Measures calcRoom(const double& numDivisions);
};

using HelperPtr = std::shared_ptr<RenderHelper>;