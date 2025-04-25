#include "player.h"
bool cheat = false;
int x, y, temp_life = 0, temp ;

void movePlayer(Player &player, int map[MAP_HEIGHT][MAP_WIDTH], char key, bool &nevoa) {
    x = player.x;
    y = player.y;
    switch(key) {
        case 72: case 'w':
            if(map[x-1][y] != 1) player.x--;
            break;
        case 80: case 's':
            if(map[x+1][y] != 1) player.x++;
            break;
        case 75: case 'a':
            if(map[x][y-1] != 1) player.y--;
            break;
        case 77: case 'd':
            if(map[x][y+1] != 1) player.y++;
            break;
        case 'c':
            cheat = !cheat;
            break;
    }
    if(cheat == true) {
        switch(key) {
            case 'v': // ATIVA/DESATIVA A NÉVOA
                    nevoa = !nevoa;                   
                break;
            case 'g': // ATIVA/DESATIVA "VIDA INFINITA"
                if(player.life <= 100) {
                    temp_life = player.life;
                    player.life = 9999;
                } else {
                    player.life = temp_life;
                }
                break;
            case 'i': // AUMENTA O NÍVEL DA ESPADA
                if(player.lvl_sword < 5) {
                    player.lvl_sword++;
                }
                break;
            case 'u': // REDUZ O NÍVEL DA ESPADA
                if(player.lvl_sword > 1) {
                    player.lvl_sword--;
                }
                break;
            case 'o': // REMOVE POÇÕES
                if(player.life_pot > 0) {
                    player.life_pot--;
                }
                break;
            case 'p': // ADICIONA POÇÕES
                if(player.life_pot < 50) {
                    player.life_pot++;
                }
                break;
            case 't': // ATIVA/DESATIVA TOCHA (+1 RAIO DE VISÃO)
                player.hasTorch = !player.hasTorch;
                break;
        }
    }
}
