#include <Room.hpp>

Room::Room()
{
    posInMatrix = {-1, -1};
    index = -1;
}

Room::Room(const int& num)
{
    posInMatrix = {-1, -1};
    index = num;
}

Room::~Room(){}

int Room::getIndex(){return index;}

std::pair<int,int> Room::getPos(){return posInMatrix;}

void Room::setPos(std::pair<int,int> value){posInMatrix = value;}