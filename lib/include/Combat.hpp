#pragma once
#include "Enemy.hpp"

class Combat 
{
public:
    static void playerAttack(Player& player, Enemy& enemy);
    static void enemyAttack(Enemy& enemy, Player& player);
    static void playerTakeDamage(Player& player, int damage);
    static void enemyTakeDamage(Enemy& enemy, int damage);
};