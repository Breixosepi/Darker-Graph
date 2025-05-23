#include "MenuSystem.hpp"
#include <iostream>
#include <Player.hpp>

MenuSystem::MenuSystem()
{
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    helper = std::make_shared<RenderHelper>();
}

MenuSystem::MenuSystem(SDL_Renderer* render, TTF_Font* fonts, SDL_Window* wind)
{
    renderer = render;
    font = fonts;
    window = wind;
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    helper = std::make_shared<RenderHelper>();
    setMainMenu(this);
}

void MenuSystem::addWidget(const std::string& name, const std::string& label, std::function<void()> action) 
{
    const int startY = 150;    
    const int stepY = 50;      
    const int centerX = 400;  

    std::unique_ptr<Widget> widget = std::make_unique<Widget>(name, centerX, startY + 2*(widgets.size() * stepY), label);
    widget->setAction(action);

    SDL_Rect rect;
    rect.x = centerX - (label.length() * 10); 
    rect.y = startY + 2*(widgets.size() * stepY) - 25;
    rect.w = label.length() * 20;
    rect.h = 50;
    widget->setRect(rect);

    if (!head) 
    {
        head = widget.get();
        current = head;
    } 
    else 
    {
        tail->setNext(widget.get());
        widget->setPrev(tail);
    }

    tail = widget.get();
    widgets.push_back(std::move(widget));

    if (widgets.size() > 1) 
    {
        tail->setNext(head);
        head->setPrev(tail);
    }
}

void MenuSystem::handleEvent(const SDL_Event& event) 
{
    if (event.type == SDL_KEYDOWN) 
    {
        switch (event.key.keysym.sym) 
        {
            case SDLK_UP: navigateUp(); break;
            case SDLK_DOWN: navigateDown(); break;
            case SDLK_RETURN: executeCurrent(); break;
            case SDLK_f:
            {
                static bool isFullscreen = false;
                isFullscreen = !isFullscreen;
                SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
                if (window) 
                {
                    SDL_SetWindowFullscreen(window, isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                    if (!isFullscreen) 
                    {
                        SDL_SetWindowSize(window, 800, 800); 
                    }
                }
                break;
            }
        }
    }
    if (event.type == SDL_MOUSEMOTION) 
    {
        for (auto& widget : widgets) 
        {
            if (widget->containsPoint(event.motion.x, event.motion.y)) 
            {
                current = widget.get();
                break;
            }
        }
    }
    
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) 
    {
        for (auto& widget : widgets) 
        {
            if (widget->containsPoint(event.button.x, event.button.y)) 
            {
                current = widget.get();
                executeCurrent();
                break;
            }
        }
    }
}

void MenuSystem::render()  
{
    if (!renderer) return;
    SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/background-title.png",renderer),NULL,NULL);
    if (font && !widgets.empty()) 
    {
        for (const auto& widget : widgets) 
        {
            widget->render(renderer, font, widget.get() == current);
        }
    }     
}
void MenuSystem::navigateUp() 
{
    if (!current) 
    {
        return; 
    }

    current = current->getPrev() ? current->getPrev() : tail;
}

void MenuSystem::navigateDown() 
{
    if (!current) 
    {
        return; 
    }

    current = current->getNext() ? current->getNext() : head;
}

void MenuSystem::executeCurrent() 
{
    if (current) 
    {
        current->execute();
    }
}

