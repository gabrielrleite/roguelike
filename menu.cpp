#include <iostream>
#include "menu.h"

using namespace std;

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
                y = 0;
                option = x;
                break;
        }
    } else {
        switch(key) {
            case 75: case 'a':
                if(y > 0){y--;} else{y = maxY - 1;}
                system("cls");
                break;
            case 77: case 'd':
                if(y < maxY - 1){y++;} else{y = 0;}
                system("cls");
                break;
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
            cout<<"- Alex Luciano Roesler Rese/Algoritmos e Programação II"<<endl;
            cout<<"Abril/2024"<<endl<<endl;
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
            cout<<" + - Poção de vida"<<endl;
            cout<<" D - Escudo (Coletavel 1 vez por partida)"<<endl;
            cout<<" T - Tocha (Coletavel 1 vez por partida)"<<endl;
            cout<<" K - Chave"<<endl;
            cout<<" I - Inimigo Comum"<<endl;
            cout<<" B - Inimigo Boss"<<endl;
            cout<<" ! - Armadilha"<<endl;
            break;
        case 2:
            cout<<"- NIVEIS DA ESPADA"<<endl<<endl;
            cout<<" Espada Enferrujada (x1.0)"<<endl;
            cout<<" Espada de Ferro (x1.2)"<<endl;
            cout<<" Espada Longa (x1.5)"<<endl;
            cout<<" Espada Rúnica (x1.8)"<<endl;
            cout<<" Espada Lendária (x2.2)"<<endl<<endl;
            cout<<" *A espada sobe de nivel a cada 2 inimigos mortos*"<<endl;
            break;
        case 3:
            cout<<"- PONTUAÇÃO:"<<endl<<endl;
            cout<<" +9999  - Ganhar sem usar itens (Poções, Escudo, etc...)"<<endl;
            cout<<" +100   - Inimigo derrotado"<<endl;
            cout<<" +100   - Chave coletada"<<endl;
            cout<<" +50    - Item coletado"<<endl;
            cout<<" +20    - A cada 10 pontos de vida restantes ao fim"<<endl;
            cout<<" -20    - Cair em armadilha"<<endl;
            break;
        case 4:
            cout<<"- OBJETIVO DO JOGO:"<<endl<<endl;
            cout<<" Explore os níveis da masmorra, colete itens,"<<endl;
            cout<<" derrote inimigos e chegue até o chefe final."<<endl;
            cout<<" Sobreviva até o fim sem perder toda sua vida!"<<endl<<endl;
            cout<<" Dica: Use itens com sabedoria."<<endl;
            break;
    }

    cout<<"\nPágina "<<y+1<<"/5\n";
    cout<<"← e → para navegar | Pressione ENTER para voltar...\n";
}

// EXIBIR MENU INICIAL
void printMenu(int x, int y, int menu[4][2], COORD coord, HANDLE hConsole) {
    SetConsoleCursorPosition(hConsole, coord);
    cout<<"==============================="<<endl;
    cout<<"           ROGUE LIKE          "<<endl;
    cout<<"==============================="<<endl;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 2; j++) {
            if(i == x && j == y) {
                cout<<"->"; // SELEÇÃO
            } else {
                switch (menu[i][j]) {
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
