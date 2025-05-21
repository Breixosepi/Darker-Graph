#include <Game.hpp>

Game::Game() : isRunning(false)
{

}
Game::~Game()
{

}

void Game::initialize(const char* title,int x_pos,int y_pos, int width, int height, bool fullscreen)
{
    Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        TTF_Init();
        window.reset(SDL_CreateWindow(title, x_pos, y_pos, width, height, flags  ));
        if (!window)
        {
            isRunning = false;
            throw std::runtime_error("No se pudo crear la ventana SDL");
            return;
        }
        
        renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
        if (!renderer)
        {
            isRunning = false;
            throw std::runtime_error("No se pudo crear el render SDL");
            return;
        }
        font.reset(TTF_OpenFont("assets/fonts/VT323.ttf", 60));
        if (!font)
        {
            isRunning = false;
            throw std::runtime_error("No se pudo cargar la fuente: " + std::string(TTF_GetError()));
            return;
        }
        TTF_SetFontStyle(font.get(), TTF_STYLE_NORMAL);
        TTF_SetFontOutline(font.get(), 0);  
        TTF_SetFontHinting(font.get(), TTF_HINTING_LIGHT);

        mainMenu = std::make_unique<MenuSystem>(renderer.get(), font.get(), window.get());

        SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }

    if((IMG_Init({IMG_INIT_PNG|IMG_INIT_JPG}))&IMG_INIT_PNG){}
    else
    {
        isRunning = false;
    }
}
void Game::handleEvents() 
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        switch (event.type) 
        {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
                {
                    //int newWidth = std::max(event.window.data1, 800);
                    //int newHeight = std::max(event.window.data2, 800);
                    
                    if (event.window.data1 > 800 && event.window.data2 > 800) 
                    {
                        SDL_SetWindowSize(window.get(), event.window.data1, event.window.data2);
                    }
                }
                break;
            default:
                if (mainMenu) mainMenu->handleEvent(event);
                break;
        }
    }
}
void Game::update()
{
}
void Game::render()
{
    SDL_RenderClear(renderer.get());
    
    if (mainMenu) 
    {
        mainMenu->render();
    }
    
    SDL_RenderPresent(renderer.get());
    
}
void Game::cleanup()
{
    std::cout<<"Start Cleanup"<<std::endl;
    mainMenu.reset();
    font.reset();
    renderer.reset();
    window.reset();
    std::cout<<"Pointers Clean"<<std::endl;
    IMG_Quit();
    std::cout<<"IMG Clean"<<std::endl;
    TTF_Quit();
    std::cout<<"TTF Clean"<<std::endl;
    SDL_Quit();
    std::cout<<"SDL Clean"<<std::endl;
}

bool Game::running()
{
    return isRunning;

}