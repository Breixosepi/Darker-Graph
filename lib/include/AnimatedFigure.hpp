#pragma once
#include <Utilities.hpp>
#include <iostream>

class AnimatedFigure
{
    public:
        AnimatedFigure();
        ~AnimatedFigure();
        void initialize(SDL_Renderer* renderer);
        void renderPortal(const SDL_Rect& dest, SDL_Renderer* renderer);
        void renderParticles(const SDL_Rect& dest, SDL_Renderer* renderer);
        void renderCircularPortal(const SDL_Rect& dest, SDL_Renderer* renderer);

    private:
        SDL_Rect sourcePortal;
        SDL_Texture* textPortal;
        SDL_Rect sourceParticles;
        SDL_Texture* textParticles;
        SDL_Rect sourceCircularPortal;
        SDL_Texture* textCircularPortal;
        SDL_Rect* updatePortal();
        SDL_Rect* updateParticles();
        SDL_Rect* updateCircularPortal();
        SDL_Texture* initializeTexture(const char* path, SDL_Renderer* renderer);
};