#include <Level.hpp>

Level::Level(){}

Level::Level(const Dungeon& level)
{
    map = std::get<0>(level);
    roomsReference = std::get<1>(level);
    currentIndex = 1; //Cuarto 1, Entrada del Nivel
    flagReward = true;
    setMatrix(std::get<2>(level));
    printMapConsole();
}

Level::~Level(){}

const Designar::Graph<Room>* Level::getMap(){return &map;}
        
const Designar::Graph<Room>* Level::getShortestPath(){return &shortestPath;}
        
const Designar::Graph<Room>* Level::getEulerianPath(){return &eulerianPath;}
        
const std::vector<std::vector<int>>* Level::getMatrix(){return &matrix;}

const std::vector<Designar::Graph<Room>::Node*>* Level::getRoomsReference(){return &roomsReference;}

Room* Level::getCurrentRoom(){return &roomsReference[currentIndex-1]->get_info();}

const bool Level::IsEulerianPath()
{
    if(flagReward)
    {
        if(visitedArcs.size()==map.get_num_arcs())
        {
            std::cout<<"right path, eulerian path made!"<<std::endl;
            return true;
        }
        else{std::cout<<"you did not walk down all the hallways."<<std::endl;}
    }
    else{std::cout<<"you walked down the same hallway more than once."<<std::endl;}
    return false;
}

//Devuelve la posicion en la matriz del vecino en la direccion indicada 
const std::pair<int,int> Level::getPosNeighbor(const int& direction)
{
    if(currentIndex>0)
    {
        std::pair<int,int> posNeighbor = *getCurrentRoom()->getPos();
        switch (direction)
        {
            case 0:
                --posNeighbor.second;
                break;
            case 1:
                --posNeighbor.first;
                break;
            case 2:
                ++posNeighbor.second;
                break;
            case 3:
                ++posNeighbor.first;
                break;
        }
        return posNeighbor;
    }
    return std::make_pair(-1,-1);
}

//Devuelve el Index del vecino en la direccion indicada o 0 si no hay cuarto vecino
const int Level::getIndexNeighbor(const int& direction)
{
    std::pair<int,int> posNeighbor = getPosNeighbor(direction);
    if(posNeighbor.first>=0&&posNeighbor.second>=0)
    {
        if(posNeighbor.first<matrix.size()&&posNeighbor.second<matrix[0].size())
        {
            return matrix[posNeighbor.first][posNeighbor.second];
        }
    }
    return 0;
}

//Devuelve un par con numero de Filas y numero de Columnas de la Matriz
const std::pair<int,int> Level::getMatrixSize(){return std::make_pair(matrix.size(),matrix[0].size());}

void Level::setMap(const Designar::Graph<Room>& value){map = value;}

void Level::setShortestPath(const Designar::Graph<Room>& value){shortestPath = value;}
        
void Level::setEulerianPath(const Designar::Graph<Room>& value){eulerianPath = value;}

void Level::calcPosInMap()
{
    std::pair<int,int> pos = *getCurrentRoom()->getPos();
    Measures measures = helper.get()->getMeasuresMap();
    int tile = std::get<0>(measures);
    int square = std::get<0>(measures) + std::get<2>(measures);
    int X = std::get<4>(measures) + std::get<2>(measures)/2.0;
    int Y = std::get<5>(measures) + std::get<3>(measures)/2.0;
    playerInMap = {X+pos.second*(square),Y+pos.first*(square),tile,tile};
}

void Level::setCurrentIndex(const int& value)
{
    currentIndex = value;
    calcPosInMap();
}

void Level::handleResizeWindow()
{
    setShapesMap();
    calcPosInMap();
    setDesignRoom();
}

void Level::setMatrix(const std::vector<std::vector<int>>& value){matrix = value;}

void Level::setDeltaTime(DeltaTime value){animated.setDeltaTime(value);}

void Level::setRenderHelper(HelperPtr value)
{
    helper = value;
    animated.setRenderHelper(value);
    setSources();
    handleResizeWindow();
}

