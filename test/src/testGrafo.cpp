#include <SDL2/SDL.h>
#include <memory>    
#include <iostream>  

constexpr int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct SDL_Deleter 
{
    void operator()(SDL_Window* window) const { SDL_DestroyWindow(window); }
    void operator()(SDL_Renderer* renderer) const { SDL_DestroyRenderer(renderer); }
};

using WindowPtr = std::unique_ptr<SDL_Window, SDL_Deleter>;
using RendererPtr = std::unique_ptr<SDL_Renderer, SDL_Deleter>;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << '\n';
        return 1;
    }

    WindowPtr window(
        SDL_CreateWindow(
            "Interfaz SDL2 + C++17",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
        )
    );

    if (!window) {
        std::cerr << "Error al crear ventana: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    RendererPtr renderer(
        SDL_CreateRenderer(
            window.get(),
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        )
    );

    if (!renderer) {
        std::cerr << "Error al crear renderer: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Rect rect{ 
        SCREEN_WIDTH / 2 - 50, 
        SCREEN_HEIGHT / 2 - 50, 
        100, 
        100 
    };

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    if (auto key = event.key.keysym.sym; key == SDLK_w) rect.y -= 10;
                    else if (key == SDLK_a) rect.x -= 10;
                    else if (key == SDLK_s) rect.y += 10;
                    else if (key == SDLK_d) rect.x += 10;
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
        SDL_RenderClear(renderer.get());

        SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
        SDL_RenderFillRect(renderer.get(), &rect);

        SDL_RenderPresent(renderer.get());
    }

    SDL_Quit();
    return 0;
}