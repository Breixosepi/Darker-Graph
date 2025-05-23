#pragma once
#include <Enemy.hpp>
#include <algorithm> 
#include <Room.hpp>

class Room;

class EnemyManager
{
public:
    EnemyManager();
    ~EnemyManager();

    void init(SDL_Renderer* renderer, const std::string& texturePath);
    void addEnemy(int x, int y);
    void update(float deltaTime, Player& player);
    void render();
    void handlePlayerAttack(Player& player);
    int getScore() const;

private:
    std::vector<std::unique_ptr<Enemy>> enemies;
    TexturePtr enemyTexture; 
    SDL_Renderer* renderer;
    int score; 
};