//Actualiza el vector de figuras a dibujar para mostrar el mapa del presente nivel.
//Trabaja con las ultimas dimensiones establecidas en helper
void Level::setShapesMap()
{
    if(!shapesMap.empty())
    {
        shapesMap.clear();
        dimensionsMap.clear();
    }

    Measures measures = helper.get()->getMeasuresMap();
    double tile = std::get<0>(measures);
    double shrink = std::get<2>(measures);
    double square = tile+shrink;
    double resizeX = std::get<4>(measures);
    double resizeY = std::get<5>(measures);

    dimensionsMap.insert({"tile",std::make_pair(tile,tile)});
    dimensionsMap.insert({"trapdoor",std::make_pair(tile,tile)});
    dimensionsMap.insert({"hallway",std::make_pair(shrink,shrink)});

    double X = resizeX - shrink/2;
    for(int i=0; i<matrix[0].size(); ++i)
    {
        double Y = resizeY - shrink/2;
        for(int j=0; j<matrix.size(); ++j)
        {
            if(matrix[j][i]!=0)
            {
                shapesMap.push_back(std::make_tuple(X+shrink,Y+shrink,"tile"));
                if(matrix[j][i]>0)
                {
                    std::vector<bool> paths = *roomsReference[matrix[j][i]-1]->get_info().getPaths();
                    for(int k=0; k<paths.size(); ++k)
                    {
                        if(paths[k])
                        {
                            switch (k)
                            {
                                case 0: //Izquierda
                                    shapesMap.push_back(std::make_tuple(X,Y+square/2.0,"hallway"));
                                break;
                                case 1: //Arriba
                                    shapesMap.push_back(std::make_tuple(X+square/2.0,Y,"hallway"));
                                break;
                                case 2: //Derecha
                                    shapesMap.push_back(std::make_tuple(X+square,Y+square/2.0,"hallway"));
                                break;
                                case 3: //Abajo
                                    shapesMap.push_back(std::make_tuple(X+square/2.0,Y+square,"hallway"));
                                break;
                            }
                        }
                    }
                }
                else{shapesMap.push_back(std::make_tuple(X+shrink,Y+shrink,"trapdoor"));}
            }
            Y += square;
        }
        X += square;
    }
}

