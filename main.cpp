#include <iostream>
#include <windows.h>
#include <ctime>
#include <conio.h>

#define MAP_HEIGHT 15
#define MAP_WIDTH 50
#define MAXLIFE 100
#define MAX_ENEMIES 5

using namespace std;

// STRUCTS
struct Enemy {
    string name;
    int life;
    int base_damage;
    int x;
    int y;
    bool isBoss;
};

Enemy enemies[MAX_ENEMIES];
int boss_life = 100;
struct Player {
    int life;
    int base_damage;
    int base_defense;
    int x;
    int y;
    int life_pot;
    bool shield;
    int lvl_sword;
    int score;
    bool hasKey;
    int killedEnemy;
};

struct Item {
    string name;
    int type; // 1 = cura, 2 = ataque
    int effect;
    int uses;
};
// SISTEMA ANTIFLICK BATALHA
void clearBattleArea(int startY, int lines, HANDLE hConsole) {
    for (int i = 0; i < lines; ++i) {
        SetConsoleCursorPosition(hConsole, {0, static_cast<SHORT>(startY + i)});
        cout<<string(100, ' ');
    }
}
Enemy spawnEnemy(int map[MAP_HEIGHT][MAP_WIDTH], Player player, string nome, int vida, int dano) {
    Enemy enemy; //

    enemy.name = nome;
    enemy.life = vida;
    enemy.base_damage = dano;

    bool placed = false;
    while (!placed) {
        int x = rand() % MAP_HEIGHT;
        int y = rand() % MAP_WIDTH;
        if (map[x][y] == 0 && !(x == player.x && y == player.y)) {
            enemy.x = x;
            enemy.y = y;
            placed = true;
        }
    }

    return enemy;
}

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
void spawnKey(int map[MAP_HEIGHT][MAP_WIDTH], Player player) {
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
        // Sempre gera no primeiro nível
        while (!placed) {
            int x = rand() % MAP_HEIGHT;
            int y = rand() % MAP_WIDTH;
            if (map[x][y] == 0 && x != player.x && y != player.y) {
                map[x][y] = 4;
                placed = true;
            }
        }
    } else {
        // Chance de ~20% para spawnar nos outros níveis
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
void resetEnemies(Enemy enemies[MAX_ENEMIES]) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].life = 0;
    }
}

void resetItens(int map[MAP_HEIGHT][MAP_WIDTH]) {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            if (map[i][j] == 2 || map[i][j] == 3 || map[i][j] == 4 || map[i][j] == 6) {
                map[i][j] = 0;
            }
        }
    }
}

