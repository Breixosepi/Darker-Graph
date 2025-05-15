#include <Room.hpp>

Room::Room()
{
    posInMatrix = {-1, -1};
    index = -1;
    paths = std::vector<bool>({false,false,false,false});
}

Room::Room(const int& num)
{
    posInMatrix = {-1, -1};
    index = num;
    paths = std::vector<bool>({false,false,false,false});
}

Room::~Room(){}

const int* Room::getIndex(){return &index;}

const std::vector<bool>* Room::getPaths(){return &paths;}

const std::pair<int,int>* Room::getPos(){return &posInMatrix;}

const std::vector<PosShape> Room::getShapes(){return shapes;}

void Room::setPos(const std::pair<int,int>& value){posInMatrix = value;}

void Room::setPath(const int& side){paths[side]=!paths[side];}

void Room::setShapes(const std::vector<PosShape> value){shapes = value;}