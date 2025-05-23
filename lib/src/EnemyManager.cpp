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

void EnemyManager::init(SDL_Renderer* renderer, const std::string& texturePath)
{
    this->renderer= renderer;
    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    enemyTexture.reset(SDL_CreateTextureFromSurface(renderer, surface));
    SDL_FreeSurface(surface);

}

void EnemyManager::addEnemy(int roomIndex, int x, int y) 
{
    auto it = std::find_if(roomsEnemies.begin(), roomsEnemies.end(),[roomIndex](const RoomData& rd) 
    { 
        return rd.roomIndex == roomIndex; 
    });
    
    if (it == roomsEnemies.end()) 
    {
        roomsEnemies.push_back({roomIndex, {}});
        it = roomsEnemies.end() - 1;
    }
    
    std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
    newEnemy->initAnimation(renderer, enemyTexture);
    newEnemy->setPosition(x, y);
    newEnemy->setHealth(2);
    newEnemy->setState(EnemyState::PATROLLING);
    
    it->enemies.push_back(std::move(newEnemy));
    // std::cout << "Enemy added in room " << roomIndex << " at: " << x << ", " << y << std::endl;
}

void EnemyManager::setCurrentRoom(int roomIndex) 
{
    currentRoomIndex = roomIndex;
}

void EnemyManager::update(float deltaTime, Player& player) 
{
    auto it = std::find_if(roomsEnemies.begin(), roomsEnemies.end(),[this](const RoomData& rd) 
    { 
        return rd.roomIndex == currentRoomIndex; 
    });
    
    if (it != roomsEnemies.end()) 
    {
        
        for (auto& enemy : it->enemies) 
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
        
        it->enemies.erase(std::remove_if(it->enemies.begin(), it->enemies.end(),[this](const std::unique_ptr<Enemy>& enemy) 
        {
                if (enemy->getState() == EnemyState::DEAD && enemy->isDeathAnimationComplete()) 
                {
                    score += 100;
                    std::cout << "Enemy defeated! Score: " << score << std::endl;
                    return true;
                }
                return false;
            }), it->enemies.end());
    }
}

void EnemyManager::render() 
{
    auto it = std::find_if(roomsEnemies.begin(), roomsEnemies.end(),[this](const RoomData& rd) 
    { 
        return rd.roomIndex == currentRoomIndex; 
    });
    
    if (it != roomsEnemies.end()) 
    {
        for (auto& enemy : it->enemies) 
        {
            if (enemy->getState() != EnemyState::DEAD || !enemy->isDeathAnimationComplete()) 
            {
                enemy->renderEnemy(renderer);
                // enemy->renderDebugBounds(renderer);      
                // enemy->renderAttackHitbox(renderer);     
            }
        }
    }
}

void EnemyManager::handlePlayerAttack(Player& player) 
{
    auto it = std::find_if(roomsEnemies.begin(), roomsEnemies.end(),[this](const RoomData& rd) 
    { 
        return rd.roomIndex == currentRoomIndex; 
    });
    
    if (it != roomsEnemies.end()) 
    {
        SDL_Rect playerAttackHitbox = player.getAttackHitbox();
        
        for (auto& enemy : it->enemies) 
        {
            if (enemy->getState() != EnemyState::DEAD) 
            {
                SDL_Rect enemyBounds = enemy->getBounds();
                if (SDL_HasIntersection(&playerAttackHitbox, &enemyBounds)) {
                    Combat::playerAttack(player, *enemy);
                }
            }
        }
    }
}

int EnemyManager::getScore() const
{
    return score;
}

void EnemyManager::setScore(int newScore)
{
    score = newScore;
}