//Actualiza el vector de figuras a dibujar para mostrar el fondo de los cuartos.
//Trabaja con las ultimas dimensiones establecidas en helper
void Level::setDesignRoom()
{
    if(!shapesRoom.empty())
    {
        shapesRoom.clear();
        lowerFrameRoom.clear();
        doors.clear();
        dimensionsRoom.clear();
    }
    double div = *getCurrentRoom()->getDivisions();
    Measures measures = helper.get()->getMeasuresRoom(div);
    
    double widthTile = std::get<0>(measures);
    double heightTile = std::get<1>(measures);
    double shrinkX = std::get<2>(measures);
    double shrinkY = std::get<3>(measures);
    double resizeX = std::get<4>(measures);
    double resizeY = std::get<5>(measures);

    dimensionsRoom.insert({"tile",std::make_pair(widthTile,heightTile)}); 
    dimensionsRoom.insert({"portal",std::make_pair(widthTile,heightTile)}); 
    dimensionsRoom.insert({"horizontalWall",std::make_pair(widthTile,heightTile)}); 
    dimensionsRoom.insert({"verticalWall",std::make_pair(shrinkX,heightTile+(shrinkY+heightTile)/div)});
    dimensionsRoom.insert({"column",std::make_pair(shrinkX*2.0,heightTile)}); 
    dimensionsRoom.insert({"horizontalDoor",std::make_pair(widthTile,heightTile)}); 
    dimensionsRoom.insert({"darkness",std::make_pair(widthTile,heightTile)});
    dimensionsRoom.insert({"verticalDoor",std::make_pair(shrinkX,heightTile+(shrinkY+heightTile)/div)});
    dimensionsRoom.insert({"verticalDoorFrame",std::make_pair(shrinkX,heightTile+(shrinkY+heightTile)/div)});

    double Y = shrinkY + resizeY;
    double supY, infY = -1.0;

    for(int i=0; i<div; ++i)
    {
        double X = shrinkX + resizeX;
        for(int j=0; j<div; ++j)
        {
            shapesRoom.push_back({X,Y,"tile"}); //Baldosas
            if(i==0 || i==div-1)
            {
                if(i<div-1)
                {
                    supY = Y-heightTile; //Linea de los frames mas superiores
                    shapesRoom.push_back({X,supY,"horizontalWall"}); //Paredes Superiores
                }
                else
                {
                    infY = Y+0.75*heightTile; //Linea de los frames mas inferiores, se tapa un cuarto de la baldosa inferior
                    lowerFrameRoom.push_back({X,infY,"horizontalWall"}); //Paredes Inferiores
                }
                if(i==0&&j==div-1){shapesRoom.push_back({widthTile*div+shrinkX/2.0+resizeX,supY,"column"});} //Columnas Superior Derecha
            }
            if(j==0 || j==div-1)
            {
                if(i==0&&j==0){shapesRoom.push_back({-shrinkX/2.0+resizeX,supY,"column"});} //Columna Superior Izquierda
                if(j<div-1){shapesRoom.push_back({X-shrinkX,Y-shrinkY,"verticalWall"});} //Paredes en Vertical a la izquierda(delgadas)
                else{shapesRoom.push_back({X+widthTile,Y-shrinkY,"verticalWall"});} //Paredes en Vertical a la derecha
            }
            X+=widthTile;
        }
        Y+=heightTile;
    }

    //Ubicacion Puertas
    double middleY = supY+heightTile*(div+1)/2.0;
    double middleX = widthTile*(div-1)/2.0+shrinkX;

    doors.push_back({resizeX,resizeY+middleY,"verticalDoorFrame"}); //izquierda
    doors.push_back({resizeX,resizeY+middleY,"verticalDoor"});

    doors.push_back({resizeX+middleX,supY,"darkness"}); //arriba
    doors.push_back({resizeX+middleX,supY,"horizontalDoor"});

    doors.push_back({resizeX+shrinkX+widthTile*(div),resizeY+middleY,"verticalDoorFrame"}); //derecha
    doors.push_back({resizeX+shrinkX+widthTile*(div),resizeY+middleY,"verticalDoor"});

    doors.push_back({middleX,infY,"darkness"}); //abajo
    doors.push_back({middleX,infY,"horizontalDoor"});

    //Columnas inferiores
    lowerFrameRoom.push_back({widthTile*div+shrinkX/2.0+resizeX,infY,"column"});
    lowerFrameRoom.push_back({-shrinkX/2.0+resizeX,infY,"column"});
}

void Level::setSources()
{
    helper.get()->setSource("backgroundMap",55,80,260,195);
    helper.get()->setSource("tile",832,208,32,32);
    helper.get()->setSource("trapdoor",486,202,80,80);
    helper.get()->setSource("hallway",832,416,64,64);
    helper.get()->setSource("horizontalWall",304,400,64,48);
    helper.get()->setSource("verticalWall",304,400,64,48);
    helper.get()->setSource("column",640,112,16,80);
    helper.get()->setSource("horizontalDoor",400,16,80,89);
    helper.get()->setSource("darkness",72,52,176,63);
    helper.get()->setSource("verticalDoor",400,112,16,80);
    helper.get()->setSource("verticalDoorFrame",432,112,16,74);
    helper.get()->setSource("dwarfInMap",104,114,15,14);
    helper.get()->setSource("leftStairs",796,54,42,52); //Escaleras que miran a la izquierda
    helper.get()->setSource("rigthStairs",728,54,42,52); //Escaleras que miran a la derecha
}

