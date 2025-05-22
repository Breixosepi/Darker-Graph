#include <MyGraph.hpp>

MyGraph::MyGraph()
{
    numRooms = 6;
    maxGrade = 4;
    probability = 75.0;
    evenOdd = {numRooms,0};
}

MyGraph::~MyGraph(){};

//Devuelve una tupla con: el mapa (Grafo conexo, dirigido y con un camino euleriano como minimo), 
//un vector con referencia a los nodos y una matriz con las posiciones de las salas y portales.
Dungeon MyGraph::createMap(const bool& custom)
{
    if(map.get_num_nodes()>0){reset();}
    if(numRooms>0)
    {
        while(!verifyCreateMap(custom))
        {
            std::cout<<"eulerian path NOT exist."<<std::endl;
            reset();
        }
        std::cout<<"eulerian path exist!"<<std::endl;
        std::cout<<"Num of Rooms: "<<map.get_num_nodes()<<std::endl;
        reduceMatrix();
    }
    ++numRooms;
    return std::make_tuple(map,roomsReference,matrix);
}

bool MyGraph::verifyCreateMap(const bool& custom)
{
    if(custom){getData();}

    std::mt19937 random(std::random_device{}());
    std::uniform_int_distribution<int> chooseSide(0, 3); // 0:Izquierda, 1:Arriba, 2:Derecha, 3:Abajo
    std::uniform_real_distribution<double> probabilityAdd(0, 1.0);

    matrix.resize(numRooms*2 - 1); //num filas
    for (int i = 0; i < matrix.size(); ++i) //num columnas
    {
        matrix[i].resize(numRooms*2 - 1, 0);
    }

    generateEntry(random,probabilityAdd,chooseSide);
    while(!queue.empty())
    {
        if(map.get_num_nodes()==numRooms){break;}
        Designar::Graph<Room>::Node* room = queue.front();
        queue.pop();
        generateEvenRooms(room,random,probabilityAdd,chooseSide);
    }

    return fixMap(random,chooseSide);
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
    std::pair<int,int> pos = *room->get_info().getPos();
    if(verifyInsert(side,pos))
    { 
        Designar::Graph<Room>::Node* neighbor = helperInsert(pos);
        queue.push(neighbor);
        insertPath(room,neighbor,side);
    }
    else if(!verifyPath(side,pos))
    {
        std::cout<<"Error al Insertar Room Numero: "<<*room->get_info().getIndex()+1<<" en Room numero: "<<*room->get_info().getIndex()<<std::endl;
    }
}

Designar::Graph<Room>::Node* MyGraph::helperInsert(const std::pair<int,int>& pos)
{
    Designar::Graph<Room>::Node* room = map.insert_node(map.get_num_nodes()+1);
    roomsReference.push_back(room);
    room->get_info().setPos(pos);
    matrix[pos.first][pos.second] = *room->get_info().getIndex();
    return room;
}

void MyGraph::countEvenOdd(Designar::Graph<Room>::Node*& room)
{
    if(room->get_num_arcs()%2==0)
    {
        ++evenOdd.first;
        --evenOdd.second;
    }
    else
    {
        --evenOdd.first;
        ++evenOdd.second;
    }
}

bool MyGraph::verifyInsert(int& side, std::pair<int,int>& pos)
{
    std::pair<int,int> aux = pos;
    for(int i=0; i<4; ++i)
    {
        switch (side) 
        {
            case 0: //Izquierda
                --pos.second;
            break;
            case 1: //Arriba
                --pos.first;
            break;
            case 2: //Derecha
                ++pos.second;
            break;
            case 3: //Abajo
                ++pos.first;
            break;
        }
        if(matrix[pos.first][pos.second]==0){return true;}
        pos = aux;
        ++side;
        if(side==4){side -= 4;} //Revisa las posiciones disponibles en sentido de las agujas del reloj
    }
    return false;
}

void MyGraph::insertPath(Designar::Graph<Room>::Node*& room, Designar::Graph<Room>::Node*& neighbor, int side)
{
    map.insert_arc(room,neighbor);
    room->get_info().setPath(side);
    side += 2;
    if(side>3){side -= 4;}
    neighbor->get_info().setPath(side);
    countEvenOdd(room);
    countEvenOdd(neighbor);
}

