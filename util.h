#ifndef UTIL_H
#define UTIL_H

#include <windows.h>
#include "player.h"
#include "enemy.h"

using namespace std;

string sword_type(int lvl);
void clearBattleArea(int startY, int lines, HANDLE hConsole);
void turnBasedBattle(Enemy &enemy, Player &player, int &score, HANDLE hConsole);
void sortPos(int map[MAP_HEIGHT][MAP_WIDTH], Player player, int &x, int &y);

#endif