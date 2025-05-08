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
    
    menu->addWidget("start", "Iniciar Juego", []() 
    {
        std::cout << "Juego iniciado!\n";
    });

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