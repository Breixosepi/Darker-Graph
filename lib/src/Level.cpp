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
    if(!posShapes.empty())
    {
        posShapes.clear();
        dimensionShapes.clear();
    }
    if(!matrix.empty())
    {
        double square = std::min(width/(columns+1),height/(rows+1)); //se escoge la celda mas pequena
        double shrink = square/4; //se encoge la celda para dejar espacio para los pasillos
        double tile = square-shrink;
        double X = (width-square*columns)/2.0 - shrink/2;
        dimensionShapes.insert({0,tile});
        dimensionShapes.insert({1,tile});
        dimensionShapes.insert({2,shrink});
        for(int i=0; i<matrix[0].size(); ++i)
        {
            double Y = (height-square*rows)/2.0 - shrink/2;
            int memory = posShapes.size();
            for(int j=0; j<matrix.size(); ++j)
            {
                if(matrix[j][i]!=-1)
                {
                    posShapes.push_back(std::make_tuple(X+shrink,Y+shrink,0));
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
                                        posShapes.push_back(std::make_tuple(X,Y+square/2,2));
                                    break;
                                    case 1: //Arriba
                                        posShapes.push_back(std::make_tuple(X+square/2,Y,2));
                                    break;
                                    case 2: //Derecha
                                        posShapes.push_back(std::make_tuple(X+square,Y+square/2,2));
                                    break;
                                    case 3: //Abajo
                                        posShapes.push_back(std::make_tuple(X+square/2,Y+square,2));
                                    break;
                                }
                            }
                        }
                    }
                    else{posShapes.push_back(std::make_tuple(X+shrink,Y+shrink,1));}
                }
                Y += square;
            }
            if(memory!=posShapes.size()){X += square;}
        }
    }
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
    SDL_Rect destTileSet;
    for(PosShape shape : posShapes)
    {
        destTileSet.x = std::get<0>(shape);
        destTileSet.y = std::get<1>(shape);
        destTileSet.w = destTileSet.h = dimensionShapes.at(std::get<2>(shape));
        SDL_RenderCopy(renderer,textTileSet,&originShapes.at(std::get<2>(shape)),&destTileSet);
    }
    SDL_RenderPresent(renderer);
}

void Level::drawRoom(const int& index, const int& width, const int& height, SDL_Renderer* renderer)
{
    SDL_Rect destTileSet;
    SDL_Rect originWall = {304,400,64,48};
    SDL_Rect originColumn = {640,112,16,80};
    double div = 6.0;
    double cell = std::min(width/div,height/div);
    double shrink = cell/6.0;
    double square = cell - shrink;
    int countColumns = 0;
    std::queue<SDL_Rect> queue;
    int Y = (height-cell*div)/2.0 + shrink*div/2;
    destTileSet.w = destTileSet.h = square;
    for(int i=0; i<div; ++i)
    {
        int X = (width-cell*div)/2.0 + shrink*div/2;
        for(int j=0; j<div; ++j)
        {
            destTileSet.x = X;
            destTileSet.y = Y;
            SDL_RenderCopy(renderer,textTileSet,&originShapes.at(0),&destTileSet);
            if(i==0 || i==div-1)
            {
                destTileSet.y -= destTileSet.h;
                if(i==div-1){destTileSet.y += cell;}
                SDL_RenderCopyEx(renderer,textTileSet,&originWall,&destTileSet,0,NULL,SDL_FLIP_NONE);
                if(j==0 || j==div-1)
                {
                    destTileSet.w = shrink*2;
                    destTileSet.x -= destTileSet.w - destTileSet.w/4;
                    if(j==div-1){destTileSet.x += cell;}
                    if(countColumns<2)
                    { 
                        SDL_RenderCopyEx(renderer,textTileSet,&originColumn,&destTileSet,0,NULL,SDL_FLIP_NONE);
                        ++countColumns;
                    }
                    else
                    {
                        queue.push(destTileSet);
                    }
                    destTileSet.w = square;
                    destTileSet.x = X;
                }
                destTileSet.y = Y;    
            }
            if(j==0 || j==div-1)
            {
                destTileSet.x -= shrink;
                if(j==div-1){destTileSet.x += cell;}
                destTileSet.w = shrink;
                destTileSet.y -= shrink;
                SDL_RenderCopyEx(renderer,textTileSet,&originWall,&destTileSet,0,NULL,SDL_FLIP_NONE); 
                destTileSet.x = X; 
                destTileSet.w = square;
                destTileSet.y = Y;
            }
            X+=square;
        }
        Y+=square;
    }
    while(!queue.empty())
    {
        destTileSet = queue.front();
        queue.pop();
        SDL_RenderCopyEx(renderer,textTileSet,&originColumn,&destTileSet,0,NULL,SDL_FLIP_NONE);
    } 
}