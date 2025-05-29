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

    frameWidth = (textureWidth / 8);   // 8 columnas
    frameHeight = (textureHeight / 6); // 6 filas

    srcRect = {0, 0, 38, 32};
}

void Enemy::setDestSize(const double& width, const double& height)
{
    destRect.w = width;
    destRect.h = height;
}

void Enemy::setPosition(int x, int y)
{
    destRect.x = x;
    destRect.y = y;
    //Hay que hacer un setter aparte para el Start, tiene un conflicto cuando se hace resize
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
    srcRect.x = currentFrame * frameWidth + 36; //Se borran 36 pixeles horizontales entre frame y frame
    srcRect.y = getAnimationRow() * frameHeight + 32; //Se borran 32 pixeles horizontales entre frame y frame
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

SDL_Rect Enemy::getDest() const{return destRect;}

SDL_Rect Enemy::getBounds() const
{
    //Reajusta los bordes a lo que se ve visualmente
    //Originalmente el Rectangulo de Destino ocupa mas espacio debido al tamano de los sprites atacando
    //Como su animacion de movimiento tiene el arma afuera, tambien se considera como hitbox
    SDL_Rect bounds = destRect;
    double difInX = destRect.w*0.235;
    double difInY = destRect.h*0.315;
    bounds.x += difInX;
    bounds.w -= difInX*3;
    bounds.y += difInY;
    bounds.h -= difInY*1.5;
    if(currentDirection==EnemyDirection::LEFT)
    {
        bounds.x += (bounds.x-destRect.x);
    }
    return bounds;
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

    SDL_Rect attackHitbox = {0, 0, 0, 0};
    SDL_Rect bounds = getBounds();
    int hitboxWidth = bounds.w * 1.80f;
    int hitboxHeight = bounds.h * 1.50f;

    if (currentDirection == EnemyDirection::RIGHT) 
    {
        attackHitbox = { bounds.x + 3*bounds.w/4, bounds.y - bounds.h/4, hitboxWidth, hitboxHeight};
    } 
    else 
    {
        attackHitbox = { bounds.x - bounds.w/4, bounds.y - bounds.h/4, hitboxWidth, hitboxHeight};
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