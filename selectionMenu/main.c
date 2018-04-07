#include <stdio.h>
#include <stdlib.h>
#include "auxfunction.h"

void consoleColorRect(unsigned int coluna_X, unsigned int linha_Y, int weidth, int height, CORES color){
    COORD coord = {coluna_X, linha_Y};
    WORD wFill = color << 4;
    DWORD dwFill = weidth;
    PDWORD pdwFill;
    for(;coord.Y-height < height; coord.Y++)
        FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wFill, dwFill, coord, &pdwFill);
}

int main(){
    consoleColorRect(5,5,40,10, COR_AMARELO);
    gotoxy(5,5);
    _getch();
    printf("oi");
    int op = selectMenu(8, 5, "Oi", "Mano", "Outro", "kkk", NULL);
    gotoxy(15,20);
    printf("select: %i", op);
    return 0;
}
