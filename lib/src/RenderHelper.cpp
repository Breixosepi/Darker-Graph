#include <RenderHelper.hpp>

RenderHelper::RenderHelper()
{
    windowWidth = windowHeight = -1;
    lastDivisions = -1;
}

RenderHelper::~RenderHelper(){}

const int& RenderHelper::getWindowWitdth(){return windowWidth;}

const int& RenderHelper::getWindowHeight(){return windowHeight;}

//Get<0>=WidthTile, Get<1>=HeightTile, Get<2>=ShrinkX, Get<3>=ShrinkY, Get<4>=ResizeX, Get<5>=ResizeY
const Measures& RenderHelper::getMeasuresMap(){return measuresMap;};

//Get<0>=WidthTile, Get<1>=HeightTile, Get<2>=ShrinkX, Get<3>=ShrinkY, Get<4>=ResizeX, Get<5>=ResizeY
//Calcula las medidas segun el numero de divisiones/numero de baldosas ingresado
const Measures& RenderHelper::getMeasuresRoom(const int& numDivisions)
{
    lastDivisions = numDivisions;
    if(!measuresRoom.count(numDivisions))
    {
        measuresRoom.insert({numDivisions,calcRoom(numDivisions)});
    }
    return measuresRoom.at(numDivisions);
}

//Get<0>=WidthTile, Get<1>=HeightTile, Get<2>=ShrinkX, Get<3>=ShrinkY, Get<4>=ResizeX, Get<5>=ResizeY
//Utiliza las Medidas Calculadas con el ultimo numero de divisiones/numero de Baldosas ingresado
const Measures& RenderHelper::getMeasuresRoom()
{
    if(lastDivisions==-1){std::cout<<"The measurements of the room have not been calculated previously."<<std::endl;}
    return measuresRoom.at(lastDivisions);
}

SDL_Texture* RenderHelper::getTexture(const std::string& path, SDL_Renderer* renderer)
{
    if(!textures.count(path))
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
                    textures.insert({path,text});
                }
            }
            else
            {
                std::cout<<"Surface "<<path<<" dont created."<<std::endl;
                return nullptr;
            }
        }
    }
    return textures.at(path);
}

SDL_Rect* RenderHelper::getSource(const std::string& figure)
{
    if(sources.count(figure)){return &sources.at(figure);}
    std::cout<<"Source "<<figure<<" not present."<<std::endl;
    return nullptr;
}

void RenderHelper::setSource(const std::string& figure, const int& x, const int& y, const int& w, const int& h)
{
    sources.insert({figure,{x,y,w,h}});
}

void RenderHelper::handleWindowResize(const int& width, const int& height, const std::pair<int,int>& matrix)
{
    windowWidth = width;
    windowHeight = height;
    calcMap(matrix);
    if(!measuresRoom.empty()){measuresRoom.clear();}
}

void RenderHelper::calcMap(const std::pair<int,int>& matrix)
{
    double square = std::min(windowWidth/(matrix.second+1.0),windowHeight/(matrix.first+1.0)); //se escoge la celda mas pequena
    double shrink = square/4.0; //se encoge la celda para dejar espacio para los pasillos
    double tile = square-shrink;
    double resizeX = (windowWidth-square*matrix.second)/2.0;
    double resizeY = (windowHeight-square*matrix.first)/2.0;
    measuresMap = std::make_tuple(tile,tile,shrink,shrink,resizeX,resizeY);
}

Measures RenderHelper::calcRoom(const double& numDivisions)
{
    int extraDim = 2;
    std::cout<<numDivisions<<std::endl;
    double cellWidth = windowWidth/(numDivisions+extraDim);
    double cellHeight = windowHeight/(numDivisions+extraDim);
    //if(centered){cellHeight = cellWidth = std::min(cellHeight,cellWidth);}
    double shrinkX = cellWidth/4.0; //margen en horizontal Hitbox! en X
    double shrinkY = cellHeight/2.0; //margen en vertical Hitbox! en Y
    double widthTile = (cellWidth*(numDivisions+extraDim) - shrinkX*2.0)/numDivisions;
    double heightTile = (cellHeight*(numDivisions+extraDim) - shrinkY*2.0)/numDivisions;
    double resizeY = (windowHeight-cellHeight*(numDivisions+extraDim))/2.0;
    double resizeX = (windowWidth-cellWidth*(numDivisions+extraDim))/2.0;
    return std::make_tuple(widthTile,heightTile,shrinkX,shrinkY,resizeX,resizeY);
}