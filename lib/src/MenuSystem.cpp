#include "MenuSystem.hpp"
#include <iostream>

MenuSystem::MenuSystem(const RendererPtr& render, const FontPtr& fonts, const WindowPtr& wind) : renderer(render.get()), font(fonts.get()),head(nullptr),tail(nullptr),current(nullptr),window(wind.get())
{

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

std::unique_ptr<MenuSystem> MenuSystem::createMainMenu(const RendererPtr& renderer, const FontPtr& font, const WindowPtr& wind) 
{
    std::unique_ptr<MenuSystem> menu = std::make_unique<MenuSystem>(renderer, font, wind);

    menu->addWidget("start", "Iniciar Juego", [&]()
    {
        int width, height;
        SDL_GetWindowSize(wind.get(),&width,&height);
        SDL_SetRenderDrawColor(renderer.get(), 20, 20, 20, 255);
        MyGraph creator;
        Level level(creator.createMap(false));
        level.setShapesMap(width,height);
        SDL_Rect originTileSet;
        SDL_Rect destTileSet;
        SDL_Rect originBackground = {55,80,260,195};
        SDL_Surface* surfTileSet = IMG_Load("assets/screenshots/tileSet.png");
        SDL_Texture* textTileSet = SDL_CreateTextureFromSurface(renderer.get(),surfTileSet);
        SDL_Surface* surfBackground = IMG_Load("assets/screenshots/perg.png");
        SDL_Texture* textBackground = SDL_CreateTextureFromSurface(renderer.get(),surfBackground);
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
                        SDL_GetWindowSize(wind.get(), &width, &height);
                        level.setShapesMap(width,height);
                    } 
                }  
            }
            SDL_RenderClear(renderer.get());
            SDL_RenderCopy(renderer.get(),textBackground,&originBackground,NULL);
            for(Shape shape : *level.getShapesMap())
            {
                if(std::get<4>(shape)==0){originTileSet = {832,208,32,32};}
                else if(std::get<4>(shape)==1){originTileSet = {486,202,80,80};}
                else if(std::get<4>(shape)==2){originTileSet = {832,416,64,64};}
                destTileSet.x = std::get<0>(shape);
                destTileSet.y = std::get<1>(shape);
                destTileSet.w = std::get<2>(shape);
                destTileSet.h = std::get<3>(shape);
                SDL_RenderCopy(renderer.get(),textTileSet,&originTileSet,&destTileSet);
            }
            SDL_RenderPresent(renderer.get());
        }
    });

    static SDL_Rect square = {375, 375, 50, 50}; 

    menu->addWidget("load", "Cargar Partida", [&]() 
    {
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
                        case SDLK_UP: square.y -= 10; break;
                        case SDLK_DOWN: square.y += 10; break;
                        case SDLK_LEFT: square.x -= 10; break;
                        case SDLK_RIGHT: square.x += 10; break;
                        case SDLK_ESCAPE: running = false; break;
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
            SDL_RenderClear(renderer.get());

            SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255); 
            SDL_RenderFillRect(renderer.get(), &square);

            SDL_RenderPresent(renderer.get());
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
    
    return menu;
}