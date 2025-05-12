#pragma once

#include <Utilities.hpp>
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

        const int* getIndex();
        const std::vector<bool>* getPaths();
        const std::pair<int,int>* getPos();

        void setPos(const std::pair<int,int>& value);
        void setPath(const int& side);

    private:
        int index;
        std::pair<int,int> posInMatrix;
        std::vector<bool> paths;
};