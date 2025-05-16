#include "Enemy.hpp"

Enemy::Enemy() : 
    texture(nullptr),
    currentDirection(EnemyDirection::LEFT),
    currentState(EnemyState::PATROLLING) 
{
    srcRect = {0, 0, 0, 0};
    destRect = {0, 0, 0, 0};
}

void Enemy::initAnimation(const RendererPtr &renderer, const TexturePtr &tex) 
{
    texture = tex.get();
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

    frameWidth = textureWidth / 8;   
    frameHeight = textureHeight / 6; 
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
    if (currentState == EnemyState::PATROLLING) 
    {
        patrol(deltaTime);
    }
    updateAnimation(deltaTime);
}

void Enemy::patrol(float deltaTime) 
{
    if (currentDirection == EnemyDirection::RIGHT) 
    {
        destRect.x += speed * deltaTime;
        if (destRect.x >= startX + patrolRange) 
        {
            currentDirection = EnemyDirection::LEFT;
        }
    } 
    else 
    {
        destRect.x -= speed * deltaTime;
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
    }
    srcRect.x = currentFrame * frameWidth;
    srcRect.y = getAnimationRow() * frameHeight;
}

int Enemy::getAnimationRow() const 
{
    return (currentState == EnemyState::PATROLLING) ? 1 : 0; 
}

void Enemy::renderEnemy(const RendererPtr &renderer) 
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
    }
}

SDL_Rect Enemy::getBounds() const 
{
    return destRect;
}