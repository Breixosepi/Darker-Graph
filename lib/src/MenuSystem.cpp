#include "MenuSystem.hpp"
#include <iostream>

MenuSystem::MenuSystem(const RendererPtr& render, const FontPtr& fonts) : renderer(render.get()), font(fonts.get()),head(nullptr),tail(nullptr),current(nullptr)
{

}

void MenuSystem::addWidget(const std::string& name, const std::string& label, std::function<void()> action) 
{
    const int startY = 200;    
    const int stepY = 50;      
    const int centerX = 400;  

    std::unique_ptr<Widget> widget = std::make_unique<Widget>(name, centerX, startY + 2*(widgets.size() * stepY), label);
    widget->setAction(action);

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

std::unique_ptr<MenuSystem> MenuSystem::createMainMenu(const RendererPtr& renderer, const FontPtr& font) 
{
    std::unique_ptr<MenuSystem> menu = std::make_unique<MenuSystem>(renderer, font);

    menu->addWidget("start", "Iniciar Juego", [&]() 
    {
        MyGraph creator;
        Level firstLevel(creator.createMap(false));
        SDL_RenderClear(renderer.get());
        auto lol = IMG_Load("assets/screenshots/tileSet.png");
        SDL_Texture* god;
        if(lol)
        {
            god = SDL_CreateTextureFromSurface(renderer.get(),lol);
        }
        
        int dimensiones = -1;
        auto pos = firstLevel.shapesToDrawMap(800,800,dimensiones);
        SDL_Rect origen;
        SDL_Rect destino;
        for(auto xd : pos)
        {
            if(xd.second==0)
            {
                origen = {832,207,32,32};
                destino.h = destino.w = dimensiones;
            }
            else if(xd.second==1)
            {
                origen = {486,202,80,80};
                destino.h = destino.w = dimensiones;
            }
            else if(xd.second==2)
            {
                origen = {832,416,64,64};
                destino.h = dimensiones/2;
                destino.w = dimensiones;
            }
            else if(xd.second==3)
            {
                origen = {832,416,64,64};
                destino.h = dimensiones;
                destino.w = dimensiones/2;
            }
            destino.x = xd.first.first;
            destino.y = xd.first.second;
            SDL_RenderCopy(renderer.get(),god,&origen,&destino);
        }
        SDL_RenderPresent(renderer.get());
    });

    /*menu->addWidget("start", "Iniciar Juego", []() 
    {

    });*/

    menu->addWidget("load", "Cargar Partida", []() 
    {
        std::cout << "Partida cargada!\n";
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
    
    return menu;
}