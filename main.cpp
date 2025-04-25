#include <iostream>
#include <windows.h>
#include <ctime>
#include <conio.h>

#include "player.h"
#include "enemy.h"
#include "item.h"
#include "map.h"
#include "menu.h"
#include "util.h"

using namespace std;

#define maxLife 100

int main() {
    srand(time(NULL));
    system("chcp 65001 >nul");

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    COORD coord = {0, 0};
    // VIDA, DANO, DEFESA, X, Y, POÇÕES, ESCUDO, TOCHA, ESPADA, PONTUAÇÃO, CHAVE, INIMIGOS ELIMINADOS, ITENS COLETADOS, TRAPS ACIONADAS, CHAVES COLETADAS
    Player player = {100, 10, 5, 1, 1, 0, false, false, 1, 0, false, 0, 0, 0, 0, false};

    bool visible[MAP_HEIGHT][MAP_WIDTH] = {false};
    bool nevoa = false;

    int menuArr[4][2] = {{0,1}, {0,2}, {0,3}, {0,4}};
    int x = 0, y = 0, option = 10;
    char key;
    int enemyMovementCounter = 0, lvl = 0;

    do {
        if (_kbhit()) {
            key = getch();
            moveMenu(key, x, 4, y, 4, option);
        }
        int lvlChange = true;
        switch (option) {
            case 0: { // JOGAR
                bossLife = 100;
                system("cls");
                updateVisibility(visible, player);
                setEnemies(maps[lvl], player, 7);

                while (player.life > 0 && bossLife > 0) {
                    if(lvlChange) {
                        spawnPotions(maps[lvl], 5, player);
                        spawnTraps(maps[lvl], 5, player);
                        spawnShield(maps[lvl], 1, player);
                        spawnTorch(maps[lvl], player);
                        spawnKey(maps[lvl], lvl, player);
                        lvlChange = false;
                    }

                    if (++enemyMovementCounter >= 100) {
                        for (int i = 0; i < maxEnemies; i++) {
                            if (enemies[i].life > 0) {
                                moveEnemy(enemies[i], maps[lvl], player);
                                int ex = enemies[i].x;
                                int ey = enemies[i].y;
                            }
                        }
                        enemyMovementCounter = 0;
                    }

                    if (_kbhit()) {
                        key = getch();
                        movePlayer(player, maps[lvl], key, nevoa);
                        updateVisibility(visible, player);
                    }

                    printMap(player, maps[lvl], coord, enemies[0], visible, nevoa, out);

                    for (int i = 0; i < maxEnemies; i++) {
                        if (player.x == enemies[i].x && player.y == enemies[i].y && enemies[i].life > 0) {
                            turnBasedBattle(enemies[i], player, player.score, out);
                            break;
                        }
                    }

                    int tile = maps[lvl][player.x][player.y];
                    if (tile == 2) {
                        player.life -= 10;
                        player.score -= 20;
                        cout<<"Você pisou em uma armadilha! Vida restante: "<<player.life<<" (-20 Pontos)                   "<<endl;
                        getch();
                        player.trapTimes++;
                    } else if (tile == 3) {
                        player.life_pot++;
                        player.score += 50;
                        cout<<"Você coletou uma poção! Total: "<<player.life_pot<<" (+50 Pontos)                            "<<endl;
                        getch();
                        maps[lvl][player.x][player.y] = 0;
                        player.collectedItens++;
                    } else if (tile == 4 && !player.shield) {
                        player.shield = true;
                        player.score += 50;
                        cout<<"Você conseguiu um escudo! (+50 Pontos)                                                       "<<endl;
                        getch();
                        maps[lvl][player.x][player.y] = 0;
                        player.collectedItens++;
                    } else if (tile == 6) {
                        player.hasKey = true;
                        player.score += 100;
                        cout<<"Você encontrou a chave! (+100 Pontos)                                                       "<<endl;
                        player.collectedKeys++;
                        getch();
                        maps[lvl][player.x][player.y] = 0;
                    } else if (tile == 5) {
                        if (player.hasKey) {
                            resetItens(maps[lvl]);
                            lvl++;
                            lvlChange = true;
                            player.hasKey = false;
                            resetEnemies(enemies);
                            if(lvl<2) {
                                setEnemies(maps[lvl], player, 7);
                            } else {
                                setEnemies(maps[lvl], player, 3);
                                enemies[0] = {"Boss", 250, 150, 30, 3, 3, true};
                            }
                            for (int i = 0; i < MAP_HEIGHT; ++i) {
                                for (int j = 0; j < MAP_WIDTH; ++j) {
                                    visible[i][j] = false;
                                }
                            }
                        } else {
                            cout<<"Você precisa de uma chave para abrir essa porta!                                        "<<endl;
                            getch();
                        }
                    } else if (tile == 7 && !player.hasTorch) {
                        player.hasTorch = true;
                        player.score += 50;
                        cout<<"Você conseguiu uma tocha! (+50 Pontos)                                                       "<<endl;
                        getch();
                        maps[lvl][player.x][player.y] = 0;
                        player.collectedItens++;
                    }
                }
                if(player.life > 0) {
                    system("cls");
                    cout<<"Parabéns!"<<endl<<endl;
                    cout<<"Você ganhou!"<<endl<<endl;
                    cout<<"Pontuação Detalhada:"<<endl;
                    cout<<"- Inimigos Eliminados ("<<player.killedEnemy<<" inimigos): +"<<player.killedEnemy*100<<" pontos"<<endl;
                    cout<<"- Chaves coletadas ("<<player.collectedKeys<<" chaves): +"<<player.collectedKeys*100<<" pontos"<<endl;
                    cout<<"- Itens Coletados ("<<player.collectedItens<<" itens): +"<<player.collectedItens*50<<" pontos"<<endl;
                    cout<<"- Vida restante ("<<player.life<<" pontos): +"<<(player.life%10)*20<<" pontos"<<endl;
                    cout<<"- Armadilhas acionadas ("<<player.trapTimes<<" armadilhas): -"<<player.trapTimes*20<<" pontos"<<endl;
                    player.score += (player.life%10)*20;
                    cout<<"- Vitória: +"<<player.score<<" pontos"<<endl<<endl;
                    player.score += player.score;
                    cout<<"PONTUAÇÂO FINAL: "<<player.score<<" PONTOS."<<endl<<endl;
                } else {
                    system("cls");
                    cout<<"GAME OVER"<<endl<<endl;
                    cout<<"Você perdeu!"<<endl<<endl;
                    cout<<"Pontuação Detalhada:"<<endl;
                    cout<<"- Inimigos Eliminados ("<<player.killedEnemy<<" inimigos): +"<<player.killedEnemy*100<<" pontos"<<endl;
                    cout<<"- Chaves coletadas ("<<player.collectedKeys<<" chaves): +"<<player.collectedKeys*100<<" pontos"<<endl;
                    cout<<"- Itens Coletados ("<<player.collectedItens<<" itens): +"<<player.collectedItens*50<<" pontos"<<endl;
                    cout<<"- Vida restante ("<<player.life<<" pontos): 0 pontos"<<endl;
                    cout<<"- Armadilhas acionadas ("<<player.trapTimes<<" armadilhas): -"<<player.trapTimes*20<<" pontos"<<endl;
                    cout<<"- Derrota: 0 pontos"<<endl<<endl;
                    cout<<"PONTUAÇÂO FINAL: "<<player.score<<" PONTOS."<<endl<<endl;
                }
                for(int i=0; i<3; i++){
                    resetItens(maps[i]);
                }
                for (int i = 0; i < MAP_HEIGHT; ++i) {
                    for (int j = 0; j < MAP_WIDTH; ++j) {
                        visible[i][j] = false;
                    }
                resetEnemies(enemies);
                }
                player = {maxLife, 10, 5, 1, 1, 0, false, false, 1, 0, false, 0, 0, 0, 0, false};
                lvl = 0;
                option = 10;
                system("pause");
                system("cls");
                break;
            }
            case 1: { // COMO JOGAR
                system("cls");
                while (option != 10) {
                    if (_kbhit()) {
                        key = getch();
                        if (key == 13) {
                            option = 10;
                        }
                        else moveMenu(key, x, 4, y, 5, option);
                    }
                    printHowToPlay(y, coord, out);
                }
                break;
            }
            case 2: { // SOBRE
                system("cls");
                while (option != 10) {
                    if (_kbhit() && getch() == 13) option = 10;
                    printAbout(y, coord, out);
                }
                break;
            }
            case 9: {
                printMenu(x, y, menuArr, coord, out);
                break;
            }
            case 10: {
                system("cls");
                y = 0;
                option = 9;
                break;
            }
        }
    } while (option != 3);

    return 0;
}