string sword_type(int lvl) {
    string sword_name;
    switch(lvl) {
        case 1:
            sword_name = "Enferrujada (x1.0)";
            break;
        case 2:
            sword_name = "de Ferro (x1.2)";
            break;
        case 3:
            sword_name = "Longa (x1.5)";
            break;
        case 4:
            sword_name = "Rúnica (x1.8)";
            break;
        case 5:
            sword_name = "Lendária (x2.2)";
            break;
    }
    return sword_name;
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
                cout<<"Escolha: ";
                SetConsoleCursorPosition(hConsole, {0, 6});
                cout<<"1 - Atacar";
                SetConsoleCursorPosition(hConsole, {0, 7});
                cout<<"2 - Bloquear (Chance de Defender)";
                SetConsoleCursorPosition(hConsole, {0, 8});
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
                        cout<<"1 - Poção de Vida (+20)";
                        char itemChoice = getch();
                        switch(itemChoice) {
                            case '1':
                                if(player.life_pot > 0) {
                                    player.life = min(100, player.life + 20);
                                    player.life_pot--;
                                    SetConsoleCursorPosition(hConsole, {0, 12});
                                    if(player.life_pot > 1 || player.life_pot < 1) {
                                    cout<<"Você usou uma poção e recuperou 20 de vida! ("<<player.life_pot<<" poções restantes.)" ;
                                    } else {
                                    cout<<"Você usou uma poção e recuperou 20 de vida! ("<<player.life_pot<<" poção restante.)";
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
            defending = false;
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
            boss_life = 0;
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
// OCULTAÇÃO DO MAPA (NÉVOA)
void updateVisibility(int x, int y, bool visible[MAP_HEIGHT][MAP_WIDTH]) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = x + dx;
            int ny = y + dy;
            if(nx >= 0 && ny >= 0 && nx < MAP_HEIGHT && ny < MAP_WIDTH) {
                visible[nx][ny] = true;
            }
        }
    }
}
// MOVIMENTAÇÃO DO JOGADOR
void movePlayer(int &x, int &y, int map[MAP_HEIGHT][MAP_WIDTH], char key, bool &cheat) {
    switch(key) {
        case 72: case 'w':  // MOVE PARA CIMA
            if(map[x-1][y] != 1) x--;
            break;
        case 80: case 's':  // MOVE PARA BAIXO
            if(map[x+1][y] != 1) x++;
            break;
        case 75: case 'a':  // MOVE PARA A ESQUERDA
            if(map[x][y-1] != 1) y--;
            break;
        case 77: case 'd':  // MOVE PARA DIREITA
            if(map[x][y+1] != 1) y++;
            break;
        case 'c':   // HABILITA/DESABILITA A NÉVOA
            cheat = !cheat;
            break;
    }
}
// MOVIMENTAÇÃO DO MENU
void moveMenu(char key, int &x, int maxX, int &y, int maxY, int &option) {
    if(option != 0 && option != 1) {
        switch(key) {
                case 72: case 'w':
                    if(x > 0){x--;} else{x = maxX - 1;}
                    break;
                case 80: case 's':
                    if(x < maxX - 1){x++;} else{x = 0;}
                    break;
                case 13:
                    option=x;
                    break;
            }
        } else {
            switch(key) {
                case 75: case 'a':
                    if(y > 0){y--;} else{y = maxY - 1;}
                    break;
                case 77: case 'd':
                    if(y < maxY - 1){y++;} else{y = 0;}
                    break;
        }
    }
}
// MODIFICAR MOVIMENTAÇÃO DOS INIMIGOS
void moveEnemy(Enemy &enemy, int map[MAP_HEIGHT][MAP_WIDTH], int playerX, int playerY) {
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    int dir = rand() % 4;

    int novoX = enemy.x + dx[dir];
    int novoY = enemy.y + dy[dir];

    if (novoX >= 0 && novoX < MAP_HEIGHT && novoY >= 0 && novoY < MAP_WIDTH) {
        if(map[novoX][novoY] != 1 && !(novoX == playerX && novoY == playerY)) {
            enemy.x = novoX;
            enemy.y = novoY;
        }
    }
}
// SOBRE (ADICIONAR INFORMAÇÕES)
void printAbout(int y, COORD coord, HANDLE hConsole) {
    SetConsoleCursorPosition(hConsole, coord);
        switch(y) {
            case 0:
                cout<<"- SOBRE:"<<endl<<endl;
                cout<<"Desenvolvedor:"<<endl;
                cout<<"- Gabriel Rodrigues Leite"<<endl<<endl;
                cout<<"Professor/Disciplina:"<<endl;
                cout<<"- Alex Luciano Roesler Rese/Algoritmos e Programação II"<<endl<<endl;
                cout<<"Pressione ENTER para voltar..."<<endl;
                break;
        }
}
// COMO JOGAR
void printHowToPlay(int y, COORD coord, HANDLE hConsole) {
    SetConsoleCursorPosition(hConsole, coord);
    switch (y) {
        case 0:
            cout<<"- COMANDOS BÁSICOS:"<<endl<<endl;
            cout<<" W - Andar para cima"<<endl;
            cout<<" S - Andar para baixo"<<endl;
            cout<<" A - Andar para a esquerda"<<endl;
            cout<<" D - Andar para a direita"<<endl;
            cout<<" ENTER - Confirmar seleção"<<endl;
            break;
        case 1:
            cout<<"- ICONES:"<<endl<<endl;
            cout<<" +   - Poção de vida"<<endl;
            cout<<" D   - Escudo"<<endl;
            cout<<" K   - Chave"<<endl;
            cout<<" I   - Inimigo Comum"<<endl;
            cout<<" B   - Inimigo Boss"<<endl;
            cout<<" !   - Armadilha"<<endl;
            cout<<" *A espada sobe de nivel a cada 2 inimigos mortos*"<<endl;
            break;
              
        case 2:
            cout<<"- PONTUAÇÃO:"<<endl<<endl;
            cout<<" +100  - Inimigo derrotado"<<endl;
            cout<<" +100  - Chave coletada"<<endl;
            cout<<" +50   - Item coletado"<<endl;
            cout<<" -20   - Cair em armadilha"<<endl;
            cout<<" +20   - Cada ponto de vida restante ao fim"<<endl;
            cout<<" -50   - Vida perdida"<<endl;
            break;
        case 3:
            cout<<"- OBJETIVO DO JOGO:"<<endl<<endl;
            cout<<" Explore os níveis da masmorra, colete itens,"<<endl;
            cout<<" derrote inimigos e chegue até o chefe final."<<endl;
            cout<<" Sobreviva até o fim sem perder toda sua vida!"<<endl<<endl;
            cout<<" Dica: Use itens com sabedoria."<<endl;
            break;
    }

    cout<<"\nPágina "<<y+1<<"/4\n";
    cout<<"← e → para navegar | Pressione ENTER para voltar...\n";
}
// EXIBIR MENU INICIAL
void printMenu(int x, int y, int menu[4][2], COORD coord, HANDLE hConsole) {
    SetConsoleCursorPosition(hConsole, coord);

        for(int i=0;i<4;i++){
            for(int j=0;j<2;j++){
                if(i==x && j==y){
                    cout<<"->"; // SELEÇÃO
                } else {
                    switch (menu[i][j]){
                        case 0: cout<<" "; break;
                        case 1: cout<<"JOGAR    "; break;
                        case 2: cout<<"COMO JOGAR   "; break;
                        case 3: cout<<"SOBRE    "; break;
                        case 4: cout<<"SAIR     "; break;
                    }
                }
            }

            cout<<"\n";
        }
}
// EXIBIR O MAPA
void printMap(int x, int y, int map[MAP_HEIGHT][MAP_WIDTH], COORD coord, Player player, Enemy enemy, bool visible[MAP_HEIGHT][MAP_WIDTH], bool cheat, HANDLE hConsole) {
    SetConsoleCursorPosition(hConsole, coord);
    cout<<"Vida: "<<player.life<<"     Pontuação: "<<player.score<<"     Espada "<< sword_type(player.lvl_sword)<<"                     "<< endl ;
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if(!visible[i][j] && !cheat) {
                cout<<u8"\u2591"; // névoa
            } else if(i == x && j == y) {
                cout<<char(36); // jogador
            } else {
                bool printed = false;
                for (int k = 0; k < MAX_ENEMIES; k++) {
                    if (i == enemies[k].x && j == enemies[k].y && enemies[k].life > 0) {
                        if(enemies[k].isBoss == true) {
                            cout<<"B";
                        } else {
                            cout<<"I";
                        }
                        printed = true;
                        break;
                    }
                }
                if (!printed) {
                    switch (map[i][j]) {
                        case 0: cout<<" "; break;
                        case 1: cout<<u8"\u2588"; break;
                        case 2: cout<<"!"; break;
                        case 3: cout<<"+"; break;
                        case 4: cout<<"D"; break;
                        case 5: cout<<"S"; break;
                        case 6: cout<<"K"; break;
                    }
                }
            }
            
        }
        cout<<"\n";
    }
}

