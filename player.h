// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

#define MAP_HEIGHT 15
#define MAP_WIDTH 50

struct Player { // VIDA, DANO BASE, DEFESA BASE, X, Y, POÇÕES, ESCUDO, TOCHA, LVL ESPADA, PONTUAÇÃO, CHAVE, INIMIGOS ELIMINADOS, ITENS COLETADOS, TRAPS ACIONADAS, CHAVES COLETADAS
    int life = 100;
    int base_damage;
    int base_defense;
    int x = 1;
    int y = 1;
    int life_pot = 0;
    bool shield = false;
    bool hasTorch = false;
    int lvl_sword = 1;
    int score = 0;
    bool hasKey = false;
    int killedEnemy = 0;
    int collectedItens = 0;
    int trapTimes = 0;
    int collectedKeys = 0;
    bool useItens = false;
};

void movePlayer(Player &player, int map[MAP_HEIGHT][MAP_WIDTH], char key, bool &nevoa);

#endif // PLAYER_H