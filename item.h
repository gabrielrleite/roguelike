#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include "player.h"

using namespace std;

struct Item {
    string name;
    int type; // 1 = cura, 2 = ataque
    int effect;
    int uses;
};

void spawnPotions(int map[MAP_HEIGHT][MAP_WIDTH], int quantity, Player player);
void spawnKey(int map[MAP_HEIGHT][MAP_WIDTH], int lvl, Player player);
void spawnTraps(int map[MAP_HEIGHT][MAP_WIDTH], int quantity, Player player);
void spawnShield(int map[MAP_HEIGHT][MAP_WIDTH], int level, Player player);
void spawnTorch(int map[MAP_HEIGHT][MAP_WIDTH], Player player);

#endif