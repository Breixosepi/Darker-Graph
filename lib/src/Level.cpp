#include <Level.hpp>

Level::Level(){}

Level::Level(std::tuple<Designar::Graph<Room>,std::vector<Designar::Graph<Room>::Node*>,std::vector<std::vector<int>>> tuple)
{
    map = std::get<0>(tuple);
    roomsReference = std::get<1>(tuple);
    rows = 0;
    columns = 0;
    setMatrix(std::get<2>(tuple));
}

Level::~Level(){}

const Designar::Graph<Room>* Level::getMap(){return &map;}
        
const Designar::Graph<Room>* Level::getShortestPath(){return &shortestPath;}
        
const Designar::Graph<Room>* Level::getEulerianPath(){return &eulerianPath;}
        
const std::vector<std::vector<int>>* Level::getMatrix(){return &matrix;}

const std::vector<Designar::Graph<Room>::Node*>* Level::getRoomsReference(){return &roomsReference;}

void Level::setMap(const Designar::Graph<Room>& value){map = value;}

void Level::setShortestPath(const Designar::Graph<Room>& value){shortestPath = value;}
        
void Level::setEulerianPath(const Designar::Graph<Room>& value){eulerianPath = value;}

void Level::setMatrix(const std::vector<std::vector<int>>& value)
{
    matrix = value;
    getRowsColumns();
    reduceMatrix();
}

//Actualiza el vector de figuras a dibujar para mostrar el mapa del presente nivel.
//Recibe el ancho y alto de la ventana ya que es redimensionable.
void Level::setShapesMap(const int& width, const int& height)
{
    if(!shapesMap.empty())
    {
        shapesMap.clear();
        dimensionsMap.clear();
    }
    if(!matrix.empty())
    {
        double square = std::min(width/(columns+1),height/(rows+1)); //se escoge la celda mas pequena
        double shrink = square/4; //se encoge la celda para dejar espacio para los pasillos
        double tile = square-shrink;
        double X = (width-square*columns)/2.0 - shrink/2;
        dimensionsMap.insert({0,std::make_pair(tile,tile)});
        dimensionsMap.insert({1,std::make_pair(tile,tile)});
        dimensionsMap.insert({2,std::make_pair(shrink,shrink)});
        for(int i=0; i<matrix[0].size(); ++i)
        {
            double Y = (height-square*rows)/2.0 - shrink/2;
            int memory = shapesMap.size();
            for(int j=0; j<matrix.size(); ++j)
            {
                if(matrix[j][i]!=-1)
                {
                    shapesMap.push_back(std::make_tuple(X+shrink,Y+shrink,0));
                    if(matrix[j][i]>-1)
                    {
                        std::vector<bool> paths = *roomsReference[matrix[j][i]-1]->get_info().getPaths();
                        for(int k=0; k<paths.size(); ++k)
                        {
                            if(paths[k])
                            {
                                switch (k)
                                {
                                    case 0: //Izquierda
                                        shapesMap.push_back(std::make_tuple(X,Y+square/2,2));
                                    break;
                                    case 1: //Arriba
                                        shapesMap.push_back(std::make_tuple(X+square/2,Y,2));
                                    break;
                                    case 2: //Derecha
                                        shapesMap.push_back(std::make_tuple(X+square,Y+square/2,2));
                                    break;
                                    case 3: //Abajo
                                        shapesMap.push_back(std::make_tuple(X+square/2,Y+square,2));
                                    break;
                                }
                            }
                        }
                    }
                    else{shapesMap.push_back(std::make_tuple(X+shrink,Y+shrink,1));}
                }
                Y += square;
            }
            if(memory!=shapesMap.size()){X += square;}
        }
    }
}

void Level::setShapesRoom(const int& index, const int& width, const int& height, const double& div, const bool& centered)
{
    if(!shapesRoom.empty())
    {
        shapesRoom.clear();
        dimensionsRoom.clear();
        lowerFrameRoom.clear();
    }
    double extraDim = 2.0;
    double cellWidth = width/(div+extraDim);
    double cellHeight = height/(div+extraDim);
    if(centered){cellHeight = cellWidth = std::min(cellHeight,cellWidth);}
    double shrinkX = cellWidth/4.0; //Ancho pared Vertical //Hitbox!
    double shrinkY = cellHeight/2.0; //Alto pared hotizontal
    double widthTile = (cellWidth*(div+extraDim) - shrinkX*2)/div;
    double heightTile = (cellHeight*(div+extraDim) - shrinkY*2)/div;

    dimensionsRoom.insert({0,std::make_pair(widthTile,heightTile)}); //piso
    dimensionsRoom.insert({3,std::make_pair(widthTile,heightTile)}); //pared horizontal
    dimensionsRoom.insert({4,std::make_pair(shrinkX,heightTile)}); //pared vertical
    dimensionsRoom.insert({5,std::make_pair(shrinkX*2,heightTile)}); //column

    double resizeY = (height-cellHeight*(div+extraDim))/2;
    double resizeX = (width-cellWidth*(div+extraDim))/2;
    double Y = shrinkY + shrinkY/2 + resizeY;
    for(int i=0; i<div; ++i)
    {
        double X = shrinkX + resizeX;
        for(int j=0; j<div; ++j)
        {
            shapesRoom.push_back({X,Y,0});
            if(i==0 || i==div-1)
            {
                if(i<div-1){shapesRoom.push_back({X,Y-heightTile,3});}
                else{lowerFrameRoom.push_back({X,Y+shrinkY,3});}
                if(i==0&&j==div-1){shapesRoom.push_back({widthTile*div+shrinkX/2.0+resizeX,Y-heightTile,5});}
            }
            if(j==0 || j==div-1)
            {
                if(i==0&&j==0){shapesRoom.push_back({-shrinkX/2.0+resizeX,Y-heightTile,5});}
                if(j<div-1){shapesRoom.push_back({X-shrinkX,Y-shrinkY,4});} //Paredes en Vertical (delgadas)
                else{shapesRoom.push_back({X+widthTile,Y-shrinkY,4});}
            }
            X+=widthTile;
        }
        Y+=heightTile;
    }
    lowerFrameRoom.push_back({widthTile*div+shrinkX/2.0+resizeX,heightTile*div+resizeY,5});
    lowerFrameRoom.push_back({-shrinkX/2.0+resizeX,heightTile*div+resizeY,5});
}

