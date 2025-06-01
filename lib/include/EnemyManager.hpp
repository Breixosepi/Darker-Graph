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
    
    HelperPtr helper;
    DeltaTime deltaTime;
    std::vector<RoomData> roomsEnemies;
    std::string texturePath;

    int currentRoomIndex;
    int score;

public:
    EnemyManager();
    ~EnemyManager();

    
    void setTexturePathEnemies(const std::string& path);
    void setRenderHelper(HelperPtr value);
    void setDeltaTime(DeltaTime value);
    void addEnemy(SDL_Renderer* renderer, int roomIndex, SDL_Rect dest, double resize, int tilesToPatrol);
    void setCurrentRoom(int roomIndex);
    void update(Player& player);
    void render(SDL_Renderer* renderer);
    void handlePlayerAttack(Player& player);
    void handleWindowResize(const Measures& lastMeasures);
    void setScore(int newScore);
    void generate(SDL_Renderer* renderer, const Designar::Graph<Room>* map);
    int getScore() const;
};