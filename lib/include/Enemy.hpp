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
    void initAnimation(SDL_Renderer* renderer, const TexturePtr &texture);
    void renderEnemy(SDL_Renderer* renderer);
    void update(float deltaTime);
    void setPosition(int x, int y);
    void setState(EnemyState newState);
    void patrol(float deltaTime);
    void updateAnimation(float deltaTime);
    int getAnimationRow() const;
    void detectPlayer(const SDL_Rect& playerRect);
    void attack(Player& player);
    SDL_Rect getBounds() const;
    SDL_Rect getAttackHitbox() const; 
    void renderAttackHitbox(SDL_Renderer* renderer) const; //helper para ver el hitbox de ataque
    void renderDebugBounds(SDL_Renderer* renderer) const; //helper para ver el rect del enemigo
 

private:
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    EnemyDirection currentDirection;
    EnemyState currentState;
    int frameWidth, frameHeight, startX, currentFrame = 0, patrolRange = 300;
    float animSpeed = 0.2f;
    float animTimer = 0.0f, speed = 150.0f;
    float attackCooldown = 1.0f; 
    float cooldownTimer = 0.0f;
    bool attackInProgress = false;
    bool hasHit = false;
};