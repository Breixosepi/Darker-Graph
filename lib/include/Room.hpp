#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <graph.hpp>

class Room
{
    public:
        Room();
        Room(const int& index);
        ~Room();

        int getIndex();
        std::pair<int,int> getPos();

        void setPos(std::pair<int,int> value);

    private:
        int index;
        std::pair<int,int> posInMatrix;
};