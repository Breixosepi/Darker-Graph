#include <MyGraph.hpp>

MyGraph::MyGraph()
{
    numRooms = 6;
    maxRooms = 4;
    probability = 50.0;
    even_odd = {0,0};
}

MyGraph::~MyGraph(){};

Designar::Graph<Room>MyGraph::createMap(const bool& custom)
{
    if(custom){getData();}
    if(numRooms>0)
    {
        if(map.get_num_nodes()>0){reset();}
        std::mt19937 random(std::random_device{}());
        std::uniform_real_distribution<double> distribution(0, 1.0);

        generateEntry(random,distribution);
        while(!queue.empty())
        {
            if(map.get_num_nodes()==numRooms){break;}
            Designar::Graph<Room>::Node* room = queue.front();
            queue.pop();
            generateEvenRooms(room,random,distribution);
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

void MyGraph::getData()
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

void MyGraph::insertRoom(Designar::Graph<Room>::Node*& room)
{
    Designar::Graph<Room>::Node* newRoom = map.insert_node(map.get_num_nodes()+1);
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

void MyGraph::generateEntry(std::mt19937 random, std::uniform_real_distribution<double> distribution)
{
    std::cout<<"Generando entrada..."<<std::endl;
    Designar::Graph<Room>::Node* room = map.insert_node(1);
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
    std::cout<<"Entrada Generada!"<<std::endl;
}

void MyGraph::generateEvenRooms(Designar::Graph<Room>::Node*& room, std::mt19937 random, std::uniform_real_distribution<double> distribution)
{
    std::cout<<"Convirtiendo Cuarto "<<"("<<room->get_info().getIndex()<<")"<<" en Grado par..."<<std::endl;
    while(room->get_num_arcs()<maxRooms)
    {
        if(room->get_num_arcs()%2!=0) //Grado Impar
        {
            insertRoom(room);
        }
        else //Grado Par
        {
            if(distribution(random)<=probability/100.0 && limitRoom(room))
            {
                insertRoom(room);
            }
            else{break;}
        }  
    }
    std::cout<<"("<<room->get_info().getIndex()<<")"<<" es par!"<<std::endl;
}

bool MyGraph::limitRoom(Designar::Graph<Room>::Node*& room)
{
    return map.get_num_nodes()+2<=numRooms && room->get_num_arcs()+2<=maxRooms;
}

void MyGraph::printMap()
{
    map.for_each_node([&](Designar::Graph<Room>::Node* room)
    {
        std::cout<<"Num of Room: "<<room->get_info().getIndex();
        for(auto arc : map.adjacent_arcs(room))
        {
            if(arc->get_src_node()->get_info().getIndex()!=room->get_info().getIndex())
            {
                std::cout<<" "<<arc->get_tgt_node()->get_info().getIndex()<<" -> "<<arc->get_src_node()->get_info().getIndex();
            }
            else
            {
                std::cout<<" "<<arc->get_src_node()->get_info().getIndex()<<" -> "<<arc->get_tgt_node()->get_info().getIndex();
            }
        }
        if(room->get_num_arcs()%2==0){std::cout<<"  Grado Par";}
        else{std::cout<<"  Grado Impar";}
        std::cout<<std::endl;
    });
}

void MyGraph::reset()
{
    std::cout<<"Limpiando..."<<std::endl;
    map.clear();
    roomsReference.clear();
    while(!queue.empty()){queue.pop();}
    even_odd.first = 0;
    even_odd.second = 0;
}