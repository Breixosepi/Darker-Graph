#pragma once
#include <Utilities.hpp>
#include <iostream>
#include <RenderHelper.hpp>

class AnimatedFigure
{
    public:
        AnimatedFigure();
        ~AnimatedFigure();

        void renderPortal(const SDL_Rect& dest, const SDL_Rect& player, SDL_Renderer* renderer);
        void renderParticles(const SDL_Rect& dest, SDL_Renderer* renderer);
        void renderCircularPortal(const SDL_Rect& dest, const SDL_Rect& player, SDL_Renderer* renderer);
        void renderCampfire(const SDL_Rect& dest, SDL_Renderer* renderer);
        void setRenderHelper(HelperPtr value);
        void setDeltaTime(DeltaTime value);

    private:
        HelperPtr helper;
        DeltaTime deltaTime;
        SDL_Rect sourcePortal;
        SDL_Rect sourceParticles;
        SDL_Rect sourceCircularPortal;
        SDL_Rect sourceCampfire;

        float portalTimer;
        float circularTimer;
        float campfireTimer;
        
        SDL_Rect* updatePortal();
        SDL_Rect* updateParticles();
        SDL_Rect* updateCircularPortal();
        SDL_Rect* updateCampfire();

        bool calcProximity(const SDL_Rect& dest, const SDL_Rect& player);
};