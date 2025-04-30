#pragma once

#include <Room.hpp>
#include <random>

class MyGraph
{
    public:

        MyGraph();
        ~MyGraph();

        Designar::Graph<Room> createMap(const bool& custom);

    private:

        int numRooms;
        int maxRooms;
        double probability;
        std::pair<int,int> even_odd;

        Designar::Graph<Room> map;
        std::vector<Designar::Graph<Room>::Node*> roomsReference;
        std::queue<Designar::Graph<Room>::Node*> queue;

        void getData();

        void printMap();

        void insertRoom(Designar::Graph<Room>::Node*& room);

        void generateEntry(std::mt19937 random, std::uniform_real_distribution<double> distribution);

        void generateEvenRooms(Designar::Graph<Room>::Node*& room, std::mt19937 random, std::uniform_real_distribution<double> distribution);

        bool limitRoom(Designar::Graph<Room>::Node*& room);

        void reset();
};