void Level::setTileSet(const char* value, SDL_Renderer* renderer)
{
    surfTileSet = IMG_Load(value);
    if(surfTileSet)
    {
        textTileSet = SDL_CreateTextureFromSurface(renderer,surfTileSet);
        if(!textTileSet)
        {
            std::cout<<"Texture "<<value<<" dont created."<<std::endl;
        }
    }
    else{std::cout<<"Surface "<<value<<" dont created."<<std::endl;}
}

void Level::insertOriginShape(const int& x, const int& y, const int& w, const int& h, const int& numOfShape)
{
    if(!originShapes.insert({numOfShape,{x,y,w,h}}).second){std::cout<<"Num of Shape is already in use"<<std::endl;}
}

void Level::setBackground(const char* value, SDL_Renderer* renderer)
{
    surfBackground = IMG_Load(value);
    if(surfBackground)
    {
        textBackground = SDL_CreateTextureFromSurface(renderer,surfBackground);
        if(!textBackground)
        {
            std::cout<<"Texture "<<value<<" dont created."<<std::endl;
        }
    }
    else{std::cout<<"Surface "<<value<<" dont created."<<std::endl;}
}

void Level::setOriginBackground(const int& x, const int& y, const int& w, const int& h){originBackground = {x,y,w,h};}

void Level::printMapConsole()
{
    for(int i=0; i<matrix.size(); ++i)
    {
        for(int j=0; j<matrix[0].size(); ++j)
        {
            if(matrix[i][j]==-1){std::cout<<"*";}
            else{std::cout<<matrix[i][j];}
            std::cout<<" ";
        }
        std::cout<<std::endl;
    }
}

//De la matriz cuenta las filas y columnas donde hayan salas.
void Level::getRowsColumns()
{
    if(!matrix.empty())
    {
        for(int i=0; i<matrix.size(); ++i)
        {
            std::pair<int,int> memory = {rows,columns};
            for(int j=0; j<matrix[0].size(); ++j)
            {
                if(matrix[i][j]!=-1&&memory.first==rows)
                {
                    ++rows;
                }
                if(matrix[j][i]!=-1&&memory.second==columns)
                {
                    ++columns;
                }
                if(memory.first!=rows&&memory.second!=columns){break;}
            }
        }
    }
}

void Level::reduceMatrix()
{
    std::vector<std::vector<int>> reduced;
    if(!matrix.empty())
    {
        for(int i=0; i<matrix.size(); ++i)
        {
            for(int j=0; j<matrix[0].size(); ++j)
            {
                if(matrix[i][j]!=-1)
                {
                    reduced.push_back(matrix[i]);
                    break;
                }
            }
        }
    }
    matrix = reduced;
}

void Level::drawMap(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderCopy(renderer,textBackground,&originBackground,NULL);
    draw(shapesMap,dimensionsMap,renderer);
    SDL_RenderPresent(renderer);
}

void Level::drawRoom(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    draw(shapesRoom,dimensionsRoom,renderer);
}

void Level::drawRoomLastFrame(SDL_Renderer* renderer)
{
    draw(lowerFrameRoom,dimensionsRoom,renderer);
}

void Level::draw(const std::vector<PosShape>& shapes, const std::unordered_map<int,std::pair<int,int>>& dimensions, SDL_Renderer* renderer)
{
    SDL_Rect destTileSet;
    std::pair<int,int> widthHeight;
    for(PosShape shape : shapes)
    {
        destTileSet.x = std::get<0>(shape);
        destTileSet.y = std::get<1>(shape);
        widthHeight = dimensions.at(std::get<2>(shape));
        destTileSet.w = widthHeight.first;
        destTileSet.h = widthHeight.second;
        SDL_RenderCopy(renderer,textTileSet,&originShapes.at(std::get<2>(shape)),&destTileSet);
    }
}