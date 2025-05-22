#pragma once
#include <Enemy.hpp>
#include <algorithm> 

class EnemyManager
{
public:
    EnemyManager();
    ~EnemyManager();

    void init(SDL_Renderer* renderer, TexturePtr enemyTexture);
    void addEnemy(int x, int y);
    void update(float deltaTime, Player& player);
    void render(SDL_Renderer* renderer);
    void handlePlayerAttack(Player& player);
    int getScore() const;

private:
    std::vector<std::unique_ptr<Enemy>> enemies;
    TexturePtr enemyTexture; 
    int score; 
};