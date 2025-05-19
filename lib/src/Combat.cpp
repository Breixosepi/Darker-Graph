#include <Combat.hpp>

void Combat::playerAttack(Player& player, Enemy& enemy) 
{
    if (player.getState() != State::ATTACKING || player.getCurrentFrame() != 2) 
    {
        player.setHasHit(false);
        return;
    }

    if (!player.getHasHit()) 
    {
        SDL_Rect attackHitbox = player.getAttackHitbox();
        SDL_Rect enemyBounds = enemy.getBounds();

        if (SDL_HasIntersection(&attackHitbox, &enemyBounds)) 
        {
            enemy.setState(EnemyState::TAKING_DAMAGE);
            player.setHasHit(true);
            enemyTakeDamage(enemy, 1); 
        }
    }
}

void Combat::enemyAttack(Enemy& enemy, Player& player) 
{
    if (enemy.getState() != EnemyState::ATTACKING || enemy.getCurrentFrame() < 3) 
    {
        enemy.setHasHit(false);
        return;
    }

    if (!enemy.getHasHit()) 
    {
        SDL_Rect attackHitbox = enemy.getAttackHitbox();
        SDL_Rect playerBounds = player.getBounds();

        if (SDL_HasIntersection(&attackHitbox, &playerBounds)) 
        {

            player.setState(State::TAKING_DAMAGE);
            enemy.setHasHit(true);
            player.takeDamage(1);
        }
    }
}

void Combat::playerTakeDamage(Player& player, int damage) 
{
    player.setHealth(player.getHealth() - damage);
    if (player.getHealth() <= 0) 
    {
        player.setState(State::DEAD);
        
    }
    std::cout << "Player health: " << player.getHealth() << std::endl;
}

void Combat::enemyTakeDamage(Enemy& enemy, int damage) 
{
    if(enemy.getState()== EnemyState::DEAD)   
    {
        return;
    }
    enemy.setHealth(enemy.getHealth() - damage);
    if (enemy.getHealth() <= 0) 
    {
        enemy.setState(EnemyState::DEAD);
    }
    std::cout << "Enemy health: " << enemy.getHealth() << std::endl;
}