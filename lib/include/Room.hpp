#pragma once

#include <Utilities.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <graph.hpp>

using PosShape = std::tuple<int,int,int>; //Get<0>=X, Get<1>=Y, Get<2>=typeShape 

class Room
{
    public:
        Room();
        Room(const int& index);
        ~Room();

        const int* getIndex();
        const std::vector<bool>* getPaths();
        const std::pair<int,int>* getPos();
        const std::vector<PosShape>* getShapes();
        const double* getDivisions();

        void setPos(const std::pair<int,int>& value);
        void setPath(const int& side);
        void setShapes(const std::vector<PosShape> value);
        void setDivisions(const double& value);

    private:
        int index;
        std::pair<int,int> posInMatrix;
        std::vector<bool> paths;
        std::vector<PosShape> shapes;
        double divisions;
};