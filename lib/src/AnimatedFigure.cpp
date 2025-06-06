#include <AnimatedFigure.hpp>

AnimatedFigure::AnimatedFigure()
{
    sourcePortal = {32*3,32*3,32,32};
    sourceParticles = {100*7,100*5,100,100};
    sourceCircularPortal = {32*3,48*2,32,48};
    sourceCampfire = {16*3,0,16,16};
    portalTimer = 0.0f;
    circularTimer = 0.0f;
    campfireTimer = 0.0f;
}

AnimatedFigure::~AnimatedFigure(){}

void AnimatedFigure::setRenderHelper(HelperPtr value){helper = value;}

void AnimatedFigure::setDeltaTime(DeltaTime value){deltaTime = value;}

bool AnimatedFigure::calcProximity(const SDL_Rect& dest, const SDL_Rect& player)
{
    int n = 2;
    if(std::abs((dest.y+dest.h)-(player.y+player.h))<=std::get<1>(helper.get()->getMeasuresRoom())*n)
    {
        return std::abs((dest.x+dest.w/2)-(player.x+player.w/2))<=std::get<0>(helper.get()->getMeasuresRoom())*n;
    }
    return false;
}

SDL_Rect* AnimatedFigure::updatePortal()
{
    portalTimer+=*deltaTime;
    if(portalTimer>=0.05f)
    {
        portalTimer = 0.0f;
        sourcePortal.x += 32;
        if(sourcePortal.x==32*4)
        {
            sourcePortal.x = 0;
            sourcePortal.y += 32;
        }
        if(sourcePortal.y==32*4){sourcePortal.y = 0;}
    }
    return &sourcePortal;
}

void AnimatedFigure::renderPortal(const SDL_Rect& dest, const SDL_Rect& player, SDL_Renderer* renderer)
{
    if(calcProximity(dest,player))
    {
        SDL_RenderCopy(renderer,helper.get()->getTexture("assets/sprites/portalRings.png",renderer),updatePortal(),&dest);
    }
    else
    {
        SDL_Rect source = {0,32*4,32,32};
        SDL_RenderCopy(renderer,helper.get()->getTexture("assets/sprites/portalRings.png",renderer),&source,&dest);
    }
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
    circularTimer+=*deltaTime;
    if(circularTimer>=0.2f)
    {
        circularTimer = 0.0f;
        sourceCircularPortal.x += 32;
        if(sourceCircularPortal.x==32*4)
        {
            sourceCircularPortal.x = 0;
            sourceCircularPortal.y += 48*1;
            if(sourceCircularPortal.y>=48*3){sourceCircularPortal.y = 48*1;}
        }
    }
    return &sourceCircularPortal;
}

void AnimatedFigure::renderCircularPortal(const SDL_Rect& dest, const SDL_Rect& player, SDL_Renderer* renderer)
{
    if(calcProximity(dest,player))
    {
        SDL_RenderCopy(renderer,helper.get()->getTexture("assets/sprites/circularPortal.png",renderer),updateCircularPortal(),&dest);
    }
    else
    {
        SDL_Rect destParticles = dest;
        if(helper.get()->getWindowWitdth()/2>=dest.x) //Lado izquierdo de la pantalla
        {
            destParticles.x += destParticles.w/2;
        }
        else //Lado derecho de la Pantalla
        {
            destParticles.x -= destParticles.w/2;
        }
        renderParticles(destParticles,renderer);
    }
}

SDL_Rect* AnimatedFigure::updateCampfire()
{
    campfireTimer+=*deltaTime;
    if(campfireTimer>=0.15f)
    {
        campfireTimer = 0.0f;
        sourceCampfire.x += 16;
        if(sourceCampfire.x==16*4)
        {
            sourceCampfire.x = 0;
        }
    }
    return &sourceCampfire;
}

void AnimatedFigure::renderCampfire(const SDL_Rect& dest, SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer,helper.get()->getTexture("assets/sprites/campfire.png",renderer),updateCampfire(),&dest);
}

