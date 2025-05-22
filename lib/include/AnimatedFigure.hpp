#pragma once
#include <Utilities.hpp>
#include <iostream>
#include <RenderHelper.hpp>

class AnimatedFigure
{
    public:
        AnimatedFigure();
        ~AnimatedFigure();
        void renderPortal(const SDL_Rect& dest, SDL_Renderer* renderer, const float& deltaTime);
        void renderParticles(const SDL_Rect& dest, SDL_Renderer* renderer, const float& deltaTime);
        void renderCircularPortal(const SDL_Rect& dest, SDL_Renderer* renderer, const float& deltaTime);
        void renderCampfire(const SDL_Rect& dest, SDL_Renderer* renderer, const float& deltaTime);
        void setRenderHelper(HelperPtr value);

    private:
        HelperPtr helper;
        SDL_Rect sourcePortal;
        SDL_Rect sourceParticles;
        SDL_Rect sourceCircularPortal;
        SDL_Rect sourceCampfire;
        float portalTimer;
        float circularTimer;
        float campfireTimer;
        SDL_Rect* updatePortal(const float& deltaTime);
        SDL_Rect* updateParticles(const float& deltaTime);
        SDL_Rect* updateCircularPortal(const float& deltaTime);
        SDL_Rect* updateCampfire(const float& deltaTime);
};