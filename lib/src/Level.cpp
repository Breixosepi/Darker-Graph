#include <Level.hpp>

Level::Level(){}

Level::Level(std::tuple<Designar::Graph<Room>,std::vector<Designar::Graph<Room>::Node*>,std::vector<std::vector<int>>> tuple)
{
    map = std::get<0>(tuple);
    roomsReference = std::get<1>(tuple);
    rows = columns = 0;
    windowWidth = windowHeight = -1;
    if(map.get_num_nodes()!=0){currentIndex = 1;}
    else{currentIndex = -1;}
    setMatrix(std::get<2>(tuple));
}

Level::~Level(){}

const Designar::Graph<Room>* Level::getMap(){return &map;}
        
const Designar::Graph<Room>* Level::getShortestPath(){return &shortestPath;}
        
const Designar::Graph<Room>* Level::getEulerianPath(){return &eulerianPath;}
        
const std::vector<std::vector<int>>* Level::getMatrix(){return &matrix;}

const std::vector<Designar::Graph<Room>::Node*>* Level::getRoomsReference(){return &roomsReference;}

Room* Level::getCurrentRoom(){return &roomsReference[currentIndex-1]->get_info();}

void Level::setMap(const Designar::Graph<Room>& value){map = value;}

void Level::setShortestPath(const Designar::Graph<Room>& value){shortestPath = value;}
        
void Level::setEulerianPath(const Designar::Graph<Room>& value){eulerianPath = value;}

void Level::setCurrentIndex(const int& value){currentIndex = value;}

double Level::setWindowSize(const int& width, const int& height)
{
    windowWidth = width;
    windowHeight = height;
    setShapesMap();
    return setDesignRoom(false);
}

void Level::setMatrix(const std::vector<std::vector<int>>& value)
{
    matrix = value;
    getRowsColumns();
    reduceMatrix();
}

