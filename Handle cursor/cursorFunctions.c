#include <windows.h>
#include <stdarg.h>

#define KB_HOME     0x47
#define KB_END      0x4F
#define KB_INSERT   0x52
#define KB_DELETE   0x53
#define KB_PAGE_UP      0x49
#define KB_PAGE_DOWN    0x51
#define KB_SETA_ESQUERDA    0x4B
#define KB_SETA_DIREITA     0x4D
#define KB_SETA_CIMA        0x48
#define KB_SETA_BAIXO       0x50

boolean caractereNormal(char c){
    //if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    if((c >= ' ' && c <= '~') || (c >= 128 && c <= 167))
        return TRUE;
    return FALSE;
}

void moveCurXY(int coluna_X, int linha_Y){ // FUNÇÃO RESPONSAVEL POR MUDAR O CURSOR DE POSIÇÃO
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO posAtual;
    GetConsoleScreenBufferInfo(hOut, &posAtual);
    if(posAtual.dwCursorPosition.X + coluna_X < posAtual.dwSize.X){
        posAtual.dwCursorPosition.X += coluna_X;
    }
    if(posAtual.dwCursorPosition.Y + linha_Y < posAtual.dwSize.Y){
        posAtual.dwCursorPosition.Y += linha_Y;
    }
    //previne do cursor ir pra posição -1
    if(posAtual.dwCursorPosition.X < 0) posAtual.dwCursorPosition.X = 0;
    //previne do cursor ir para uma posição acima do tamanho max da tela
    else if(posAtual.dwCursorPosition.X > posAtual.dwSize.X - 1) posAtual.dwCursorPosition.X = posAtual.dwSize.X - 1;

    //previne do cursor ir pra posição -1
    if(posAtual.dwCursorPosition.Y < 0) posAtual.dwCursorPosition.Y = 0;
    else if(posAtual.dwCursorPosition.Y > posAtual.dwSize.Y - 1) posAtual.dwCursorPosition.Y = posAtual.dwSize.Y - 1;

    SetConsoleCursorPosition(hOut, posAtual.dwCursorPosition);
}

void moveCurHome(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO posAtual;
    GetConsoleScreenBufferInfo(hOut, &posAtual);
    posAtual.dwCursorPosition.X = 0;
    SetConsoleCursorPosition(hOut, posAtual.dwCursorPosition);
}

void moveCurPageUp(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO posAtual;
    GetConsoleScreenBufferInfo(hOut, &posAtual);
    posAtual.dwCursorPosition.Y -= (posAtual.dwCursorPosition.Y != posAtual.srWindow.Top)? (posAtual.dwCursorPosition.Y - posAtual.srWindow.Top):(posAtual.srWindow.Bottom - posAtual.dwCursorPosition.Y);
    if(posAtual.dwCursorPosition.Y < 0) posAtual.dwCursorPosition.Y = 0;
    SetConsoleCursorPosition(hOut, posAtual.dwCursorPosition);
}

void moveCurPageDown(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO posAtual;
    GetConsoleScreenBufferInfo(hOut, &posAtual);
    posAtual.dwCursorPosition.Y += (posAtual.dwCursorPosition.Y != posAtual.srWindow.Bottom)? (posAtual.srWindow.Bottom - posAtual.dwCursorPosition.Y):(posAtual.srWindow.Bottom - posAtual.srWindow.Top);
    if(posAtual.dwCursorPosition.Y > posAtual.dwSize.Y-1) posAtual.dwCursorPosition.Y = posAtual.dwSize.Y-1;
    SetConsoleCursorPosition(hOut, posAtual.dwCursorPosition);
}

void moveCurEndPos(int posicao){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO posAtual;
    GetConsoleScreenBufferInfo(hOut, &posAtual);
    posAtual.dwCursorPosition.X = posicao;
    SetConsoleCursorPosition(hOut, posAtual.dwCursorPosition);
}

void moveCurEndSize(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO posAtual;
    GetConsoleScreenBufferInfo(hOut, &posAtual);
    posAtual.dwCursorPosition.X = posAtual.dwSize.X-1;
    SetConsoleCursorPosition(hOut, posAtual.dwCursorPosition);
}

void gotoxy(int coluna_X, int linha_Y){
    COORD POINT;
    POINT.X = coluna_X;
    POINT.Y = linha_Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), POINT);
}

void gotoxy_coord(COORD coord){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Imprime uma entrada formatada(igual ao printf) nas coordenadas passadas e retorna para coordenada antiga
void gotoxyAndBack(int coluna_X, int linha_Y, const char *string, ...){
    POINT point, posAtual;
    GetCursorPos(&posAtual);
    point.x = coluna_X;
    point.y = linha_Y;

    SetCursorPos(point.x, point.y);

    va_list arg;
    va_start(arg, string);
    vprintf(string, arg);
    va_end(arg);

    SetCursorPos(posAtual.x, posAtual.y);
}
