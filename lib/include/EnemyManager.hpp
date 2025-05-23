#pragma once
#include <Enemy.hpp>
#include <algorithm> 
#include <Room.hpp>

class EnemyManager 
{
private:
    struct RoomData 
    {
        int roomIndex;
        std::vector<std::unique_ptr<Enemy>> enemies;
    };
    
    std::vector<RoomData> roomsEnemies;
    TexturePtr enemyTexture;
    SDL_Renderer* renderer;
    int currentRoomIndex;
    int score;

public:
    EnemyManager();
    ~EnemyManager();
    
    void init(SDL_Renderer* renderer, const std::string& texturePath);
    void addEnemy(int roomIndex, int x, int y);
    void setCurrentRoom(int roomIndex);
    void update(float deltaTime, Player& player);
    void render();
    void handlePlayerAttack(Player& player);
    int getScore() const;
    void setScore(int newScore) ;
};