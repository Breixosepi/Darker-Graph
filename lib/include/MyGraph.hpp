#pragma once

#include <Room.hpp>
#include <random>
#include <unordered_set>

using Dungeon = std::tuple<Designar::Graph<Room>,std::vector<Designar::Graph<Room>::Node*>,std::vector<std::vector<int>>>;

class MyGraph
{
    public:

        MyGraph();
        ~MyGraph();

        Dungeon createMap(const bool& custom);

    private:

        int numRooms;
        int maxGrade;
        double probability;
        std::pair<int,int> evenOdd;

        Designar::Graph<Room> map;
        std::vector<Designar::Graph<Room>::Node*> roomsReference;
        std::queue<Designar::Graph<Room>::Node*> queue;
        std::vector<std::vector<int>> matrix;
        std::unordered_set<int> neighborsEntry;

        bool verifyCreateMap(const bool& custom);
        void getData();
        void insertRoom(Designar::Graph<Room>::Node*& room, int side);
        Designar::Graph<Room>::Node* helperInsert(const std::pair<int,int>& pos);
        void countEvenOdd(Designar::Graph<Room>::Node*& room);
        bool verifyInsert(int& side, std::pair<int,int>& pos);
        void insertPath(Designar::Graph<Room>::Node*& room, Designar::Graph<Room>::Node*& neighbor, int side);
        bool verifyPath(int& side, std::pair<int,int>& pos);
        void generateEntry(std::mt19937 random, std::uniform_real_distribution<double> add, std::uniform_int_distribution<int> side);
        void generateEvenRooms(Designar::Graph<Room>::Node*& room, std::mt19937 random, std::uniform_real_distribution<double> add, std::uniform_int_distribution<int> side);
        bool limitRoom(Designar::Graph<Room>::Node*& room);
        bool fixMap(std::mt19937 random, std::uniform_int_distribution<int> side);
        std::pair<int,int> getRowsColumns();
        void reduceMatrix();
        int searchOdds();
        void printLastGraph(); 
        void reset();
};
