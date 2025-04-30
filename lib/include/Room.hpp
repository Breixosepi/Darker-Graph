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

    private:
        int index;
};