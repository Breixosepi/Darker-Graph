#include <Level.hpp>

Level::Level(){}

Level::Level(std::tuple<Designar::Graph<Room>,std::vector<Designar::Graph<Room>::Node*>,std::vector<std::vector<int>>> tuple)
{
    map = std::get<0>(tuple);
    roomsReference = std::get<1>(tuple);
    rowsColumns = {0,0};
    setMatrix(std::get<2>(tuple));
}

Level::~Level(){}

const Designar::Graph<Room>* Level::getMap(){return &map;}
        
const Designar::Graph<Room>* Level::getShortestPath(){return &shortestPath;}
        
const Designar::Graph<Room>* Level::getEulerianPath(){return &eulerianPath;}
        
const std::vector<std::vector<int>>* Level::getMatrix(){return &matrix;}

const std::vector<Designar::Graph<Room>::Node*>* Level::getRoomsReference(){return &roomsReference;}

const std::vector<shape>* Level::getShapesMap(){return &shapes;}

void Level::setMap(const Designar::Graph<Room>& value){map = value;}

void Level::setShortestPath(const Designar::Graph<Room>& value){shortestPath = value;}
        
void Level::setEulerianPath(const Designar::Graph<Room>& value){eulerianPath = value;}

void Level::setMatrix(const std::vector<std::vector<int>>& value)
{
    matrix = value;
    getRowsColumns();
    reduceMatrix();
}

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

void Level::getRowsColumns()
{
    if(!matrix.empty())
    {
        for(int i=0; i<matrix.size(); ++i)
        {
            std::pair<int,int> memory = rowsColumns;
            for(int j=0; j<matrix[0].size(); ++j)
            {
                if(matrix[i][j]!=-1&&memory.first==rowsColumns.first)
                {
                    ++rowsColumns.first;
                }
                if(matrix[j][i]!=-1&&memory.second==rowsColumns.second)
                {
                    ++rowsColumns.second;
                }
                if(memory.first!=rowsColumns.first&&memory.second!=rowsColumns.second){break;}
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

void Level::setShapesMap(const int& width, const int& height)
{
    std::vector<shape> result;
    if(!matrix.empty())
    {
        const int cellWidth = width/(rowsColumns.second+1);
        const int cellHeight = height/(rowsColumns.first+1);
        int square = -1;
        if(cellHeight>cellWidth){square = cellWidth;}
        else{square = cellHeight;}
        int dif = square/4;
        int X = cellWidth-square/2-dif/2;
        for(int i=0; i<matrix[0].size(); ++i)
        {
            int Y = cellHeight-square/2-dif/2;
            int memory = result.size();
            for(int j=0; j<matrix.size(); ++j)
            {
                if(matrix[j][i]!=-1)
                {
                    result.push_back(std::make_tuple(X+dif,Y+dif,square-dif,square-dif,0));
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
                                        result.push_back(std::make_tuple(X,Y+square/2,dif,dif,2));
                                    break;
                                    case 1: //Arriba
                                        result.push_back(std::make_tuple(X+square/2,Y,dif,dif,2));
                                    break;
                                    case 2: //Derecha
                                        result.push_back(std::make_tuple(X+square,Y+square/2,dif,dif,2));
                                    break;
                                    case 3: //Abajo
                                        result.push_back(std::make_tuple(X+square/2,Y+square,dif,dif,2));
                                    break;
                                }
                            }
                        }
                    }
                    else{result.push_back(std::make_tuple(X+dif,Y+dif,square-dif,square-dif,1));}
                }
                Y += square;
            }
            if(memory!=result.size()){X += square;}
        }
    }
    shapes = result;
}