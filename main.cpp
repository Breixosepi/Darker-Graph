#include <iostream>
#include <queue>
#include <vector>
#include <set>
#include <random>
#include "graph.hpp"

void getData(int& numRooms, double& probability, int& maxRooms);
void printMap(const Designar::Graph<int>& map);
void insertRoom(Designar::Graph<int>::Node*& room, Designar::Graph<int>& map, std::vector<Designar::Graph<int>::Node*>& roomsReference, std::queue<Designar::Graph<int>::Node*>& queue, std::pair<int,int>& pair);
void generateEntry(Designar::Graph<int>& map, std::vector<Designar::Graph<int>::Node*>& roomsReference, std::queue<Designar::Graph<int>::Node*>& queue, std::pair<int,int>& count, const int& numRooms, const int& maxRooms, std::mt19937 random, std::uniform_real_distribution<double> distribution, const double& probability);
void generateEvenRooms(Designar::Graph<int>::Node*& room, Designar::Graph<int>& map, std::vector<Designar::Graph<int>::Node*>& roomsReference, std::queue<Designar::Graph<int>::Node*>& queue, std::pair<int,int>& count, const int& numRooms, const int& maxRooms, std::mt19937 random, std::uniform_real_distribution<double> distribution, const double& probability);
bool limitRoom(Designar::Graph<int>::Node*& room, Designar::Graph<int>& map, const int& num, const int& max);

int main()
{
    int numRooms = -1;
    int maxRooms = -1;
    double probability = -1.0;
    std::pair<int,int> even_odd{0,0};
    std::mt19937 random(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0, 1.0);

    getData(numRooms,probability,maxRooms);

    Designar::Graph<int> map;
    std::vector<Designar::Graph<int>::Node*> roomsReference;
    std::queue<Designar::Graph<int>::Node*> queue;

    generateEntry(map,roomsReference,queue,even_odd,numRooms,maxRooms,random,distribution,probability);

    while(!queue.empty())
    {
        auto room = queue.front();
        queue.pop();
        generateEvenRooms(room,map,roomsReference,queue,even_odd,numRooms,maxRooms,random,distribution,probability);
        if(map.get_num_nodes()==numRooms){break;}
    }

    printMap(map);
    if(even_odd.second==2){std::cout<<"Hay un camino euleriano!"<<std::endl;}
    else
    {
        std::cout<<"Corrigiendo grafo..."<<std::endl;
        int aux = 0;
        if(!queue.empty())
        {
            while(aux<queue.size()-1)
            {
                map.insert_arc(roomsReference[numRooms-queue.size()+aux/2],roomsReference[numRooms-2-aux/2]);
                aux = aux+2;
                even_odd.second = even_odd.second-2;
            }
        }
        if(even_odd.second==2)
        {
            printMap(map);
            std::cout<<"Hay un camino euleriano!"<<std::endl;
        }
        else
        {
            std::cout<<"No hay camino euleriano."<<std::endl;
        }
    }
    if(map.get_num_nodes()==numRooms){std::cout<<"Numero de Cuartos Correcto!"<<std::endl;}
    else{std::cout<<"Mapa incompleto."<<std::endl;}
    return 0;
}

void getData(int& numRooms, double& probability, int& maxRooms)
{
    std::cout<<"TRY Grafo con camino euleriano"<<std::endl;
    std::cout<<"Numero de Cuartos a Generar: ";
    std::cin>>numRooms;
    std::cout<<std::endl;
    std::cout<<"Porcentaje para su generacion: ";
    std::cin>>probability;
    std::cout<<std::endl;
    if(probability>100.0){probability=100.0;}
    else if(probability<0){probability=0;}
    std::cout<<"Grado Maximo para Cuartos: ";
    std::cin>>maxRooms;
    std::cout<<std::endl;
    if(maxRooms>numRooms){maxRooms=numRooms;}
    else if(maxRooms<0){maxRooms=0;}
}

void printMap(const Designar::Graph<int>& map)
{
    map.for_each_node([&](Designar::Graph<int>::Node* room)
    {
        std::cout<<"Num of Room: "<<room->get_info();
        for(auto arc : map.adjacent_arcs(room))
        {
            if(arc->get_src_node()->get_info()!=room->get_info())
            {
                std::cout<<" "<<arc->get_tgt_node()->get_info()<<" -> "<<arc->get_src_node()->get_info();
            }
            else
            {
                std::cout<<" "<<arc->get_src_node()->get_info()<<" -> "<<arc->get_tgt_node()->get_info();
            }
        }
        if(room->get_num_arcs()%2==0){std::cout<<"  Grado Par";}
        else{std::cout<<"  Grado Impar";}
        std::cout<<std::endl;
    });
}

void insertRoom(Designar::Graph<int>::Node*& room, Designar::Graph<int>& map, std::vector<Designar::Graph<int>::Node*>& roomsReference, std::queue<Designar::Graph<int>::Node*>& queue, std::pair<int,int>& pair)
{
    auto newRoom = map.insert_node(map.get_num_nodes()+1);
    roomsReference.push_back(newRoom);
    queue.push(newRoom);
    map.insert_arc(room,newRoom);
    ++pair.second;
    if(room->get_num_arcs()%2==0)
    {
        ++pair.first;
        --pair.second;
    }
    else{++pair.second;}
}

void generateEntry(Designar::Graph<int>& map, std::vector<Designar::Graph<int>::Node*>& roomsReference, std::queue<Designar::Graph<int>::Node*>& queue, std::pair<int,int>& count, const int& numRooms, const int& maxRooms, std::mt19937 random, std::uniform_real_distribution<double> distribution, const double& probability)
{
    auto room = map.insert_node(1);
    roomsReference.push_back(room);
    if(maxRooms>0)
    {
        insertRoom(room,map,roomsReference,queue,count);
        while (room->get_num_arcs()<maxRooms)
        {
            if(distribution(random)<=probability/100.0 && limitRoom(room,map,numRooms,maxRooms))
            {
                insertRoom(room,map,roomsReference,queue,count);
                insertRoom(room,map,roomsReference,queue,count);
            }
            else{break;}
        }
    }   
}

void generateEvenRooms(Designar::Graph<int>::Node*& room, Designar::Graph<int>& map, std::vector<Designar::Graph<int>::Node*>& roomsReference, std::queue<Designar::Graph<int>::Node*>& queue, std::pair<int,int>& count, const int& numRooms, const int& maxRooms, std::mt19937 random, std::uniform_real_distribution<double> distribution, const double& probability)
{
    while(room->get_num_arcs()<maxRooms)
    {
        if(room->get_num_arcs()%2!=0) //Impar
        {
            insertRoom(room,map,roomsReference,queue,count);
        }
        else //Par
        {
            if(distribution(random)<=probability/100.0 && limitRoom(room,map,numRooms,maxRooms))
            {
                insertRoom(room,map,roomsReference,queue,count);
            }
            else{break;}
        }  
    }
}

bool limitRoom(Designar::Graph<int>::Node*& room, Designar::Graph<int>& map, const int& num, const int& max)
{
    return map.get_num_nodes()+2<=num && room->get_num_arcs()+2<=max;
}