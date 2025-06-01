#include <RenderHelper.hpp>

RenderHelper::RenderHelper()
{
    windowWidth = windowHeight = -1;
    lastDivisions = -1;
}

RenderHelper::~RenderHelper(){}

const int& RenderHelper::getWindowWitdth(){return windowWidth;}

const int& RenderHelper::getWindowHeight(){return windowHeight;}

const int& RenderHelper::getDivisions(){return lastDivisions;}

//Get<0>=WidthTile, Get<1>=HeightTile, Get<2>=ShrinkX, Get<3>=ShrinkY, Get<4>=ResizeX, Get<5>=ResizeY
const Measures& RenderHelper::getMeasuresMap(){return measuresMap;}

//Get<0>=WidthTile, Get<1>=HeightTile, Get<2>=ShrinkX, Get<3>=ShrinkY, Get<4>=ResizeX, Get<5>=ResizeY
//Calcula las medidas segun el numero de divisiones/numero de baldosas ingresado
const Measures& RenderHelper::getMeasuresRoom(const int& numDivisions)
{
    lastDivisions = numDivisions;
    auto it = measuresRoom.find(numDivisions);
    if(it==measuresRoom.end())
    {
        auto result = measuresRoom.emplace(numDivisions,calcRoom(numDivisions));
        return result.first->second;
    }
    return (*it).second;
}

//Get<0>=WidthTile, Get<1>=HeightTile, Get<2>=ShrinkX, Get<3>=ShrinkY, Get<4>=ResizeX, Get<5>=ResizeY
//Utiliza las Medidas Calculadas con el ultimo numero de divisiones/numero de Baldosas ingresado
const Measures& RenderHelper::getMeasuresRoom()
{
    auto it = measuresRoom.find(lastDivisions);
    if(it==measuresRoom.end())
    {
        std::cout<<"The measurements of the room have not been calculated previously."<<std::endl;
        return Measures{};
    }
    return (*it).second;
}

const double RenderHelper::widthTile(){return std::ceil(std::get<0>(getMeasuresRoom()));}

const double RenderHelper::heightTile(){return std::ceil(std::get<1>(getMeasuresRoom()));}

const double RenderHelper::shrinkX(){return std::ceil(std::get<2>(getMeasuresRoom()));}

const double RenderHelper::shrinkY(){return std::ceil(std::get<3>(getMeasuresRoom()));}

const double RenderHelper::resizeX(){return std::ceil(std::get<4>(getMeasuresRoom()));}

const double RenderHelper::resizeY(){return std::ceil(std::get<5>(getMeasuresRoom()));}

const Measures RenderHelper::truncMeasuresRoom()
{
    return std::make_tuple(widthTile(),heightTile(),shrinkX(),shrinkY(),resizeX(),resizeY());
}

const int& RenderHelper::getMiddlePointInX()
{
    auto it = middlePoints.find(lastDivisions);
    if(it==middlePoints.end())
    {
        std::cout<<"The measurements of the room have not been calculated previously."<<std::endl;
        return int{};
    }
    return (*it).second.first;
}

const int& RenderHelper::getMiddlePointInY()
{
    auto it = middlePoints.find(lastDivisions);
    if(it==middlePoints.end())
    {
        std::cout<<"The measurements of the room have not been calculated previously."<<std::endl;
        return int{};
    }
    return (*it).second.second;
}

SDL_Texture* RenderHelper::getTexture(const std::string& path, SDL_Renderer* renderer)
{
    auto it = textures.find(path);
    if(it==textures.end())
    {
        if(!renderer)
        {
            std::cout<<"Renderer Null."<<std::endl;
            return nullptr;
        }
        else
        {
            SDL_Surface* surf = IMG_Load(path.c_str());
            if(surf)
            {
                SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
                SDL_FreeSurface(surf);
                if(!text)
                {
                    std::cout<<"Texture "<<path<<" dont created."<<std::endl;
                    return nullptr;
                }
                else
                {
                    auto result = textures.emplace(path,text);
                    return result.first->second;
                }
            }
            else
            {
                std::cout<<"Surface "<<path<<" dont created."<<std::endl;
                return nullptr;
            }
        }
    }
    return (*it).second;
}

SDL_Rect* RenderHelper::getSource(const std::string& figure)
{
    auto it = sources.find(figure);
    if(it!=sources.end())
    {
        return &(*it).second;
    }
    std::cout<<"Source "<<figure<<" not present."<<std::endl;
    return nullptr;
}

void RenderHelper::setSource(const std::string& figure, const int& x, const int& y, const int& w, const int& h)
{
    sources.insert({figure,{x,y,w,h}});
}

void RenderHelper::setMatrixSize(const std::pair<int,int>& value){matrixSize = value;}

void RenderHelper::handleWindowResize(const int& width, const int& height)
{
    windowWidth = width;
    windowHeight = height;
    calcMap();
    if(!measuresRoom.empty())
    {
        measuresRoom.clear();
        middlePoints.clear();
    }
}

void RenderHelper::calcMap()
{
    double square = std::min(windowWidth/(matrixSize.second+1.0),windowHeight/(matrixSize.first+1.0)); //se escoge la celda mas pequena
    double shrink = square/4.0; //se encoge la celda para dejar espacio para los pasillos
    double tile = square-shrink;
    double resizeX = (windowWidth-square*matrixSize.second)/2.0;
    double resizeY = (windowHeight-square*matrixSize.first)/2.0;
    measuresMap = std::make_tuple(tile,tile,shrink,shrink,resizeX,resizeY);
}

Measures RenderHelper::calcRoom(const double& numDivisions)
{
    int extraDim = 2;
    double cellWidth = windowWidth/(numDivisions+extraDim);
    double cellHeight = windowHeight/(numDivisions+extraDim);
    //if(centered){cellHeight = cellWidth = std::min(cellHeight,cellWidth);}
    double shrinkX = cellWidth/4.0; //margen en horizontal Hitbox! en X
    double shrinkY = cellHeight/2.0; //margen en vertical Hitbox! en Y
    double widthTile = (cellWidth*(numDivisions+extraDim) - shrinkX*2.0)/numDivisions;
    double heightTile = (cellHeight*(numDivisions+extraDim) - shrinkY*2.0)/numDivisions;
    double resizeY = (windowHeight-cellHeight*(numDivisions+extraDim))/2.0;
    double resizeX = (windowWidth-cellWidth*(numDivisions+extraDim))/2.0;
    middlePoints.insert({numDivisions,{resizeX+shrinkX+widthTile*numDivisions/2.0,resizeY+shrinkY+heightTile*numDivisions/2.0}});
    return std::make_tuple(widthTile,heightTile,shrinkX,shrinkY,resizeX,resizeY);
}