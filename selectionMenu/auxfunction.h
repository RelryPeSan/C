/* Por: Relry PeSan
 * Para fins de auxiliar na programação de programas e games simples
 */

#ifndef _AUXFUNCTION_H_
#define _AUXFUNCTION_H_

#ifndef _STDIO_H_
    #include <stdio.h>
#endif // _STDIO_H_

#ifndef _WINDOWS_H
    #include <windows.h>
#endif // _WINDOWS_H


#define TYPE_CURSOR_INVISIBLE 0x0100
#define TYPE_CURSOR_SOLID 0x0064
#define TYPE_CURSOR_MIDLLE 0x0032
#define TYPE_CURSOR_NORMAL 0x0014
#define TYPE_CURSOR_LINE2 0x0009
#define TYPE_CURSOR_LINE 0x0001

#define seta_C 72
#define seta_B 80
#define seta_D 77
#define seta_E 75

#ifdef __cplusplus
extern "C" {
#endif

//"Declarando" false e true como 0 e 1, respectivamente
typedef enum{
    false, true
}bool;

//Enumerando as cores para ser usadas nas funções CorTexto e CorFundo
typedef enum _cor{
   COR_PRETO,
   COR_AZUL,
   COR_VERDE,
   COR_CIANO,
   COR_VERMELHO,
   COR_MAGENTA,
   COR_MARROM,
   COR_CINZA_CLARO,
   COR_CINZA_ESCURO,
   COR_AZUL_CLARO,
   COR_VERDE_CLARO,
   COR_CIANO_CLARO,
   COR_VERMELHO_CLARO,
   COR_ROSA,
   COR_AMARELO,
   COR_BRANCO
}CORES;

//Define a cor dos texto
//OBS.: Caracteres existentes antes de chamar esta função não são afetadas
void colorText(CORES color){ // FUNÇÃO RESPONSAVEL POR SETAR A COR DAS LETRAS
   CONSOLE_SCREEN_BUFFER_INFO csbi;

   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),(csbi.wAttributes & 0xf0) | color);
}

//Define a cor do fundo dos caracteres
//OBS.: fundos existentes antes de chamar esta função não são afetados
void colorBackGround(int color){ // FUNÇÃO RESPONSAVEL POR SETAR A COR DO FUNDO
   CONSOLE_SCREEN_BUFFER_INFO csbi;

   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),(csbi.wAttributes & 0x0f) | (color << 4));
}

//muda o tipo do cursor
void typeCursor(int type_cursor){
    CONSOLE_CURSOR_INFO conCurInfo;
    conCurInfo.bVisible = type_cursor ^ 0x0100;
    conCurInfo.dwSize = (type_cursor % 0x65);

    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conCurInfo);
}

void gotoxy(unsigned int coluna_X, unsigned int linha_Y){
    COORD coord = {coluna_X, linha_Y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Esta função retorna um inteiro referente a uma das opcao citadas nas strings
//Esta função tem tamanho de argumentos variaveis assim como printf
//OBS.: ATENÇÃO!!! SEMPRE QUANDO TERMINAR DE DECLARAR TODOS OS PARAMETROS STRINGS DECLARE UM ULTIMO COMO "NULL" - SEM ASPAS
int selectMenu(unsigned int coluna_X, unsigned int linha_Y, char *string, ...){
    unsigned int linhaMin = linha_Y, linhaMax, tmp = 0;
    va_list arg;

    gotoxy(coluna_X+3, linha_Y++);
    printf("%s", string);
    va_start(arg, string);

    tmp = va_arg(arg, char*);

    while(tmp != NULL){
        gotoxy(coluna_X+3, linha_Y++);
        printf("%s", tmp);
        tmp = va_arg(arg, char*);
    }
    va_end(arg);
    linhaMax = linha_Y-1;
    linha_Y = linhaMin;

    //apaga vestigios que podem haver de antigas funções
    for(int x = 0; x <= linhaMax-linhaMin; x++){
        COORD coord = {coluna_X, linha_Y + x};
        WriteConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), "  ", 2, coord, NULL);
    }
    COORD coord = {coluna_X, linha_Y};
    WriteConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), "=>", 2, coord, NULL);
    typeCursor(TYPE_CURSOR_INVISIBLE);
    while(TRUE){
        switch(_getch()){
        case seta_C:
            if(coord.Y > linhaMin){
                WriteConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), "  ", 2, coord, NULL);
                coord.Y--;
                WriteConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), "=>", 2, coord, NULL);
            }
            break;
        case seta_B:
            if(coord.Y < linhaMax){
                WriteConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), "  ", 2, coord, NULL);
                coord.Y++;
                WriteConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), "=>", 2, coord, NULL);
            }
            break;
        case 27://caso pressionar ESC
            return 0;
        case 13://caso pressionar Enter retorna a opção
            return (coord.Y - linhaMin + 1);
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif /* _AUXFUNCTION_H_ */