void MenuSystem::setMainMenu(MenuSystem* menu) 
{ 
    menu->addWidget("start", "Iniciar Juego", [&]()
    {   
        creator.restart();
        bool gameOver = false;
        while(!gameOver)
        {
      // room setup
        Level level = Level(creator.createMap(false));
        
        SDL_GetWindowSize(window,&windowWidth,&windowHeight);
        helper.get()->handleWindowResize(windowWidth,windowHeight,level.getMatrixSize());
        level.setRenderHelper(helper);

      // Player setup
        SDL_Surface* playerSurface = IMG_Load("assets/sprites/dwarf.png");
        TexturePtr playerTexture(SDL_CreateTextureFromSurface(renderer, playerSurface));
        SDL_FreeSurface(playerSurface);
        Player player;
        player.initAnimation(renderer, playerTexture);
        player.setPosition(400, 300); 
        player.setRenderHelper(helper);

      // Enemy setup 
        EnemyManager enemies;
        enemies.init(renderer, "assets/sprites/Orc.png");
        auto* dungeonGraph = level.getMap();
        if (dungeonGraph) 
        {
        dungeonGraph->enum_for_each_node([&](Designar::nat_t index, auto* node) 
        {
            if (node) 
            {
                int roomIndex = index;
                
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> countDist(1, 3);
                int enemyCount = countDist(gen);
                
                for (int i = 0; i < enemyCount; ++i) 
                {
                    std::uniform_int_distribution<> xDist(100, 400);
                    std::uniform_int_distribution<> yDist(100, 300);
                    enemies.addEnemy(roomIndex, xDist(gen), yDist(gen));
                }
            }
            return true;
        });
    }

        bool running = true;
        bool showMap = false;
        SDL_Event event;
        Uint32 lastTime = SDL_GetTicks();

        while (running) 
        {
            Uint32 currentTime = SDL_GetTicks();
            float deltaTime = (currentTime - lastTime) / 1000.0f; 
            lastTime = currentTime;

            while (SDL_PollEvent(&event)) 
            {
                player.handleImput(event);
                if (event.type == SDL_QUIT) 
                {
                    running = false;
                    gameOver = true;
                    SDL_PushEvent(&event); 
                }
                else if (event.type == SDL_KEYDOWN) 
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        if(showMap){showMap = false;}
                        else
                        {
                            running = false;
                            gameOver = true;
                        } 
                        break;
                    case SDLK_m: //Con la tecla 'm' se muestra el mapa de la dungeon
                        if(showMap){showMap = false;}
                        else{showMap = true;}
                        break;
                    }  
                }
                else if(event.type == SDL_WINDOWEVENT)
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
                    {
                        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                        helper.get()->handleWindowResize(windowWidth,windowHeight,level.getMatrixSize());
                        level.handleResizeWindow();
                    } 
                }  
            }
            if(!showMap)
            {
                int currentRoomIndex = *level.getCurrentRoom()->getIndex();
                enemies.setCurrentRoom(currentRoomIndex);
                // Actualización de objetos
                player.update(deltaTime);
                enemies.update(deltaTime, player);
                enemies.handlePlayerAttack(player);

                //Comprueba si despues de moverse se encuentra en los limites del cuarto actual
                if(player.getIsInBound()&&player.getState()==State::RUNNING)
                {
                    int index = *level.getCurrentRoom()->getIndex();
                    player.setPosition(level.verifyPassRoom(static_cast<int>(player.getDirection()),player.getBounds()));
                    if(index!=*level.getCurrentRoom()->getIndex())
                    {
                        std::cout<<"you advanced to room number: "<<*level.getCurrentRoom()->getIndex()<<" you come from room number: "<<index<<std::endl;
                    }
                }
                //Si esta en la Ultima sala, comprueba que este sobre la escalera para avanzar de nivel
                else if(*level.getCurrentRoom()->getIndex()==level.getMap()->get_num_nodes())
                {
                    int middleX = helper.get()->getMiddlePointInX();
                    int middleY = helper.get()->getMiddlePointInY();
                    SDL_Rect dest = player.getBounds();
                    if(std::abs(middleX-(dest.x+dest.w/2))<=dest.w/2&&std::abs(middleY-(dest.y+dest.h))<=dest.h/4)
                    {
                        bool result = level.IsEulerianPath(); //retorna true si se hizo correctamente un camino euleriano
                        std::cout<<"Loading New Level!"<<std::endl;
                        running = false;
                        creator.levelUp();
                        SDL_Delay(650);
                    }
                }
                // Renderizado
                SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255); 
                SDL_RenderClear(renderer);
                level.renderRoom(renderer,deltaTime);
                player.renderPlayer(renderer);
                enemies.render();
                level.renderRoomLastFrame(renderer,deltaTime);
                SDL_RenderPresent(renderer);
                SDL_Delay(16); 
            }
            else{level.renderMap(renderer);}
        }
        }     
    }); 

    menu->addWidget("load", "Puntuaciones", []() 
    {
    
    });

    
    menu->addWidget("options", "Opciones", []() 
    {
        std::cout << "Opciones seleccionadas\n";
    });

    menu->addWidget("credits", "Creditos", []() 
    {
        std::cout << "todo: Eugenio El musculoso!\n" << "Nada : Francisco el negro\n" ;
    });
    
    menu->addWidget("exit", "Salir", []() 
    {
        SDL_Event quitEvent{SDL_QUIT};
        SDL_PushEvent(&quitEvent);
    });
} 