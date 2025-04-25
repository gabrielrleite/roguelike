#include "enemy.h"
#include "util.h"
#include <iostream>
#include <windows.h>

using namespace std;

int bossLife = 100;

Enemy enemyTemplates[5] = {
    { "Goblin",    50, 50, 25 },
    { "Orc",       70, 70, 30 },
    { "Esqueleto", 40, 40, 20 },
    { "Morcego",   30, 30, 15 },
    { "Bruxa",     60, 60, 28 }
  };

Enemy enemies[maxEnemies];

void setEnemies(int map[MAP_HEIGHT][MAP_WIDTH], Player player, int numberEnemies) {
    for (int i = 0; i < numberEnemies; ++i) {
        enemies[i] = enemyTemplates[rand() % 5];
        spawnEnemy(map, player, enemies[i]);
    }
}

void spawnEnemy(int map[MAP_HEIGHT][MAP_WIDTH], Player player, Enemy &enemy) {
    int x, y;

    if (enemy.isBoss) {
        if (enemy.x < 0 || enemy.x >= MAP_HEIGHT || enemy.y < 0 || enemy.y >= MAP_WIDTH  || map[enemy.x][enemy.y] != 0) {
            sortPos(map, player, x, y);
            enemy.x = x;
            enemy.y = y;
        }
    }
    else {
        sortPos(map, player, x, y);
        enemy.x = x;
        enemy.y = y;
    }
}

void resetEnemies(Enemy enemies[maxEnemies]) {
    for (int i = 0; i < maxEnemies; i++) {
        enemies[i].life = 0;
    }
}

void moveEnemy(Enemy &enemy, int map[MAP_HEIGHT][MAP_WIDTH], Player player) {
    int oldX = enemy.x;
    int oldY = enemy.y;
    int dx[4] = { -1, 1, 0, 0 };
    int dy[4] = {  0, 0,-1, 1 };
    int dir = rand() % 4;

    int novoX = oldX + dx[dir];
    int novoY = oldY + dy[dir];

    if (novoX >= 0 && novoX < MAP_HEIGHT &&
        novoY >= 0 && novoY < MAP_WIDTH &&
        map[novoX][novoY] != 1 &&
        !(novoX == player.x && novoY == player.y))
    {
        bool overlap = false;
        for (int i = 0; i < maxEnemies; ++i) {
            if (&enemies[i] != &enemy &&
                enemies[i].life > 0 &&
                enemies[i].x == novoX &&
                enemies[i].y == novoY)
            {
                overlap = true;
                break;
            }
        }
        if (!overlap) {
            enemy.x = novoX;
            enemy.y = novoY;
            if (map[novoX][novoY] == 2) {
                enemy.life -= 10;
            }
        }
    }
}

