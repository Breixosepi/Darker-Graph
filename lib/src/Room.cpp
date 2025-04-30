#include <Room.hpp>

Room::Room()
{
    index = -1;
}

Room::Room(const int& num)
{
    index = num;
}

Room::~Room(){};

int Room::getIndex(){return index;}