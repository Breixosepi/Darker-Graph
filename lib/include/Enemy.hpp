#pragma once
#include <Utilities.hpp>

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
    void initAnimation(const RendererPtr &renderer, const TexturePtr &texture);
    void renderEnemy(const RendererPtr &renderer);
    void update(float deltaTime);
    void setPosition(int x, int y);
    void setState(EnemyState newState);
    void patrol(float deltaTime);
    void updateAnimation(float deltaTime);
    int getAnimationRow() const;
    void detectPlayer(const SDL_Rect& playerRect);
    SDL_Rect getBounds() const;
 

private:
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    EnemyDirection currentDirection;
    EnemyState currentState;
    int frameWidth, frameHeight, startX, currentFrame = 0, patrolRange = 300;
    const float ANIM_SPEED = 0.3f;
    float animTimer = 0.0f, speed = 150.0f;
    float attackCooldown = 1.0f; 
    float cooldownTimer = 0.0f;
    bool attackInProgress = false;

};