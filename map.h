#ifndef MAP_H
#define MAP_H

#include <windows.h>
#include "player.h"
#include "enemy.h"

#define MAP_HEIGHT 15
#define MAP_WIDTH 50

extern int map1[MAP_HEIGHT][MAP_WIDTH];
extern int map2[MAP_HEIGHT][MAP_WIDTH];
extern int map3[MAP_HEIGHT][MAP_WIDTH];
extern int (*maps[3])[MAP_WIDTH];

void resetItens(int map[MAP_HEIGHT][MAP_WIDTH]);
void updateVisibility(bool visible[MAP_HEIGHT][MAP_WIDTH], Player player);
void printMap(Player player, int map[MAP_HEIGHT][MAP_WIDTH], COORD coord, Enemy enemy, bool visible[MAP_HEIGHT][MAP_WIDTH], bool nevoa, HANDLE hConsole);

#endif