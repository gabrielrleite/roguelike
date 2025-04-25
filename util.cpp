#include <iostream>
#include <windows.h>
#include <conio.h>
#include "player.h"
#include "enemy.h"

using namespace std;

void sortPos(int map[MAP_HEIGHT][MAP_WIDTH], Player player, int &x, int &y) {
    do {
        x = rand() % MAP_HEIGHT;
        y = rand() % MAP_WIDTH;
    } while (map[x][y] != 0 || (x == player.x && y == player.y));
}

string sword_type(int lvl) {
    string sword_name;
    switch(lvl) {
        case 1:
            sword_name = "Enferrujada (x1.0)        ";
            break;
        case 2:
            sword_name = "de Ferro (x1.2)           ";
            break;
        case 3:
            sword_name = "Longa (x1.5)              ";
            break;
        case 4:
            sword_name = "Rúnica (x1.8)             ";
            break;
        case 5:
            sword_name = "Lendária (x2.2)           ";
            break;
    }
    return sword_name;
}

// SISTEMA ANTIFLICK BATALHA
void clearBattleArea(int startY, int lines, HANDLE hConsole) {
    for (int i = 0; i < lines; ++i) {
        SetConsoleCursorPosition(hConsole, {0, static_cast<SHORT>(startY + i)});
        cout<<string(100, ' ');
    }
}

// SISTEMA DE BATALHA DE TURNOS
void turnBasedBattle(Enemy &enemy, Player &player, int &score, HANDLE hConsole) {
    int turn = 0;
    bool defending = false;
    bool playerActed = false;

    while (player.life > 0 && enemy.life > 0) {
        clearBattleArea(0, 20, hConsole);
        int defenseChance;

        SetConsoleCursorPosition(hConsole, {0, 0});
        cout<<"--- Batalha contra "<<enemy.name<<" ---";
        SetConsoleCursorPosition(hConsole, {0, 1});
        cout<<"Sua vida: "<<player.life<<"         ";
        SetConsoleCursorPosition(hConsole, {0, 2});
        cout<<"Vida do inimigo: "<<enemy.life<<"         ";
        int damage, base, randFactor, multiplier[5] = {10, 12, 15, 18, 22};
        if(turn % 2 == 0) {
            playerActed = false;
            while (!playerActed) {
                clearBattleArea(4, 10, hConsole);
                SetConsoleCursorPosition(hConsole, {0, 4});
                cout<<"Sua vez!";
                SetConsoleCursorPosition(hConsole, {0, 5});
                cout<<"1 - Atacar";
                SetConsoleCursorPosition(hConsole, {0, 6});
                cout<<"2 - Bloquear (Chance de Defender)";
                SetConsoleCursorPosition(hConsole, {0, 7});
                cout<<"3 - Usar item";

                char escolha = getch();

                switch (escolha) {
                    case '1':
                        base = player.base_damage;
                        randFactor = 80 + rand() % 71; // SORTEIO DE DANO
                        
                        damage = (base * multiplier[player.lvl_sword-1] * randFactor) / (100 * 10);
                        enemy.life -= damage;
                        clearBattleArea(10, 2, hConsole);
                        SetConsoleCursorPosition(hConsole, {0, 10});
                        cout<<"Você causou "<<damage<<" de dano!";
                        defending = false;
                        playerActed = true;
                        break;
                    case '2':
                        clearBattleArea(10, 2, hConsole);
                        SetConsoleCursorPosition(hConsole, {0, 10});
                        cout<<"Você está em posição defensiva.";
                        defending = true;
                        playerActed = true;
                        break;
                    case '3':
                        clearBattleArea(10, 4, hConsole);
                        SetConsoleCursorPosition(hConsole, {0, 10});
                        cout<<"Escolha o item: ";
                        SetConsoleCursorPosition(hConsole, {0, 11});
                        cout<<"1 - Poção de Vida (+50) "<<player.life_pot<<" poção(ões) restante(s)";
                        char itemChoice = getch();
                        switch(itemChoice) {
                            case '1':
                                if(player.life_pot > 0) {
                                    player.life = min(100, player.life + 50);
                                    player.life_pot--;
                                    SetConsoleCursorPosition(hConsole, {0, 12});
                                    if(player.life_pot != 1) {
                                        cout<<"Você usou uma poção e recuperou 50 de vida! ("<<player.life_pot<<" poções restantes.)";
                                    } else {
                                        cout<<"Você usou uma poção e recuperou 50 de vida! (1 poção restante.)";
                                    }
                                    playerActed = true;
                                } else {
                                    SetConsoleCursorPosition(hConsole, {0, 12});
                                    cout<<"Você não tem poções."<<endl;
                                    system("pause");
                                }
                                break;
                        }
                        break;
                }
            }
        } else {
            clearBattleArea(10, 2, hConsole);
            SetConsoleCursorPosition(hConsole, {0, 10});
            cout<<"Turno do inimigo!";
            defenseChance = rand() % 100;
            int damage = 1 + rand()%enemy.base_damage;
            int critChance = rand() % 100;
            if(critChance < 30) damage *= 2;
            if(defenseChance < player.base_defense * 5) {
                damage = 0;
            }
            if(defending) {
                int chance = rand() % 100;
                if(chance < 30) damage /= 2;
            }
            player.life -= damage;
            SetConsoleCursorPosition(hConsole, {0, 11});
            if(critChance < 30 && defenseChance > player.base_defense * 5) {
                cout<<enemy.name<<" causou "<<damage<<" de dano! (DANO CRITÍCO)";
            } else if(defenseChance < player.base_defense * 5) {
                cout<<"Você esquivou e o "<<enemy.name<<" causou "<<damage<<" de dano!";
            } else {
                cout<<enemy.name<<" causou "<<damage<<" de dano!";
            }
        }

        SetConsoleCursorPosition(hConsole, {0, 13});
        cout<<"Pressione qualquer tecla para continuar...";
        getch();
        clearBattleArea(13, 2, hConsole);
        turn++;
    }

    clearBattleArea(0, 20, hConsole);
    SetConsoleCursorPosition(hConsole, {0, 0});
    cout<<"--- Batalha contra "<<enemy.name<<" ---";
    SetConsoleCursorPosition(hConsole, {0, 1});
    cout<<"Sua vida: " <<player.life<< "         ";
    SetConsoleCursorPosition(hConsole, {0, 2});
    cout<<"Vida do inimigo: "<<enemy.life<< "         ";

    SetConsoleCursorPosition(hConsole, {0, 4});
    if(player.life <= 0)
        cout<<"Você foi derrotado...";
    else {
        cout<<"Você venceu a batalha!";
        if(enemy.isBoss == true) {
            bossLife = 0;
        }
        score += 100;
        player.killedEnemy++;
        if(player.lvl_sword < 5) {
            player.lvl_sword=1+player.killedEnemy/2;
        }
    }

    SetConsoleCursorPosition(hConsole, {0, 6});
    cout<<"Pressione qualquer tecla para sair da batalha..."<<endl;
    getch();
}
