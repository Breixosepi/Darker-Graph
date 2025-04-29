#include <Graph.hpp>

Graph::Graph()
{
    numRooms = -1;
    maxRooms = -1;
    probability = -1.0;
    even_odd = {0,0};
}

Graph::~Graph(){};

Designar::Graph<int> Graph::createMap()
{
    getData();
    if(numRooms>0)
    {
        if(map.get_num_nodes()>0)
        {
            map.clear();
            roomsReference.clear();
            while(!queue.empty()){queue.pop();}
        }
        std::mt19937 random(std::random_device{}());
        std::uniform_real_distribution<double> distribution(0, 1.0);

        generateEntry(random,distribution);
        while(!queue.empty())
        {
            auto room = queue.front();
            queue.pop();
            generateEvenRooms(room,random,distribution);
            if(map.get_num_nodes()==numRooms){break;}
        }

        printMap();
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
                printMap();
                std::cout<<"Hay un camino euleriano!"<<std::endl;
            }
            else
            {
                std::cout<<"No hay camino euleriano."<<std::endl;
            }
        }
        if(map.get_num_nodes()==numRooms){std::cout<<"Numero de Cuartos Correcto!"<<std::endl;}
        else{std::cout<<"Mapa incompleto."<<std::endl;}
    }
    return map;
}

void Graph::getData()
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

void Graph::insertRoom(Designar::Graph<int>::Node*& room)
{
    auto newRoom = map.insert_node(map.get_num_nodes()+1);
    roomsReference.push_back(newRoom);
    queue.push(newRoom);
    map.insert_arc(room,newRoom);
    ++even_odd.second;
    if(room->get_num_arcs()%2==0)
    {
        ++even_odd.first;
        --even_odd.second;
    }
    else{++even_odd.second;}
}

void Graph::generateEntry(std::mt19937 random, std::uniform_real_distribution<double> distribution)
{
    auto room = map.insert_node(1);
    roomsReference.push_back(room);
    if(maxRooms>0)
    {
        insertRoom(room);
        while (room->get_num_arcs()<maxRooms)
        {
            if(distribution(random)<=probability/100.0 && limitRoom(room))
            {
                insertRoom(room);
                insertRoom(room);
            }
            else{break;}
        }
    }   
}

void Graph::generateEvenRooms(Designar::Graph<int>::Node*& room, std::mt19937 random, std::uniform_real_distribution<double> distribution)
{
    while(room->get_num_arcs()<maxRooms)
    {
        if(room->get_num_arcs()%2!=0) //Impar
        {
            insertRoom(room);
        }
        else //Par
        {
            if(distribution(random)<=probability/100.0 && limitRoom(room))
            {
                insertRoom(room);
            }
            else{break;}
        }  
    }
}

bool Graph::limitRoom(Designar::Graph<int>::Node*& room)
{
    return map.get_num_nodes()+2<=numRooms && room->get_num_arcs()+2<=maxRooms;
}

void Graph::printMap()
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