//Actualiza el vector de figuras a dibujar para mostrar el mapa del presente nivel.
//Trabaja con el ultimo tamano de ventana establecido.
void Level::setShapesMap()
{
    if(!shapesMap.empty())
    {
        shapesMap.clear();
        dimensionsMap.clear();
    }
    if(!matrix.empty())
    {
        double square = std::min(windowWidth/(columns+1),windowHeight/(rows+1)); //se escoge la celda mas pequena
        double shrink = square/4.0; //se encoge la celda para dejar espacio para los pasillos
        double tile = square-shrink;
        double X = (windowWidth-square*columns)/2.0 - shrink/2.0;
        dimensionsMap.insert({0,std::make_pair(tile,tile)});
        dimensionsMap.insert({1,std::make_pair(tile,tile)});
        dimensionsMap.insert({2,std::make_pair(shrink,shrink)});
        for(int i=0; i<matrix[0].size(); ++i)
        {
            double Y = (windowHeight-square*rows)/2.0 - shrink/2.0;
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
                                        shapesMap.push_back(std::make_tuple(X,Y+square/2.0,2));
                                    break;
                                    case 1: //Arriba
                                        shapesMap.push_back(std::make_tuple(X+square/2.0,Y,2));
                                    break;
                                    case 2: //Derecha
                                        shapesMap.push_back(std::make_tuple(X+square,Y+square/2.0,2));
                                    break;
                                    case 3: //Abajo
                                        shapesMap.push_back(std::make_tuple(X+square/2.0,Y+square,2));
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

//Actualiza el vector de figuras a dibujar para mostrar el fondo de los cuartos.
//Trabaja con el ultimo tamano de ventana establecido.
//Tambien se puede ver modificado a traves de alguno de los 2 parametros que recibe
double Level::setDesignRoom(const bool& centered)
{
    if(!shapesRoom.empty())
    {
        shapesRoom.clear();
        dimensionsRoom.clear();
        lowerFrameRoom.clear();
        doors.clear();
    }
    int extraDim = 2;
    double div = *getCurrentRoom()->getDivisions();
    double cellWidth = windowWidth/(div+extraDim);
    double cellHeight = windowHeight/(div+extraDim);
    if(centered){cellHeight = cellWidth = std::min(cellHeight,cellWidth);}
    double shrinkX = cellWidth/4.0; //Ancho pared Vertical //Hitbox!
    double shrinkY = cellHeight/2.0; //Alto pared hotizontal
    double widthTile = (cellWidth*(div+extraDim) - shrinkX*2.0)/div;
    double heightTile = (cellHeight*(div+extraDim) - shrinkY*2.0)/div;

    dimensionsRoom.insert({0,std::make_pair(widthTile,heightTile)}); //piso
    dimensionsRoom.insert({3,std::make_pair(widthTile,heightTile)}); //pared horizontal
    dimensionsRoom.insert({4,std::make_pair(shrinkX,heightTile+(shrinkY+heightTile)/div)}); //pared vertical, en el alto se considera los margenes hacia arriba y hacia abajo para dibujar las horizontales
    dimensionsRoom.insert({5,std::make_pair(shrinkX*2.0,heightTile)}); //columna
    dimensionsRoom.insert({6,std::make_pair(widthTile,heightTile)}); //puerta superior inferior
    dimensionsRoom.insert({7,std::make_pair(widthTile,heightTile)}); //oscuridad puerta
    dimensionsRoom.insert({8,std::make_pair(shrinkX,heightTile)}); //puerta lateral
    dimensionsRoom.insert({9,std::make_pair(shrinkX,heightTile)}); //marco puerta lateral

    double resizeY = (windowHeight-cellHeight*(div+extraDim))/2.0;
    double resizeX = (windowWidth-cellWidth*(div+extraDim))/2.0;
    double Y = 0.75*heightTile + resizeY;
    double supY, infY = -1.0;
    for(int i=0; i<div; ++i)
    {
        double X = shrinkX + resizeX;
        for(int j=0; j<div; ++j)
        {
            shapesRoom.push_back({X,Y,0}); //Baldosas
            if(i==0 || i==div-1)
            {
                if(i<div-1)
                {
                    supY = Y-heightTile; //Linea de los frames mas superiores
                    shapesRoom.push_back({X,supY,3}); //Paredes Superiores
                }
                else
                {
                    infY = Y+0.75*heightTile; //Linea de los frames mas inferiores, se tapa un cuarto de la baldosa inferior
                    lowerFrameRoom.push_back({X,infY,3}); //Paredes Inferiores
                }
                if(i==0&&j==div-1){shapesRoom.push_back({widthTile*div+shrinkX/2.0+resizeX,supY,5});} //Columnas Superior Derecha
            }
            if(j==0 || j==div-1)
            {
                if(i==0&&j==0){shapesRoom.push_back({-shrinkX/2.0+resizeX,supY,5});} //Columna Superior Izquierda
                if(j<div-1){shapesRoom.push_back({X-shrinkX,Y-shrinkY,4});} //Paredes en Vertical a la izquierda(delgadas)
                else{shapesRoom.push_back({X+widthTile,Y-shrinkY,4});} //Paredes en Vertical a la derecha
            }
            X+=widthTile;
        }
        Y+=heightTile;
    }

    //Ubicacion Puertas
    double middleY = supY+heightTile*(div+1)/2.0;
    double middleX = widthTile*(div-1)/2.0+shrinkX;

    doors.push_back({resizeX,resizeY+middleY,9}); //izquierda
    doors.push_back({resizeX,resizeY+middleY,8});

    doors.push_back({resizeX+middleX,supY,7}); //arriba
    doors.push_back({resizeX+middleX,supY,6});

    doors.push_back({resizeX+shrinkX+widthTile*(div),resizeY+middleY,9}); //derecha
    doors.push_back({resizeX+shrinkX+widthTile*(div),resizeY+middleY,8});

    doors.push_back({middleX,infY,7}); //abajo
    doors.push_back({middleX,infY,6});
    //Columnas inferiores
    lowerFrameRoom.push_back({widthTile*div+shrinkX/2.0+resizeX,infY,5});
    lowerFrameRoom.push_back({-shrinkX/2.0+resizeX,infY,5});

    return shrinkX;
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

void Level::insertSourceShape(const int& x, const int& y, const int& w, const int& h, const int& numOfShape)
{
    if(!sourceShapes.insert({numOfShape,{x,y,w,h}}).second){std::cout<<"Num of Shape is already in use"<<std::endl;}
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

void Level::setSourceBackground(const int& x, const int& y, const int& w, const int& h){sourceBackground = {x,y,w,h};}

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
    SDL_RenderCopy(renderer,textBackground,&sourceBackground,NULL);
    draw(shapesMap,dimensionsMap,renderer);
    SDL_RenderPresent(renderer);
}

void Level::drawRoom(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    draw(shapesRoom,dimensionsRoom,renderer);
    drawDoors(renderer);
}

void Level::drawDoors(SDL_Renderer* renderer)
{
    SDL_Rect destDoor;
    SDL_Rect destBonus;
    for(int i=0; i<3; ++i)
    {
        if((*getCurrentRoom()->getPaths())[i])
        {
            destDoor = fillRect(doors[i*2+1],dimensionsRoom);
            destBonus = fillRect(doors[i*2],dimensionsRoom);
            if(i==0||i==2)
            {
                SDL_RenderCopy(renderer,textTileSet,&sourceShapes.at(9),&destBonus);
                SDL_RenderCopy(renderer,textTileSet,&sourceShapes.at(8),&destDoor);
            }
            else
            {
                SDL_RenderCopy(renderer,textTileSet,&sourceShapes.at(7),&destBonus);
                SDL_RenderCopy(renderer,textTileSet,&sourceShapes.at(6),&destDoor);
            }
        }
    }
}

void Level::drawRoomLastFrame(SDL_Renderer* renderer)
{
    draw(lowerFrameRoom,dimensionsRoom,renderer);
    if((*getCurrentRoom()->getPaths())[3])
    {
        SDL_Rect destDoor = fillRect(doors[3*2+1],dimensionsRoom);
        SDL_Rect destBonus = fillRect(doors[3*2],dimensionsRoom);
        SDL_RenderCopy(renderer,textTileSet,&sourceShapes.at(7),&destBonus);
        SDL_RenderCopy(renderer,textTileSet,&sourceShapes.at(6),&destDoor);
    }
}

void Level::draw(const std::vector<PosShape>& shapes, const std::unordered_map<int,std::pair<int,int>>& dimensions, SDL_Renderer* renderer)
{
    SDL_Rect destTileSet;
    for(PosShape shape : shapes)
    {
        destTileSet = fillRect(shape,dimensions);
        SDL_RenderCopy(renderer,textTileSet,&sourceShapes.at(std::get<2>(shape)),&destTileSet);
    }
}

SDL_Rect Level::fillRect(const PosShape& shape, const std::unordered_map<int,std::pair<int,int>>& dimensions)
{
    SDL_Rect dest;
    std::pair<int,int> widthHeight;
    dest.x = std::get<0>(shape);
    dest.y = std::get<1>(shape);
    widthHeight = dimensions.at(std::get<2>(shape));
    dest.w = widthHeight.first;
    dest.h = widthHeight.second;
    return dest;
}