void Level::printMapConsole()
{
    for(int i=0; i<matrix.size(); ++i)
    {
        for(int j=0; j<matrix[0].size(); ++j)
        {
            if(matrix[i][j]==0){std::cout<<"*";}
            else{std::cout<<matrix[i][j];}
            std::cout<<" ";
        }
        std::cout<<std::endl;
    }
}

void Level::renderMap(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/perg.png",renderer),helper.get()->getSource("backgroundMap"),NULL);
    draw(shapesMap,dimensionsMap,renderer);
    SDL_RenderCopy(renderer,helper.get()->getTexture("assets/sprites/dwarf.png",renderer),helper.get()->getSource("dwarfInMap"),&playerInMap);
    SDL_RenderPresent(renderer);
}

void Level::renderRoom(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    draw(shapesRoom,dimensionsRoom,renderer);
    drawDoors(renderer);
    if(currentIndex==1) //Fogata
    {
        double widthTile = std::get<0>(helper.get()->getMeasuresRoom());
        double heightTile = std::get<1>(helper.get()->getMeasuresRoom());
        SDL_Rect destCampfire = {helper.get()->getMiddlePointInX()-widthTile/4.0,helper.get()->getMiddlePointInY()-heightTile/4.0,widthTile/2.0,heightTile/2.0};
        animated.renderCampfire(destCampfire,renderer);
    }
    if(currentIndex==map.get_num_nodes()) //Escaleras
    {
        double widthTile = std::get<0>(helper.get()->getMeasuresRoom());
        double heightTile = std::get<1>(helper.get()->getMeasuresRoom());
        SDL_Rect destStairs = {helper.get()->getMiddlePointInX()-widthTile/2.0,helper.get()->getMiddlePointInY()-heightTile/2.0,widthTile,heightTile};
        if((*getCurrentRoom()->getPaths())[0])
        {
            SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/tileSet.png",renderer),helper.get()->getSource("leftStairs"),&destStairs);
        }
        else
        {
            SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/tileSet.png",renderer),helper.get()->getSource("rigthStairs"),&destStairs);
        }
    }
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
                SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/tileSet.png",renderer),helper.get()->getSource("verticalDoorFrame"),&destBonus);
                SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/tileSet.png",renderer),helper.get()->getSource("verticalDoor"),&destDoor);
                if(getIndexNeighbor(i)<0)
                {
                    animated.renderCircularPortal(destDoor,renderer);
                }
            }
            else
            {
                if(getIndexNeighbor(i)>0)
                {
                    SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/tileSet.png",renderer),helper.get()->getSource("darkness"),&destBonus);
                    SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/tileSet.png",renderer),helper.get()->getSource("horizontalDoor"),&destDoor);
                }
                else if(getIndexNeighbor(i)<0)
                {
                    animated.renderPortal(destDoor,renderer);
                } 
            }
        }
    }
}

void Level::renderRoomLastFrame(SDL_Renderer* renderer)
{
    draw(lowerFrameRoom,dimensionsRoom,renderer);
    int i = 3;
    if((*getCurrentRoom()->getPaths())[i])
    {
        SDL_Rect destDoor = fillRect(doors[i*2+1],dimensionsRoom);
        SDL_Rect destBonus = fillRect(doors[i*2],dimensionsRoom);
        if(getIndexNeighbor(i)>0)
        {
            SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/tileSet.png",renderer),helper.get()->getSource("darkness"),&destBonus);
            SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/tileSet.png",renderer),helper.get()->getSource("horizontalDoor"),&destDoor);
        }
        else if(getIndexNeighbor(i)<0)
        {
            animated.renderPortal(destDoor,renderer);
        }
    }
}

void Level::draw(const std::vector<PosShape>& shapes, const std::unordered_map<std::string,std::pair<int,int>>& dimensions, SDL_Renderer* renderer)
{
    SDL_Rect destTileSet;
    for(PosShape shape : shapes)
    {
        destTileSet = fillRect(shape,dimensions);
        SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/tileSet.png",renderer),helper.get()->getSource(std::get<2>(shape)),&destTileSet);
    }
}

