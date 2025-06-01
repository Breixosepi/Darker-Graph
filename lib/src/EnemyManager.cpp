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

void EnemyManager::setTexturePathEnemies(const std::string& path){texturePath = path;}

void EnemyManager::setRenderHelper(HelperPtr value){helper = value;}

void EnemyManager::setDeltaTime(DeltaTime value){deltaTime = value;}

void EnemyManager::addEnemy(SDL_Renderer* renderer, int roomIndex, SDL_Rect dest, double resize, int tilesToPatrol) 
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
    newEnemy->initAnimation(renderer, helper.get()->getTexture(texturePath, renderer));
    newEnemy->setPosition(dest.x,dest.y);
    newEnemy->setStartX(dest.x);
    newEnemy->setDestSize(dest.w,dest.h);
    newEnemy->setResize(resize);
    newEnemy->setHealth(2);
    newEnemy->setState(EnemyState::PATROLLING);
    int patrolRange = tilesToPatrol*helper->widthTile() + (helper->widthTile()-dest.w);
    newEnemy->setPatrolRange(patrolRange);
    newEnemy->setTilesToPatrol(tilesToPatrol);
    it->enemies.push_back(std::move(newEnemy));
    // std::cout << "Enemy added in room " << roomIndex << " at: " << x << ", " << y << std::endl;
}

void EnemyManager::setCurrentRoom(int roomIndex) 
{
    currentRoomIndex = roomIndex;
}

void EnemyManager::update(Player& player) 
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
                enemy->update(*deltaTime);
                
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

void EnemyManager::render(SDL_Renderer* renderer) 
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
                enemy->renderEnemy(renderer, helper.get()->getTexture(texturePath,renderer));
                //enemy->renderDebugBounds(renderer);      
                //enemy->renderAttackHitbox(renderer);     
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

void EnemyManager::handleWindowResize(const Measures& lastMeasures)
{
    auto it = std::find_if(roomsEnemies.begin(), roomsEnemies.end(),[this](const RoomData& rd) 
    { 
        return rd.roomIndex == currentRoomIndex; 
    });
    
    if (it != roomsEnemies.end()) 
    {
        double widthTile = std::get<0>(lastMeasures);
        double heightTile = std::get<1>(lastMeasures);
        double shrinkX = std::get<2>(lastMeasures);
        double shrinkY = std::get<3>(lastMeasures);

        for (auto& enemy : it->enemies) 
        {
            double factorStart = (enemy->getStartX()-shrinkX)/widthTile;
            double factorX = (enemy->getDest().x-shrinkX)/widthTile;
            double factorY = (enemy->getDest().y-shrinkY)/heightTile;

            int X = helper->shrinkX()+factorX*helper->widthTile();
            int Y = helper->shrinkY()+factorY*helper->heightTile();

            int width = helper->widthTile()*enemy->getResize();
            int height = helper->heightTile()*enemy->getResize();
            int patrolRange = enemy->getTilesToPatrol()*helper->widthTile() + (helper->widthTile()-width);
            
            enemy->setDestSize(width,height);
            enemy->setPosition(X,Y);
            enemy->setStartX(helper->shrinkX()+factorStart*helper->widthTile());
            enemy->setPatrolRange(patrolRange);
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

void EnemyManager::generate(SDL_Renderer* renderer, const Designar::Graph<Room>* map)
{
    if (map) 
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> countEnemies(1, 5);
        std::uniform_real_distribution<> enemySize(1, 1.75);
        map->enum_for_each_node([&](Designar::nat_t index, Designar::Graph<Room>::Node* node) 
        {
            if (node&&index!=1&&index!=map->get_num_nodes()) 
            {
                int roomIndex = index;
                int enemyCount = countEnemies(gen);
                int numDivisions = *node->get_info().getDivisions();
                std::uniform_int_distribution<> numTiles(1,numDivisions-1);
                for (int i = 0; i < enemyCount; ++i) 
                {
                    double resize = enemySize(gen);
                    int width = helper->widthTile()*resize;
                    int height = helper->heightTile()*resize;
                    int num = numTiles(gen);

                    std::uniform_int_distribution<> xDist(helper->shrinkX(), helper->shrinkX()+(numDivisions-num-1)*helper->widthTile());
                    std::uniform_int_distribution<> yDist(helper->shrinkY(), helper->shrinkY()+numDivisions*helper->heightTile()-height);

                    addEnemy(renderer, roomIndex, {xDist(gen),yDist(gen),width,height}, resize, num);
                }
            }
            return true;
        });
    }
}