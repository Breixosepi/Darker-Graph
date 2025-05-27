#include "Enemy.hpp"
#include <cmath>
#include <Combat.hpp>

Enemy::Enemy() :
    currentDirection(EnemyDirection::LEFT),
    currentState(EnemyState::PATROLLING)
{
    srcRect = {0, 0, 0, 0};
    destRect = {0, 0, 0, 0};
}

void Enemy::initAnimation(SDL_Renderer* renderer, SDL_Texture* texture)
{
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
    if (currentState == EnemyState::DEAD)
    {
        if (currentFrame < 3) 
        {
            updateAnimation(deltaTime);
        }
        else
        {
            deathCompleted = true; 
        }
        return;
    }
    else if (currentState == EnemyState::ATTACKING)
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
        animSpeed = 0.1f;
        cooldownTimer += deltaTime;
        if (cooldownTimer >=attackCooldown) 
        {
            cooldownTimer = 0.0f;
            animSpeed = 0.2f;
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
    if (animTimer >= animSpeed)
    {
        animTimer = 0.0f;
        if (currentState == EnemyState::DEAD)
        {
            if (currentFrame < 3)
            {
                currentFrame++;
            }
        }
        else
        {
            currentFrame = (currentFrame + 1) % 4;
            if (currentState == EnemyState::ATTACKING && currentFrame == 0 && attackInProgress)
            {
                attackInProgress = false;
            }
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
        case EnemyState::TAKING_DAMAGE: return 4;
        case EnemyState::DEAD: return 5;
        default: return 0;
    }
}

void Enemy::renderEnemy(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RendererFlip flip = (currentDirection == EnemyDirection::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0, nullptr, flip);
}

void Enemy::setState(EnemyState newState)
{

    if (currentState != newState)
    {
        if (currentState == EnemyState::TAKING_DAMAGE && newState != EnemyState::PATROLLING) 
        {
            if (newState == EnemyState::DEAD) 
            {
                currentState = newState;
                currentFrame = 0;
                animTimer = 0.0f;
            }           
            return;
        }
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

    const int enemyWidth = frameWidth ;  
    const int enemyHeight = frameHeight ; 
    
    return {
        destRect.x + (destRect.w -frameWidth) / 2 , 
        destRect.y + (destRect.h - frameHeight) / 2,
        enemyWidth,
        enemyHeight
    };
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

void Enemy::renderDebugBounds(SDL_Renderer* renderer) const
{
    SDL_Rect bounds = getBounds();
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
    SDL_RenderFillRect(renderer, &bounds);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &bounds);
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Enemy::attack(Player& player)
{
    Combat::enemyAttack(*this, player);
}

SDL_Rect Enemy::getAttackHitbox() const 
{
    if (currentState != EnemyState::ATTACKING) 
    {
        return {0, 0, 0, 0};
    }

    int baseWidth = frameWidth * 6;  
    int baseHeight = frameHeight * 6; 
    
    SDL_Rect attackHitbox = {0, 0, 0, 0};
    int attackRange = 60; 
    int hitboxWidth = baseWidth * 0.6f;
    int hitboxHeight = baseHeight * 0.6f;

    if (currentDirection == EnemyDirection::RIGHT) 
    {
        attackHitbox = { destRect.x + baseWidth/2 + attackRange, destRect.y + baseHeight/3, attackRange, hitboxHeight/2};
    } 
    else 
    {
        attackHitbox = { destRect.x + attackRange*3, destRect.y + baseHeight/3, attackRange, hitboxHeight/2};
    }

    return attackHitbox;
}

void Enemy::renderAttackHitbox(SDL_Renderer* renderer) const
{
    if (currentState != EnemyState::ATTACKING) 
    { 
        return;  
    }

    SDL_Rect hitbox = getAttackHitbox();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);  
    SDL_RenderFillRect(renderer, &hitbox);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    SDL_RenderDrawRect(renderer, &hitbox);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

EnemyState Enemy::getState()
{
    return currentState;
}   

int Enemy::getCurrentFrame() const 
{
    return currentFrame;
}

bool Enemy::getHasHit() const 
{
    return hasHit;
}

void Enemy::setHasHit(bool hit) 
{
    hasHit = hit;
}

int Enemy::getHealth() const 
{
    return health;
}

void Enemy::setHealth(int health) 
{
    this->health = health;
}
bool Enemy::isDeathAnimationComplete() const
{
    return deathCompleted;
}