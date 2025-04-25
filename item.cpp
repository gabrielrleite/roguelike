#include "item.h"
#include <iostream>

void spawnPotions(int map[MAP_HEIGHT][MAP_WIDTH], int quantity, Player player) {
    int count = 0;
    while (count < quantity) {
        int x = rand() % MAP_HEIGHT;
        int y = rand() % MAP_WIDTH;
        if(map[x][y] == 0 && x != player.x && y != player.y) {
            map[x][y] = 3;
            count++;
        }
    }
}

void spawnKey(int map[MAP_HEIGHT][MAP_WIDTH], int lvl, Player player) {
    if(lvl != 3) {
        bool placed = false;
        while (!placed) {
            int x = rand() % MAP_HEIGHT;
            int y = rand() % MAP_WIDTH;
            if (map[x][y] == 0 && x != player.x && y != player.y) {
                map[x][y] = 6;
                placed = true;
            }
        }
    }
    
}

void spawnTraps(int map[MAP_HEIGHT][MAP_WIDTH], int quantity, Player player) {
    int count = 0;
    while (count < quantity) {
        int x = rand() % MAP_HEIGHT;
        int y = rand() % MAP_WIDTH;
        if(map[x][y] == 0 && x != player.x && y != player.y) {
            map[x][y] = 2;
            count++;
        }
    }
}

void spawnShield(int map[MAP_HEIGHT][MAP_WIDTH], int level, Player player) {
    bool placed = false;
    if (level == 1) {
        while (!placed) {
            int x = rand() % MAP_HEIGHT;
            int y = rand() % MAP_WIDTH;
            if (map[x][y] == 0 && x != player.x && y != player.y) {
                map[x][y] = 4;
                placed = true;
            }
        }
    } else {
        int chance = rand() % 100;
        if (chance < 20) {
            while (!placed) {
                int x = rand() % MAP_HEIGHT;
                int y = rand() % MAP_WIDTH;
                if (map[x][y] == 0 && x != player.x && y != player.y) {
                    map[x][y] = 4;
                    placed = true;
                }
            }
        }
    }
}

void spawnTorch(int map[MAP_HEIGHT][MAP_WIDTH], Player player) {
    bool placed = false;
    int chance = rand() % 100;
    if (chance < 50) {
        while (!placed) {
            int x = rand() % MAP_HEIGHT;
            int y = rand() % MAP_WIDTH;
            if (map[x][y] == 0 && x != player.x && y != player.y) {
                map[x][y] = 7;
                placed = true;
            }
        }
    }
}
