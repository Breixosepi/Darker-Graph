// EnemyManager.cpp
#include "EnemyManager.hpp"
#include <iostream>
#include <Combat.hpp>

EnemyManager::EnemyManager() : score(0)
{
    // Constructor
}

EnemyManager::~EnemyManager()
{
    // Destructor
}

void EnemyManager::init(SDL_Renderer* renderer, TexturePtr enemyTex)
{
    enemyTexture = std::move(enemyTex);
}

void EnemyManager::addEnemy(int x, int y)
{
    auto newEnemy = std::make_unique<Enemy>();
    newEnemy->initAnimation(nullptr, enemyTexture);
    newEnemy->setPosition(x, y);
    newEnemy->setHealth(2); 
    newEnemy->setState(EnemyState::PATROLLING);
    enemies.push_back(std::move(newEnemy));
    std::cout << "Enemy added at: " << x << ", " << y << std::endl;
}

void EnemyManager::update(float deltaTime, Player& player)
{
    
    for (auto& enemy : enemies)
    {
        enemy->detectPlayer(player.getBounds());
        if (enemy->getState() != EnemyState::DEAD || !enemy->isDeathAnimationComplete())
        {
            enemy->update(deltaTime);
            if (enemy->getState() == EnemyState::ATTACKING)
            {
                SDL_Rect enemyAttackHitbox = enemy->getAttackHitbox();
                SDL_Rect playerBounds = player.getBounds();
                if (SDL_HasIntersection(&enemyAttackHitbox, &playerBounds))
                {
                    Combat::enemyAttack(*enemy, player);
                }
            }
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),[this](const std::unique_ptr<Enemy>& enemy) 
    {
        if (enemy->getState() == EnemyState::DEAD && enemy->isDeathAnimationComplete()) 
        {
            score += 100;
            return true; 
        }
        return false;
    }), enemies.end());
}

void EnemyManager::render(SDL_Renderer* renderer)
{
    for (auto& enemy : enemies)
    {
        if (enemy->getState() != EnemyState::DEAD || !enemy->isDeathAnimationComplete())
        {
            enemy->renderEnemy(renderer);
            enemy->renderDebugBounds(renderer);
            enemy->renderAttackHitbox(renderer);
        }
    }
}

void EnemyManager::handlePlayerAttack(Player& player)
{
    SDL_Rect playerAttackHitbox = player.getAttackHitbox();

    for (auto& enemy : enemies)
    {
        if (enemy->getState() != EnemyState::DEAD)
        {
            SDL_Rect enemyBounds = enemy->getBounds();
            if (SDL_HasIntersection(&playerAttackHitbox, &enemyBounds))
            {
                Combat::playerAttack(player, *enemy);
            }
        }
    }
}

int EnemyManager::getScore() const
{
    return score;
}