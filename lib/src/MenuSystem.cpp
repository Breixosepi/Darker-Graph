#include "MenuSystem.hpp"
#include <iostream>
#include <Player.hpp>
#include <Enemy.hpp>

MenuSystem::MenuSystem()
{
    head = nullptr;
    tail = nullptr;
    current = nullptr;
}

MenuSystem::MenuSystem(SDL_Renderer* render, TTF_Font* fonts, SDL_Window* wind)
{
    renderer = render;
    font = fonts;
    window = wind;
    head = nullptr;
    tail = nullptr;
    current = nullptr;
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

void MenuSystem::render() const 
{
    if (!renderer) return;
    
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
        MyGraph creator;
        int width, height;
        SDL_GetWindowSize(window,&width,&height);
        Level level(creator.createMap(false));
        level.setWindowSize(width,height);

        level.setBackground("assets/screenshots/perg.png",renderer);
        level.setSourceBackground(55,80,260,195);
        level.setTileSet("assets/screenshots/tileSet.png",renderer);
        level.insertSourceShape(832,208,32,32,0);
        level.insertSourceShape(486,202,80,80,1);
        level.insertSourceShape(832,416,64,64,2);
        
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
                        SDL_GetWindowSize(window, &width, &height);
                        level.setWindowSize(width,height);
                    } 
                }  
            }
            level.drawMap(renderer);    
        }
    }); 

menu->addWidget("load", "Puntuaciones", [&]() 
{
    MyGraph creator;
    int width, height;

    // room setup
    SDL_GetWindowSize(window,&width,&height);
    Level level(creator.createMap(false));
    std::pair<double,double> borderRoom = level.setWindowSize(width,height);
    
    level.setBackground("assets/screenshots/perg.png",renderer);
    level.setSourceBackground(55,80,260,195);
    level.setTileSet("assets/screenshots/tileSet.png",renderer);
    level.insertSourceShape(832,208,32,32,0);
    level.insertSourceShape(486,202,80,80,1);
    level.insertSourceShape(832,416,64,64,2);
    level.insertSourceShape(304,400,64,48,3);
    level.insertSourceShape(304,400,64,48,4);
    level.insertSourceShape(640,112,16,80,5);
    level.insertSourceShape(400,16,80,89,6);
    level.insertSourceShape(72,52,176,63,7);
    level.insertSourceShape(400,112,16,80,8);
    level.insertSourceShape(432,112,16,74,9);
    level.printMapConsole();

    // Player setup
    SDL_Surface* playerSurface = IMG_Load("assets/sprites/dwarf.png");
    TexturePtr playerTexture(SDL_CreateTextureFromSurface(renderer, playerSurface));
    SDL_FreeSurface(playerSurface);
    Player player;
    player.initAnimation(renderer, playerTexture);
    player.setPosition(400, 300); 

    // Enemy setup 
    SDL_Surface* enemySurface = IMG_Load("assets/sprites/Orc.png");
    TexturePtr enemyTexture(SDL_CreateTextureFromSurface(renderer, enemySurface));
    SDL_FreeSurface(enemySurface);
    std::vector<Enemy> enemies;
    Enemy enemy;
    enemy.initAnimation(renderer, enemyTexture);
    enemy.setPosition(450, 200);

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
                    SDL_GetWindowSize(window, &width, &height);
                    level.setWindowSize(width,height);
                } 
            }  
        }

        // Actualización de objetos
        player.update(deltaTime,borderRoom,width,height);
        player.attack(enemy);
        enemy.detectPlayer(player.getBounds());
        enemy.attack(player);
        enemy.update(deltaTime);

        // Renderizado
        SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255); 
        SDL_RenderClear(renderer);
        level.drawRoom(renderer);
        player.renderPlayer(renderer);
        // player.renderAttackHitbox(renderer);
        // player.renderDebugBounds(renderer);
        enemy.renderEnemy(renderer); 
        // enemy.renderAttackHitbox(renderer);
        // enemy.renderDebugBounds(renderer);
        level.drawRoomLastFrame(renderer);
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