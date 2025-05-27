#pragma once 
#include <Utilities.hpp>
#include <iostream>
#include <HealthBar.hpp>
#include <RenderHelper.hpp>

class Enemy;
        
    enum class State 
    { 
        IDLE, RUNNING, ATTACKING, TAKING_DAMAGE, DEAD 
    };
    enum class Direction 
    { 
        LEFT, UP, RIGHT, DOWN
    };

class Player 
{
    public:

        Player();
        void initAnimation(SDL_Renderer* renderer);
        void renderPlayer(SDL_Renderer* renderer);
        void update();
        void setPosition(int x, int y);
        void setPosition(std::pair<int,int> value);
        void updateAnimation();
        void setState(State newState);
        void handleImput(const SDL_Event& event);
        void renderAttackHitbox(SDL_Renderer* renderer) const; //helper para ver el hitbox de ataque
        void attack(Enemy& enemies);
        void setHealth(int lives);
        void setHasHit(bool hit);
        void renderDebugBounds(SDL_Renderer* renderer); //helper para ver el rect del player
        void takeDamage(int damage);
        void handleWindowResize(int newWidth, int newHeight);
        void setRenderHelper(HelperPtr value);
        void setDeltaTime(DeltaTime value);

        int getAnimationRow() const;
        int getCurrentFrame() const;
        int getHealth() const;

        bool getIsInBound() const;
        bool isAlive() const;
        bool getHasHit() const;

        SDL_Rect getAttackHitbox() const;
        SDL_Rect getBounds() const;
        State getState() const { return currentState; }
        Direction getDirection() const {return currentDirection;}
        

    private:
        HelperPtr helper;
        DeltaTime deltaTime;

        SDL_Rect srcRect;
        SDL_Rect destRect;
        Direction currentDirection;
        State currentState;
        HealthBar healthBar;

        int frameWidth, frameHeight;
        int currentFrame = 0;
        int lives ;

        bool isMoving = false;
        bool hasHit = false;
        std::vector<bool> isInBound;

        float animTimer = 0.0f;
        float damageCooldown = 0.0f;
        const float DAMAGE_COOLDOWN_TIME = 0.2f; 

};