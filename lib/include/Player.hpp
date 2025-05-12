#pragma once 
#include <Utilities.hpp>
        
    enum class State 
    { 
        IDLE, RUNNING, ATTACKING, TAKING_DAMAGE, DEAD 
    };
    enum class Direction 
    { 
        RIGHT, UP, DOWN, LEFT 
    };

class Player 
{
    public:

        Player();
        void initAnimation(const RendererPtr &renderer, const TexturePtr &texture);
        void renderPlayer(const RendererPtr &renderer);
        void update(float deltaTime);
        void setPosition(int x, int y);
        void updateAnimation(float deltaTime);
        void setState(State newState);
        void handleImput(const SDL_Event& event);
        int getAnimationRow() const;
        
    private:
        SDL_Texture* texture;
        SDL_Rect srcRect;
        SDL_Rect destRect;
        Direction currentDirection;
        State currentState;
        int frameWidth, frameHeight;
        bool isMoving = false;
        int currentFrame = 0;
        float animTimer = 0.0f;
        

};