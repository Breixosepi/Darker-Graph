#include "Enemy.hpp"
#include <iostream>
#include <cmath>

Enemy::Enemy() :
    texture(nullptr),
    currentDirection(EnemyDirection::LEFT),
    currentState(EnemyState::PATROLLING)
{
    srcRect = {0, 0, 0, 0};
    destRect = {0, 0, 0, 0};
}

void Enemy::initAnimation(const RendererPtr& renderer, const TexturePtr& tex)
{
    texture = tex.get();

    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    frameWidth = textureWidth / 8;   // 8 columnas
    frameHeight = textureHeight / 6; // 6 filas

    srcRect = {0, 0, frameWidth, frameHeight};
    destRect = {0, 0, frameWidth * 6, frameHeight * 6};
}

void Enemy::setPosition(int x, int y)
{
    destRect.x = x;
    destRect.y = y;
    startX = x;
}

void Enemy::update(float deltaTime)
{
    if (currentState == EnemyState::ATTACKING)
    {
        cooldownTimer += deltaTime;

        if (cooldownTimer >= attackCooldown)
        {
            cooldownTimer = 0.0f;
        }
    }
    else if (currentState == EnemyState::PATROLLING)
    {
        patrol(deltaTime);
    }
    else if (currentState == EnemyState::TAKING_DAMAGE)
    {
        cooldownTimer += deltaTime;
        if (cooldownTimer >= 0.5f) 
        {
            cooldownTimer = 0.0f;
            setState(EnemyState::PATROLLING);
        }
    }

    updateAnimation(deltaTime);
}



void Enemy::patrol(float deltaTime)
{
    if (currentDirection == EnemyDirection::RIGHT)
    {
        destRect.x += static_cast<int>(speed * deltaTime);
        if (destRect.x >= startX + patrolRange)
        {
            currentDirection = EnemyDirection::LEFT;
        }
    }
    else
    {
        destRect.x -= static_cast<int>(speed * deltaTime);
        if (destRect.x <= startX)
        {
            currentDirection = EnemyDirection::RIGHT;
        }
    }
}

void Enemy::updateAnimation(float deltaTime)
{
    animTimer += deltaTime;
    if (animTimer >= ANIM_SPEED)
    {
        animTimer = 0.0f;
        currentFrame = (currentFrame + 1) % 4;
        if (currentState == EnemyState::ATTACKING && currentFrame == 0 && attackInProgress)
        {
            attackInProgress = false;
        }
    }
    srcRect.x = currentFrame * frameWidth;
    srcRect.y = getAnimationRow() * frameHeight;
}

int Enemy::getAnimationRow() const
{
    switch (currentState)
    {
        case EnemyState::IDLE: return 0;
        case EnemyState::PATROLLING: return 1;
        case EnemyState::ATTACKING: return 3;
        case EnemyState::TAKING_DAMAGE: return 5;
        default: return 0;
    }
}

void Enemy::renderEnemy(const RendererPtr& renderer)
{
    SDL_RendererFlip flip = (currentDirection == EnemyDirection::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer.get(), texture, &srcRect, &destRect, 0, nullptr, flip);
}

void Enemy::setState(EnemyState newState)
{
    if (currentState != newState)
    {
        currentState = newState;
        currentFrame = 0;
        animTimer = 0.0f;
        if (newState == EnemyState::ATTACKING)
        {
            attackInProgress = true;
        }
        else
        {
            attackInProgress = false;
        }
    
    }
}

SDL_Rect Enemy::getBounds() const
{
    return destRect;
}

void Enemy::detectPlayer(const SDL_Rect& playerRect)
{
    if (currentState == EnemyState::TAKING_DAMAGE || currentState == EnemyState::DEAD)
    {
        return;
    }
    int enemyCenterX = destRect.x + destRect.w / 2;
    int enemyCenterY = destRect.y + destRect.h / 2;
    int playerCenterX = playerRect.x + playerRect.w / 2;
    int playerCenterY = playerRect.y + playerRect.h / 2;
    int distanceX = playerCenterX - enemyCenterX;
    int distanceY = playerCenterY - enemyCenterY;
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);
    const float detectionRange = 120.0f; 

    if (distance <= detectionRange)
    {
        bool playerInFront = false;

        if (currentDirection == EnemyDirection::RIGHT && distanceX > 0)
        {
            playerInFront = true;
        }
        else if (currentDirection == EnemyDirection::LEFT && distanceX < 0)
        {
            playerInFront = true;
        }

        if (playerInFront)
        {
            if (currentState != EnemyState::ATTACKING)
            {
                setState(EnemyState::ATTACKING);
            }
        }
        else
        {
            if (currentState == EnemyState::ATTACKING && !attackInProgress)
            {
                setState(EnemyState::PATROLLING);
            }
        }
    }
    else
    {
        if (currentState == EnemyState::ATTACKING && !attackInProgress)
        {
            setState(EnemyState::PATROLLING);
        }
    }
}