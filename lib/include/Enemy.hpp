#pragma once
#include <Utilities.hpp>
#include "Player.hpp"

enum class EnemyState 
{
    IDLE, PATROLLING, CHASING, ATTACKING, TAKING_DAMAGE, DEAD
};

enum class EnemyDirection 
{
    RIGHT, LEFT, UP, DOWN
};

class Enemy 
{
public:
    Enemy();
    void initAnimation(SDL_Renderer* renderer, SDL_Texture* texture);
    void renderEnemy(SDL_Renderer* renderer, SDL_Texture* texture);
    void update(float deltaTime);
    void setPosition(int x, int y);
    void setState(EnemyState newState);
    void setDestSize(const double& width, const double& height);
    void patrol(float deltaTime);
    void updateAnimation(float deltaTime);
    void detectPlayer(const SDL_Rect& playerRect);
    void attack(Player& player);
    void setHasHit(bool hit);
    void renderAttackHitbox(SDL_Renderer* renderer) const; //helper para ver el hitbox de ataque
    void setHealth(int health);
    void renderDebugBounds(SDL_Renderer* renderer) const; //helper para ver el rect del enemigo
    
    int getAnimationRow() const;
    int getCurrentFrame() const;
    int getHealth() const;

    bool getHasHit() const;
    bool isDeathAnimationComplete() const;

    EnemyState getState();
    SDL_Rect getBounds() const;
    SDL_Rect getAttackHitbox() const; 

 

private:

    SDL_Rect srcRect;
    SDL_Rect destRect;

    EnemyDirection currentDirection;
    EnemyState currentState;

    int frameWidth, frameHeight, startX, currentFrame = 0, patrolRange = 300;
    float animSpeed = 0.2f;
    float animTimer = 0.0f, speed = 150.0f;
    float attackCooldown = 1.0f; 
    float cooldownTimer = 0.0f;
    bool attackInProgress = false, hasHit = false, deathCompleted = false;
    int health = 2;
};
