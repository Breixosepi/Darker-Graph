#include <MyGraph.hpp>

MyGraph::MyGraph()
{
    numRooms = 8;
    maxGrade = 4;
    probability = 50.0;
    even_odd = {0,0};
}

MyGraph::~MyGraph(){};

std::pair<Designar::Graph<Room>,std::vector<std::vector<int>>> MyGraph::createMap(const bool& custom)
{
    if(custom){getData();}
    if(numRooms>0)
    {
        if(map.get_num_nodes()>0){reset();}

        std::mt19937 random(std::random_device{}());
        std::uniform_int_distribution<int> chooseSide(1, 4); // 1:Izquierda, 2:Arriba, 3:Derecha, 4:Abajo
        std::uniform_real_distribution<double> probabilityAdd(0, 1.0);

        matrix.resize(numRooms*2 - 1); //num filas
        for (int i = 0; i < matrix.size(); ++i) //num columnas
        {
            matrix[i].resize(numRooms*2 - 1, -1);
        }

        generateEntry(random,probabilityAdd,chooseSide);
        while(!queue.empty())
        {
            if(map.get_num_nodes()==numRooms){break;}
            Designar::Graph<Room>::Node* room = queue.front();
            queue.pop();
            generateEvenRooms(room,random,probabilityAdd,chooseSide);
        }
        for(int i=0; i<matrix.size(); ++i)
        {
            for(int j=0; j<matrix[0].size(); ++j)
            {
                if(matrix[i][j]==-1){std::cout<<"*";}
                else{std::cout<<matrix[i][j];}
            }
            std::cout<<std::endl;
        }
        fixMap(random,chooseSide);
        for(int i=0; i<matrix.size(); ++i)
        {
            for(int j=0; j<matrix[0].size(); ++j)
            {
                if(matrix[i][j]==-1){std::cout<<"*";}
                else{std::cout<<matrix[i][j];}
            }
            std::cout<<std::endl;
        }
    }
    return {map,matrix};
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
    std::cin>>maxGrade;
    std::cout<<std::endl;
    if(maxGrade>numRooms){maxGrade=numRooms;}
    else if(maxGrade<0){maxGrade=0;}
}

void MyGraph::insertRoom(Designar::Graph<Room>::Node*& room, int side)
{
    std::pair<int,int> pos = room->get_info().getPos();
    if(verifyInsert(side,pos))
    {
        Designar::Graph<Room>::Node* newRoom = map.insert_node(map.get_num_nodes()+1);
        roomsReference.push_back(newRoom);
        newRoom->get_info().setPos(pos);
        matrix[pos.first][pos.second] = newRoom->get_info().getIndex();
        
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
}

bool MyGraph::verifyInsert(int side, std::pair<int,int>& pos)
{
    std::pair<int,int> aux = pos;
    for(int i=0; i<4; ++i)
    {
        switch (side)
        {
            case 1:
                --pos.second;
            break;
            case 2:
                --pos.first;
            break;
            case 3:
                ++pos.second;
            break;
            case 4:
                ++pos.first;
            break;
        }
        if(matrix[pos.first][pos.second]==-1){return true;}
        pos = aux;
        ++side;
        if(side==5){side = side-4;}
    }
    return false;
}

void MyGraph::generateEntry(std::mt19937 random, std::uniform_real_distribution<double> add, std::uniform_int_distribution<int> side)
{
    std::cout<<"Generando entrada..."<<std::endl;
    Designar::Graph<Room>::Node* room = map.insert_node(map.get_num_nodes()+1); //Sala 1
    roomsReference.push_back(room);
    room->get_info().setPos({numRooms-1,numRooms-1});
    matrix[room->get_info().getPos().first][room->get_info().getPos().second] = room->get_info().getIndex();
    if(maxGrade>0)
    {
        insertRoom(room,side(random));
        while (room->get_num_arcs()<maxGrade)
        {
            if(add(random)<=probability/100.0 && limitRoom(room))
            {
                insertRoom(room,side(random));
                insertRoom(room,side(random));
            }
            else{break;}
        }
    }
    std::cout<<"Entrada Generada!"<<std::endl;
}

void MyGraph::generateEvenRooms(Designar::Graph<Room>::Node*& room, std::mt19937 random, std::uniform_real_distribution<double> add, std::uniform_int_distribution<int> side)
{
    std::cout<<"Convirtiendo Cuarto "<<"("<<room->get_info().getIndex()<<")"<<" en Grado par..."<<std::endl;
    while(room->get_num_arcs()<maxGrade)
    {
        if(room->get_num_arcs()%2!=0) //Grado Impar
        {
            insertRoom(room,side(random));
        }
        else //Grado Par
        {
            if(add(random)<=probability/100.0 && limitRoom(room))
            {
                insertRoom(room,side(random));
            }
            else{break;}
        }  
    }
    std::cout<<"("<<room->get_info().getIndex()<<")"<<" es par!"<<std::endl;
}

bool MyGraph::limitRoom(Designar::Graph<Room>::Node*& room)
{
    return map.get_num_nodes()+2<=numRooms && room->get_num_arcs()+2<=maxGrade;
}

void MyGraph::fixMap(std::mt19937 random, std::uniform_int_distribution<int> side)
{
    printWrittenGraph();
    if(even_odd.second==2){std::cout<<"Hay un camino euleriano!"<<std::endl;}
    else
    {
        std::cout<<"Corrigiendo grafo..."<<std::endl;
        int aux = 0;
        if(!queue.empty())
        {
            while(aux<queue.size()-1)
            {
                Designar::Graph<Room>::Node* RoomA = roomsReference[numRooms-queue.size()+aux/2];
                Designar::Graph<Room>::Node* RoomB = roomsReference[numRooms-2-aux/2];
                std::pair<int,int> posA = RoomA->get_info().getPos();
                if(verifyInsert(side(random),posA))
                {
                    matrix[posA.first][posA.second] = RoomB->get_info().getIndex();
                }
                std::pair<int,int> posB = RoomB->get_info().getPos();
                if(verifyInsert(side(random),posB))
                {
                    matrix[posB.first][posB.second] = RoomA->get_info().getIndex();
                }
                map.insert_arc(RoomA, RoomB);
                aux = aux+2;
                even_odd.second = even_odd.second-2;
            }
        }
        if(even_odd.second==2)
        {
            printWrittenGraph();
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

void MyGraph::printWrittenGraph()
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
    even_odd = {0,0};
    matrix.clear();
}