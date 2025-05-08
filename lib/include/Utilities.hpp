#pragma once 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <vector>


struct SDL_Deleter 
{
    void operator()(SDL_Window* window) const 
    { 
        if (window) SDL_DestroyWindow(window); 
    }
    void operator()(SDL_Renderer* renderer) const 
    { 
        if (renderer) SDL_DestroyRenderer(renderer); 
    }
    void operator()(SDL_Texture* texture) const 
    { 
        if (texture) SDL_DestroyTexture(texture); 
    }
    void operator()(TTF_Font* font) const 
    { 
        if (font) TTF_CloseFont(font); 
    }
    void operator()(SDL_Surface* surface) const 
    { 
        if (surface) SDL_FreeSurface(surface); 
    }
};

using WindowPtr = std::unique_ptr<SDL_Window, SDL_Deleter>;
using RendererPtr = std::unique_ptr<SDL_Renderer, SDL_Deleter>;
using TexturePtr = std::unique_ptr<SDL_Texture, SDL_Deleter>;
using FontPtr = std::unique_ptr<TTF_Font, SDL_Deleter>;
using SurfacePtr = std::unique_ptr<SDL_Surface, SDL_Deleter>;
