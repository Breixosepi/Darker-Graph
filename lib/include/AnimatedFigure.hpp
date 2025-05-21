#pragma once
#include <Utilities.hpp>
#include <iostream>
#include <RenderHelper.hpp>

class AnimatedFigure
{
    public:
        AnimatedFigure();
        ~AnimatedFigure();
        void renderPortal(const SDL_Rect& dest, SDL_Renderer* renderer);
        void renderParticles(const SDL_Rect& dest, SDL_Renderer* renderer);
        void renderCircularPortal(const SDL_Rect& dest, SDL_Renderer* renderer);
        void setRenderHelper(HelperPtr value);

    private:
        HelperPtr helper;
        SDL_Rect sourcePortal;
        SDL_Rect sourceParticles;
        SDL_Rect sourceCircularPortal;
        SDL_Rect* updatePortal();
        SDL_Rect* updateParticles();
        SDL_Rect* updateCircularPortal();
};