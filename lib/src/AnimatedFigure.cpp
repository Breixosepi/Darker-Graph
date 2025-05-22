#include <AnimatedFigure.hpp>

AnimatedFigure::AnimatedFigure()
{
    sourcePortal = {32*3,32*3,32,32};
    sourceParticles = {100*7,100*5,100,100};
    sourceCircularPortal = {32*3,48*3,32,48};
}

AnimatedFigure::~AnimatedFigure(){}

void AnimatedFigure::setRenderHelper(HelperPtr value){helper = value;}

SDL_Rect* AnimatedFigure::updatePortal()
{
    sourcePortal.x += 32;
    if(sourcePortal.x==32*4)
    {
        sourcePortal.x = 0;
        sourcePortal.y += 32;
    }
    if(sourcePortal.y==32*4){sourcePortal.y = 0;}
    return &sourcePortal;
}

void AnimatedFigure::renderPortal(const SDL_Rect& dest, SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer,helper.get()->getTexture("assets/sprites/portalRings.png",renderer),updatePortal(),&dest);
}

SDL_Rect* AnimatedFigure::updateParticles()
{
    sourceParticles.x += 100;
    if(sourceParticles.x==100*8)
    {
        sourceParticles.x = 0;
        sourceParticles.y += 100;
    }
    if(sourceParticles.y==100*8){sourceParticles.y = 0;}
    return &sourceParticles;
}

void AnimatedFigure::renderParticles(const SDL_Rect& dest, SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer,helper.get()->getTexture("assets/sprites/blueParticles.png",renderer),updateParticles(),&dest);   
}

SDL_Rect* AnimatedFigure::updateCircularPortal()
{
    sourceCircularPortal.x += 32;
    if(sourceCircularPortal.x==32*4)
    {
        sourceCircularPortal.x = 0;
        sourceCircularPortal.y += 48;
        if(sourceCircularPortal.y==48*4){sourceCircularPortal.y = 48*2;}
    }
    return &sourceCircularPortal;
}

void AnimatedFigure::renderCircularPortal(const SDL_Rect& dest, SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer,helper.get()->getTexture("assets/sprites/circularPortal.png",renderer),updateCircularPortal(),&dest);
}

