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
        void initAnimation(SDL_Renderer* renderer, const TexturePtr &texture);
        void renderPlayer(SDL_Renderer* renderer);
        void update(float deltaTime);
        void setPosition(int x, int y);
        void setPosition(std::pair<int,int> value);
        void updateAnimation(float deltaTime);
        void setState(State newState);
        void handleImput(const SDL_Event& event);
        int getAnimationRow() const;
        void attack(Enemy& enemies);
        int getCurrentFrame() const;
        SDL_Rect getAttackHitbox() const;
        SDL_Rect getBounds() const;
        int getHealth() const;
        void setHealth(int lives);
        State getState() const { return currentState; }
        Direction getDirection() const {return currentDirection;}
        bool getIsInBound() const;
        void renderAttackHitbox(SDL_Renderer* renderer) const; //helper para ver el hitbox de ataque
        void renderDebugBounds(SDL_Renderer* renderer); //helper para ver el rect del player
        bool getHasHit() const;
        void setHasHit(bool hit);
        void takeDamage(int damage);
        void handleWindowResize(int newWidth, int newHeight);
        void setRenderHelper(HelperPtr value);

    private:
        HelperPtr helper;
        SDL_Texture* texture;
        SDL_Rect srcRect;
        SDL_Rect destRect;
        Direction currentDirection;
        State currentState;
        int frameWidth, frameHeight;
        bool isMoving = false;
        int currentFrame = 0;
        float animTimer = 0.0f;
        float damageCooldown = 0.0f;
        const float DAMAGE_COOLDOWN_TIME = 0.2f; 
        bool hasHit = false;
        int lives ;
        HealthBar healthBar;
        std::vector<bool> isInBound;
};