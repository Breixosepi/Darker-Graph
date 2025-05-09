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

std::vector<std::pair<std::pair<int,int>,int>> Level::shapesToDrawMap(const int& width, const int& height, int& square)
{
    std::vector<std::pair<std::pair<int,int>,int>> positions; //pair.second tipo de figura 0:cuarto, 1:cuarto-portal, 2:pasillo horizontal, 3:pasillo vertical
    if(!matrix.empty())
    {
        const double resizeSquare = 1.5; //con 1.25 parece quedar grande
        const int cellWidth = width/(matrix[0].size()+1);
        const int cellHeight = height/(matrix.size()+1);
        square = cellWidth;
        if(rowsColumns.first!=1&&rowsColumns.second!=1){square = square*resizeSquare;}
        int Y = cellHeight;
        for(int i=0; i<matrix.size(); ++i)
        {
            int X = cellWidth;
            for(int j=0; j<matrix[0].size(); ++j)
            {
                if(matrix[i][j]!=-1)
                {
                    positions.push_back({{X-(square/2),Y-(square/2)},0});
                    if(matrix[i][j]>-1)
                    {
                        std::vector<bool> paths = *roomsReference[matrix[i][j]-1]->get_info().getPaths();
                        for(int i=0; i<paths.size(); ++i)
                        {
                            if(paths[i])
                            {
                                switch (i)
                                {
                                    case 0:
                                        positions.push_back({{X-square,Y-(square/4)},2});
                                    break;
                                    case 1:
                                        positions.push_back({{X-(square/4),Y-square},3});
                                    break;
                                    case 2:
                                        positions.push_back({{X,Y-(square/4)},2});
                                    break;
                                    case 3:
                                        positions.push_back({{X-(square/4),Y},3});
                                    break;
                                }
                            }
                        }
                    }
                    else{positions.push_back({{X-(square/2),Y-(square/2)},1});}
                }
                X += square;
            }
            Y += square;
        }
    }
    return positions;
}