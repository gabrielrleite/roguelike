#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include "player.h"

using namespace std;

const int maxEnemies = 500;

struct Enemy {
    string name;
    int life;
    int max_life;
    int base_damage;
    int x;
    int y;
    bool isBoss = false;
};

extern Enemy enemies[maxEnemies];
extern int bossLife;

void spawnEnemy(int map[MAP_HEIGHT][MAP_WIDTH], Player player, Enemy &enemy);
void setEnemies(int map[MAP_HEIGHT][MAP_WIDTH], Player player, int numberEnemies);
void resetEnemies(Enemy enemies[maxEnemies]);
void moveEnemy(Enemy &enemy, int map[MAP_HEIGHT][MAP_WIDTH], Player player);

#endif