bool MyGraph::verifyPath(int& side, std::pair<int,int>& pos)
{
    Designar::Graph<Room>::Node* room = roomsReference[(matrix[pos.first][pos.second])-1];
    for(int i=0; i<4; ++i)
    {
        if(!(*(room->get_info()).getPaths())[side])
        {
            switch (side)
            {
                case 0: //Izquierda
                    --pos.second;
                break;
                case 1: //Arriba
                    --pos.first;
                break;
                case 2: //Derecha
                    ++pos.second;
                break;
                case 3: //Abajo
                    ++pos.first;
                break;
            }
            if(matrix[pos.first][pos.second]>1)
            {
                Designar::Graph<Room>::Node* neighbor = roomsReference[(matrix[pos.first][pos.second])-1];
                insertPath(room,neighbor,side);
                return true;
            }
        }
        ++side;
        if(side==4){side -= 4;}
    }
    return false;
}

void MyGraph::generateEntry(std::mt19937 random, std::uniform_real_distribution<double> add, std::uniform_int_distribution<int> side)
{
    Designar::Graph<Room>::Node* room = helperInsert({numRooms-1,numRooms-1});
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
    neighborsEntry.insert(1);
    for(auto arc : map.adjacent_arcs(room))
    {
        neighborsEntry.insert(*arc->get_tgt_node()->get_info().getIndex());
    }
}

