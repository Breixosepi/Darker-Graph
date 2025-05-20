#include <AnimatedFigure.hpp>

AnimatedFigure::AnimatedFigure(){}

AnimatedFigure::~AnimatedFigure(){}

void AnimatedFigure::initialize(SDL_Renderer* renderer)
{
    sourcePortal = {32*3,32*3,32,32};
    sourceParticles = {100*7,100*5,100,100};
    sourceCircularPortal = {0,48*2,32,48};
    textPortal = initializeTexture("assets/sprites/portalRings.png",renderer);
    textParticles = initializeTexture("assets/sprites/blueParticles.png",renderer);
    textCircularPortal = initializeTexture("assets/sprites/circularPortal.png",renderer);
}

SDL_Texture* AnimatedFigure::initializeTexture(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surf = IMG_Load(path);
    if(surf)
    {
        SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
        if(!text)
        {
            std::cout<<"Texture "<<path<<" dont created."<<std::endl;
        }
        SDL_FreeSurface(surf);
        return text;
    }
    else{std::cout<<"Surface "<<path<<" dont created."<<std::endl;}
    return nullptr;
}

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
    SDL_RenderCopy(renderer,textPortal,updatePortal(),&dest);
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
    SDL_RenderCopy(renderer,textParticles,updateParticles(),&dest);   
}

SDL_Rect* AnimatedFigure::updateCircularPortal()
{
    sourceCircularPortal.x += 32;
    if(sourceCircularPortal.x==32*4)
    {
        sourceCircularPortal.x = 0;
    }
    return &sourceCircularPortal;
}

void AnimatedFigure::renderCircularPortal(const SDL_Rect& dest, SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer,textCircularPortal,updateCircularPortal(),&dest);
}

