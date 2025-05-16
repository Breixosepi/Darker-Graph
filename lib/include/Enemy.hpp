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
    SDL_Rect getBounds() const;

private:
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    EnemyDirection currentDirection;
    EnemyState currentState;
    int frameWidth, frameHeight, startX, currentFrame = 0, patrolRange = 200;
    const float ANIM_SPEED = 0.15f;
    float animTimer = 0.0f, speed = 100.0f;
};