int main() {
    srand(time(NULL));
    system("chcp 65001 >nul"); // DEFINE O CONSOLE EM UTF-8 (CORREÇÃO DE ACENTUAÇÃO)
    //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
    //FIM: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
    //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    short int CX=0, CY=0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;
    //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA

    Player player = {100, 10, 5, 1, 1, 0 , false, 1, false};

    int map1[MAP_HEIGHT][MAP_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    
    int map2[MAP_HEIGHT][MAP_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    
    int map3[MAP_HEIGHT][MAP_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    int (*maps[3])[MAP_WIDTH] = { map1, map2, map3 };

    bool visible[MAP_HEIGHT][MAP_WIDTH] = { false }, cheat = false;

    int menu[4][2]={0,1,
                    0,2,
                    0,3,
                    0,4};


    int x=0, y=0, option=10, dano_aplicado, score;
    char key;

    do {
        if( _kbhit() ){
            key = getch();
            moveMenu(key, x, 4, y, 4, option);
        }
        int enemyMovementCounter = 0;
        Enemy enemy;
        int lvl;
        switch(option) {
            case 0: //JOGAR
                boss_life = 100;
                resetItens(map1);
                resetItens(map2);
                resetItens(map3);
                spawnPotions(map1, 5, player);
                spawnPotions(map2, 5, player);
                spawnPotions(map3, 5, player);
                spawnShield(map1, 0, player);
                spawnShield(map2, 1, player);
                spawnShield(map3, 2, player);
                spawnTraps(map1, 5, player);
                spawnTraps(map1, 5, player);
                spawnTraps(map3, 5, player);
                spawnKey(map1, player);
                spawnKey(map2, player);
                system("cls");
                lvl = 0;
                player.x = 1;
                player.y = 1;
                player.life = MAXLIFE;
                updateVisibility(player.x, player.y, visible);
                player.score = 0;
                enemies[0] = {"Goblin", 50, 25, 19, 10};
                enemies[1] = {"Orc", 70, 30, 5, 35};
                enemies[2] = {"Esqueleto", 40, 20, 7, 7};
                enemies[3] = {"Morcego", 30, 15, 10, 20};
                enemies[4] = {"Bruxa", 60, 28, 12, 45};
                while(player.life>0 && boss_life>0){
                    enemyMovementCounter++;
                    if(enemyMovementCounter >= 100) {
                        for (int i = 0; i < MAX_ENEMIES; i++) {
                            if (enemies[i].life > 0)
                                moveEnemy(enemies[i], maps[lvl], player.x, player.y);
                        }
                        enemyMovementCounter = 0;
                    }
                    int x_ant = player.x;
                    int y_ant = player.y;

                    printMap(player.x, player.y, maps[lvl], coord, player, enemy, visible, cheat, out);

                     if( _kbhit() ){
                        key = getch();
                        x_ant = player.x;
                        y_ant = player.y;
                        movePlayer(player.x, player.y, maps[lvl], key, cheat);
                        updateVisibility(player.x, player.y, visible);
                     } else {
                        dano_aplicado=0;
                     }
                     // Início da batalha se jogador colidir com o enemy
                     for (int i = 0; i < MAX_ENEMIES; i++) {
                        if (player.x == enemies[i].x && player.y == enemies[i].y && enemies[i].life > 0) {
                            turnBasedBattle(enemies[i], player, player.score, out);
                            break;
                        }
                    }
                    
                        if(maps[lvl][player.x][player.y] == 2 && (player.x != x_ant || player.y != y_ant)) {
                            player.life -= 10;
                            cout<<"Você pisou em uma armadilha! Vida restante: "<<player.life<<" (-20 Pontos)                       ";
                            player.score -= 20;
                            if(player.life<=0) {
                                printMap(player.x, player.y, maps[lvl], coord, player, enemy, visible, cheat, out);
                            }
                        }
                        if(maps[lvl][player.x][player.y] == 3) {
                            player.life_pot++;
                            maps[lvl][player.x][player.y] = 0;
                            cout<<"Você coletou uma poção! Total: "<<player.life_pot<<" (+50 Pontos!)                                          ";
                            player.score += 50;
                        }
                        if(maps[lvl][player.x][player.y] == 4 && player.shield == false) {
                            player.shield = true;
                            maps[lvl][player.x][player.y] = 0;
                            player.score += 50;
                            cout<<"Você conseguiu um escudo! (+50 Pontos!)                     ";
                        }
                        if (maps[lvl][player.x][player.y] == 5) {
                            if(player.hasKey == true) {
                                lvl++;
                                player.hasKey = false;
                                resetEnemies(enemies);

                                if(lvl<2) {
                                enemies[0] = {"Goblin", 50, 25, 19, 10, false};
                                enemies[1] = {"Orc", 70, 30, 5, 35, false};
                                enemies[2] = {"Esqueleto", 40, 20, 7, 7, false};
                                enemies[3] = {"Morcego", 30, 15, 10, 20, false};
                                enemies[4] = {"Bruxa", 60, 28, 12, 45, false};
                                } else {
                                    enemies[0] = {"Boss", 100, 30, 3, 3, true};
                                }
                            } else {
                                cout<<"Você precisa de uma chave para abrir essa porta!";
                            }
                        }
                        
                        if (maps[lvl][player.x][player.y] == 6) {
                            maps[lvl][player.x][player.y] = 0;
                            player.score += 100;
                            cout<<"Você encontrou a chave! (+100 Pontos!)                              ";
                            player.hasKey = true;
                        }
                        
                }
                if(player.life <= 0) {
                    for (int i = 0; i < MAP_HEIGHT; ++i)
                        for (int j = 0; j < MAP_WIDTH; ++j)
                            visible[i][j] = false;
                }
                
                option=10;
                system("pause");
                system("cls");
                break;
            case 1: //COMO JOGAR
                system("cls");
                while(option != 10) {
                    if(_kbhit()) {
                        key = getch();
                        moveMenu(key, x, 4, y, 4, option);
                        if(key == 13) {
                            option = 10;
                        }
                        system("cls");
                        printHowToPlay(y, coord, out);
                    }
                    printHowToPlay(y, coord, out);
                }
                break;
            case 2: //SOBRE
                system("cls");
                while(option != 10) {
                    if(_kbhit()) {
                        key = getch();
                        if(key == 13) {
                            option = 10;
                        }
                    }
                    printAbout(y, coord, out);
                }
                break;
            case 9:
                y=0;
                printMenu(x, y, menu, coord, out);
                break;
            case 10:
                system("cls");
                option = 9;
                break;
        }
    } while(option != 3);

    return 0;
}
