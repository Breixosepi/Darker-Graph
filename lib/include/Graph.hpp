#pragma once

#include "Room.hpp"
#include <random>

class Graph
{
    public:

        Graph();
        ~Graph();

        Designar::Graph<int> createMap();

    private:

        int numRooms;
        int maxRooms;
        double probability;
        std::pair<int,int> even_odd;

        Designar::Graph<int> map;
        std::vector<Designar::Graph<int>::Node*> roomsReference;
        std::queue<Designar::Graph<int>::Node*> queue;

        void getData();

        void printMap();

        void insertRoom(Designar::Graph<int>::Node*& room);

        void generateEntry(std::mt19937 random, std::uniform_real_distribution<double> distribution);

        void generateEvenRooms(Designar::Graph<int>::Node*& room, std::mt19937 random, std::uniform_real_distribution<double> distribution);

        bool limitRoom(Designar::Graph<int>::Node*& room);
};
