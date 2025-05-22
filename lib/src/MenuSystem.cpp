#include "MenuSystem.hpp"
#include <iostream>
#include <Player.hpp>
#include <Enemy.hpp>

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
        while(!levels.empty()){levels.pop();}
        levels.push(creator.createMap(0));
        //levels.push(creator.createMap(0));
        //levels.push(creator.createMap(0));
        
        Level actualLevel = levels.front();
        levels.pop();
        
        SDL_GetWindowSize(window,&windowWidth,&windowHeight);
        helper.get()->handleWindowResize(windowWidth,windowHeight,actualLevel.getMatrixSize());
        actualLevel.setRenderHelper(helper);

        SDL_Event event;
        bool running = true;
        while (running) 
        {
            while (SDL_PollEvent(&event)) 
            {
                if (event.type == SDL_QUIT) 
                {
                    running = false;
                }
                else if (event.type == SDL_KEYDOWN) 
                {
                    switch (event.key.keysym.sym) 
                    {
                        case SDLK_ESCAPE: running = false; break;
                    }
                }
                else if(event.type == SDL_WINDOWEVENT)
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
                    {
                        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                        helper.get()->handleWindowResize(windowWidth,windowHeight,actualLevel.getMatrixSize());
                        actualLevel.handleResizeWindow();
                    } 
                }  
            }
            actualLevel.renderMap(renderer);    
        }
    }); 

menu->addWidget("load", "Puntuaciones", [&]() 
{
    // room setup
    while(!levels.empty()){levels.pop();}
    levels.push(creator.createMap(0));
    //levels.push(creator.createMap(0));
    //levels.push(creator.createMap(0));
        
    Level actualLevel = levels.front();
    levels.pop();
        
    SDL_GetWindowSize(window,&windowWidth,&windowHeight);
    helper.get()->handleWindowResize(windowWidth,windowHeight,actualLevel.getMatrixSize());
    actualLevel.setRenderHelper(helper);

    // Player setup
    SDL_Surface* playerSurface = IMG_Load("assets/sprites/dwarf.png");
    TexturePtr playerTexture(SDL_CreateTextureFromSurface(renderer, playerSurface));
    SDL_FreeSurface(playerSurface);
    Player player;
    player.initAnimation(renderer, playerTexture);
    player.setPosition(400, 300); 
    player.setRenderHelper(helper);

    bool running = true;
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
                SDL_PushEvent(&event); 
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) 
            {
                running = false; 
            }
            else if(event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
                {
                    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                    helper.get()->handleWindowResize(windowWidth,windowHeight,actualLevel.getMatrixSize());
                    actualLevel.handleResizeWindow();
                } 
            }  
        }

        // Actualización de objetos
        player.update(deltaTime);

        if(player.getIsInBound()&&player.getState()==State::RUNNING)
        {
            int index = *actualLevel.getCurrentRoom()->getIndex();
            player.setPosition(actualLevel.verifyPassRoom(static_cast<int>(player.getDirection()),player.getBounds()));
            if(index!=*actualLevel.getCurrentRoom()->getIndex())
            {
                std::cout<<"Te moviste al cuarto Numero: "<<*actualLevel.getCurrentRoom()->getIndex()<<" Provienes del Cuarto Numero: "<<index<<std::endl;
            }
        }

        // Renderizado
        SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255); 
        SDL_RenderClear(renderer);
        actualLevel.renderRoom(renderer);
        player.renderPlayer(renderer);
        // player.renderAttackHitbox(renderer);
        // player.renderDebugBounds(renderer);
        actualLevel.renderRoomLastFrame(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); 
    }
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