void MyGraph::generateEvenRooms(Designar::Graph<Room>::Node*& room, std::mt19937 random, std::uniform_real_distribution<double> add, std::uniform_int_distribution<int> side)
{
    while(room->get_num_arcs()<maxGrade)
    {
        if(room->get_num_arcs()%2!=0) //Grado Impar
        { 
            insertRoom(room,side(random)); //Asegura que todos los Cuartos sean pares (excepto la entrada y la salida)
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
}

bool MyGraph::limitRoom(Designar::Graph<Room>::Node*& room)
{
    if(map.get_num_nodes()+2<=numRooms && room->get_num_arcs()+2<=maxGrade)
    {
        //Evita bugs sobre que algun cuarto se quiera generar y conectar con la entrada
        if(!neighborsEntry.count(*room->get_info().getIndex()))
        {
            std::pair<int,int> pos = *room->get_info().getPos();
            int row = pos.first;
            int column = pos.second;
            //numRooms-1 es la posicion de la entrada en la matriz, tanto en X como en Y
            if(std::abs((numRooms-1)-row)==1||std::abs((numRooms-1)-column)==1)
            {
                if(matrix[row][column-1]==1){return false;} //Izquierda
                if(matrix[row-1][column]==1){return false;} //Arriba
                if(matrix[row][column+1]==1){return false;} //Derecha
                if(matrix[row+1][column]==1){return false;} //Abajo
            }
        }
        return true;
    }
    return false;
}

//Termina de conectar los cuartos impares para que entonces el grafo cumpla con las condiciones necesarias
bool MyGraph::fixMap(std::mt19937 random, std::uniform_int_distribution<int> side)
{
    if(evenOdd.second!=2) //Si no hay 2 impares se procede a intentar corregir
    {
        if(!queue.empty())
        {
            int aux = 0;
            while(evenOdd.second>2)
            {
                int start = (*queue.front()->get_info().getIndex()-1)+aux/2;
                Designar::Graph<Room>::Node* roomA = roomsReference[start];
                std::pair<int,int> posA = *roomA->get_info().getPos();
                int sideA = side(random);
                while(!verifyInsert(sideA,posA)||roomA->get_num_arcs()%2==0)
                {
                    ++start;
                    roomA = roomsReference[start];
                    posA = *roomA->get_info().getPos();
                    sideA = side(random);
                }
                int end = (numRooms-2)-aux/2;
                Designar::Graph<Room>::Node* roomB = roomsReference[end];
                std::pair<int,int> posB = *roomB->get_info().getPos();
                int sideB = side(random);
                while(!verifyInsert(sideB,posB)||roomB->get_num_arcs()%2==0)
                {
                    --end;
                    roomB = roomsReference[end];
                    posB = *roomB->get_info().getPos();
                    sideB = side(random);
                }
                map.insert_arc(roomA, roomB);
                matrix[posA.first][posA.second] = -(*roomB->get_info().getIndex());
                roomA->get_info().setPath(sideA);
                matrix[posB.first][posB.second] = -(*roomA->get_info().getIndex());
                roomB->get_info().setPath(sideB);
                aux += 2;
                evenOdd.second -= 2;
            }
            std::cout<<"Correction Made. Finish map Creation!."<<std::endl;
            return searchOdds()==2;
        }
        return false;
    }
    std::cout<<"Finish map Creation!."<<std::endl;
    return true;
}

//De la matriz cuenta las filas y columnas donde hayan salas.
std::pair<int,int> MyGraph::getRowsColumns()
{
    std::pair<int,int> pair = {0,0};
    for(int i=0; i<matrix.size(); ++i)
    {
        std::pair<int,int> memory = pair;
        for(int j=0; j<matrix[0].size(); ++j)
        {
            if(matrix[i][j]!=0&&memory.first==pair.first)
            {
                ++pair.first;
            }
            if(matrix[j][i]!=0&&memory.second==pair.second)
            {
                ++pair.second;
            }
            if(memory.first!=pair.first&&memory.second!=pair.second){break;}
        }
    }
    return pair;
}

//Toma la matriz y elimina todas las filas y columnas donde no haya ningun cuarto en ellas
void MyGraph::reduceMatrix()
{
    std::vector<std::vector<int>> rowsWithRooms;
    bool firstRow = false;
    int supRowsEliminated = 0;
    for(int i=0; i<matrix.size(); ++i)
    {
        for(int j=0; j<matrix[0].size(); ++j)
        {
            if(matrix[i][j]!=0)
            {
                rowsWithRooms.push_back(matrix[i]);
                if(!firstRow)
                {
                    firstRow = true;
                    supRowsEliminated = i;
                }
                break;
            }
        }
    }
    std::pair<int,int> rowsColumns = getRowsColumns();
    std::vector<std::vector<int>> reduced(rowsColumns.first,std::vector<int>(rowsColumns.second,0));
    int k=0;
    bool firstColumn = false;
    int leftColumnsEliminated = 0;
    for(int i=0; i<rowsWithRooms[0].size()||k<rowsColumns.second; ++i) //Columnas a Recorrer
    {
        bool found = false;
        for(int j=0; j<rowsWithRooms.size(); ++j) //Filas a Recorrer
        {
            if(rowsWithRooms[j][i]!=0)
            {
                reduced[j][k]=rowsWithRooms[j][i];
                found = true;
                if(!firstColumn)
                {
                    firstColumn = true;
                    leftColumnsEliminated = i;
                }
            }
        }
        if(found){++k;}
    }
    matrix = reduced;
    for(auto node : roomsReference)
    {
        node->get_info().setPos({(*node).get_info().getPos()->first-supRowsEliminated,(*node).get_info().getPos()->second-leftColumnsEliminated});
    }
}

//Cuenta los nodos impares, se detiene cuando ha contado mas de 2
int MyGraph::searchOdds()
{
    int result = 0;
    for(auto it=map.nodes_begin();it!=map.nodes_end();++it)
    {
        if((*it)->get_num_arcs()%2!=0){++result;}
        if(result>2){return result;}
    }
    return result;
}

void MyGraph::printLastGraph()
{
    map.for_each_node([&](Designar::Graph<Room>::Node* room)
    {
        std::cout<<"Num of Room: "<<*room->get_info().getIndex();
        for(auto arc : map.adjacent_arcs(room))
        {
            if(arc->get_src_node()->get_info().getIndex()!=room->get_info().getIndex())
            {
                std::cout<<" "<<*arc->get_tgt_node()->get_info().getIndex()<<" -> "<<*arc->get_src_node()->get_info().getIndex();
            }
            else
            {
                std::cout<<" "<<*arc->get_src_node()->get_info().getIndex()<<" -> "<<*arc->get_tgt_node()->get_info().getIndex();
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
    neighborsEntry.clear();
    roomsReference.clear();
    while(!queue.empty()){queue.pop();}
    evenOdd = {numRooms,0};
    matrix.clear();
}