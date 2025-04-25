#ifndef MENU_H
#define MENU_H

#include <windows.h>

void moveMenu(char key, int &x, int maxX, int &y, int maxY, int &option);
void printAbout(int y, COORD coord, HANDLE hConsole);
void printHowToPlay(int y, COORD coord, HANDLE hConsole);
void printMenu(int x, int y, int menu[4][2], COORD coord, HANDLE hConsole);

#endif