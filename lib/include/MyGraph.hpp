#pragma once

#include <Room.hpp>
#include <random>

class MyGraph
{
    public:

        MyGraph();
        ~MyGraph();

        std::pair<Designar::Graph<Room>,std::vector<std::vector<int>>> createMap(const bool& custom);

    private:

        int numRooms;
        int maxGrade;
        double probability;
        std::pair<int,int> even_odd;

        Designar::Graph<Room> map;
        std::vector<Designar::Graph<Room>::Node*> roomsReference;
        std::queue<Designar::Graph<Room>::Node*> queue;
        std::vector<std::vector<int>> matrix;

        void getData();

        void printWrittenGraph();

        void insertRoom(Designar::Graph<Room>::Node*& room, int side);

        void generateEntry(std::mt19937 random, std::uniform_real_distribution<double> add, std::uniform_int_distribution<int> side);

        void generateEvenRooms(Designar::Graph<Room>::Node*& room, std::mt19937 random, std::uniform_real_distribution<double> add, std::uniform_int_distribution<int> side);

        void fixMap(std::mt19937 random, std::uniform_int_distribution<int> side);

        bool limitRoom(Designar::Graph<Room>::Node*& room);

        bool verifyInsert(int side, std::pair<int,int>& pos);
        
        void reset();
};
