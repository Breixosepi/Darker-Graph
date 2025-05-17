#include <Room.hpp>

Room::Room()
{
    posInMatrix = {-1, -1};
    index = -1;
    paths = std::vector<bool>({false,false,false,false});
    divisions = 1.0;
}

Room::Room(const int& num)
{
    posInMatrix = {-1, -1};
    index = num;
    paths = std::vector<bool>({false,false,false,false});
    divisions = 5.0;
}

Room::~Room(){}

const int* Room::getIndex(){return &index;}

const std::vector<bool>* Room::getPaths(){return &paths;}

const std::pair<int,int>* Room::getPos(){return &posInMatrix;}

const std::vector<PosShape>* Room::getShapes(){return &shapes;}

const double* Room::getDivisions(){return &divisions;}

void Room::setPos(const std::pair<int,int>& value){posInMatrix = value;}

void Room::setPath(const int& side){paths[side]=!paths[side];}

void Room::setShapes(const std::vector<PosShape> value){shapes = value;}

void Room::setDivisions(const double& value){divisions = value;}