SDL_Rect Level::fillRect(const PosShape& shape, const std::unordered_map<std::string,std::pair<int,int>>& dimensions)
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

std::pair<int,int> Level::verifyPassRoom(int direction, const SDL_Rect& rectPlayer)
{
    if((*getCurrentRoom()->getPaths())[direction]) //Verifica si hay puerta en dicha direccion
    {
        if(direction==0||direction==2) 
        {
            if(std::get<1>(doors[direction*2+1])<=rectPlayer.y*1.10) //Verifica si el personaje esta alineado con la puerta
            {
                if(std::get<1>(doors[direction*2+1])+(dimensionsRoom.at("verticalDoor")).second>=rectPlayer.y+rectPlayer.h)
                {
                    return passRoom(direction,rectPlayer);
                }
            }
        }
        else if(direction==1||direction==3)
        {
            if(std::get<0>(doors[direction*2+1])<=rectPlayer.x) //Verifica si el personaje esta alineado con la puerta
            {
                if(std::get<0>(doors[direction*2+1])+(dimensionsRoom.at("horizontalDoor")).first>=rectPlayer.x+rectPlayer.w)
                {
                    return passRoom(direction,rectPlayer);
                }
            }
        }
    }
    //Si no estaba alineado, se retorna su misma posicion
    return std::make_pair(rectPlayer.x,rectPlayer.y);     
}

std::pair<int,int> Level::passRoom(int direction, const SDL_Rect& rectPlayer)
{
    int aux = currentIndex; //Index original antes del cambio
    int neighborIndex = getIndexNeighbor(direction);
    setCurrentIndex(std::abs(neighborIndex));
    std::pair<int,int> playerPos;
    for(int i=0; i<4; ++i) //Nos movimos de cuarto y vamos a buscar en que direccion estaba el cuarto del cual venimos
    {
        int X = std::get<0>(doors[direction*2+1]); //Ubicacion de la puerta que se esta revisando
        int Y = std::get<1>(doors[direction*2+1]);
        switch (direction)
        {
        case 0:
            playerPos = std::make_pair(X+rectPlayer.w, Y+(dimensionsRoom.at("verticalDoor").second-rectPlayer.h)/2);
            break;
        case 1:
            playerPos = std::make_pair(X+(dimensionsRoom.at("horizontalDoor").first-rectPlayer.w)/2, Y+rectPlayer.h);
            break;
        case 2:
            playerPos = std::make_pair(X-dimensionsRoom.at("verticalDoor").first-rectPlayer.w, Y+(dimensionsRoom.at("verticalDoor").second-rectPlayer.h)/2);
            break;
        case 3:
            playerPos = std::make_pair(X+(dimensionsRoom.at("horizontalDoor").first-rectPlayer.w)/2, Y-rectPlayer.h);
            break;
        }
        if(neighborIndex>0) //Pasar entre cuarto y cuarto
        {
            if(getIndexNeighbor(direction)==aux)
            {
                if(flagReward){markArc(aux);}
                return playerPos;
            } 
        }
        else //Pasar entre portal y portal
        {
            if(getIndexNeighbor(direction)==-aux)
            {
                if(flagReward){markArc(aux);}
                return playerPos;
            } 
        }
        ++direction;
        if(direction>3){direction -= 4;}
    }
    return std::make_pair(rectPlayer.x,rectPlayer.y);
}

void Level::markArc(const int& comp)
{
    auto arcs = map.adjacent_arcs(roomsReference[currentIndex-1]);
    for(auto it=arcs.begin(); it!=arcs.end(); ++it)
    {
        if(*(*it)->get_src_node()->get_info().getIndex()==comp||*(*it)->get_tgt_node()->get_info().getIndex()==comp)
        {
            if(!visitedArcs.insert(*it).second)
            {
                flagReward = false;
                break;
            }
        }
    }
}