#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "cursorFunctions.c"

typedef struct _Line{
    char character;
    struct _Line *prevCharacter;
    struct _Line *nextCharacter;
}Line;

typedef struct _Text{
    Line *linha;
    struct _Text *proxLinha;
}Text;

Text *atualLinha, *primeiraLinha = NULL, *novaLinha;
Line *atualCaracter, *primeiroCaracter = NULL, *novoCaracter;

int main(){
    system("mode con: cols=70 lines=30");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO getCoordenada;

    //GetConsoleScreenBufferInfo(hOut, &getCoordenada);
    //printf("tam x:%i, y:%i", getCoordenada.srWindow.Right, getCoordenada.srWindow.Bottom);
    getCoordenada.dwSize.X = 70;
    getCoordenada.dwSize.Y = 30;
    SetConsoleScreenBufferSize(hOut, getCoordenada.dwSize);
    GetConsoleScreenBufferInfo(hOut, &getCoordenada);
    //printf("tam x:%i, y:%i", getCoordenada.srWindow.Right, getCoordenada.srWindow.Bottom);

    //inicializa a linha
    primeiraLinha = atualLinha = novaLinha = (Text *) malloc(sizeof(Text));
    primeiraLinha->proxLinha = NULL;
    primeiraLinha->linha = NULL;
    char teste;

    char ch = 0;
    while((ch = _getch()) != 27){
        //pega informações do cursor e do prompt
        GetConsoleScreenBufferInfo(hOut, &getCoordenada);
        if(ch == 0 || ch == -32){//teclas especiais
            switch(_getch()){
            case KB_SETA_ESQUERDA:
                moveCurXY(-1,0);
                break;
            case KB_SETA_DIREITA:
                if(TRUE){//verifica até onde o cursor pode andar para a direita
                    Text *verificaLinha = primeiraLinha;
                    for(int x = 0; x < getCoordenada.dwCursorPosition.Y; x++)
                        verificaLinha = verificaLinha->proxLinha;
                    Line *verificaFront = verificaLinha->linha;
                    for(int x = 0; x < getCoordenada.dwCursorPosition.X; x++){
                        if(verificaFront->character == '\n') break;
                        verificaFront = verificaFront->nextCharacter;
                    }
                    if(verificaFront == NULL) break;
                    else if(verificaFront->character != '\n')
                        moveCurXY(1,0);
                }
                break;
            case KB_SETA_CIMA:
                if(getCoordenada.dwCursorPosition.Y != 0){//verifica se já não está no topo
                    Text *verificaLinha = primeiraLinha;
                    for(int x = 0; x < getCoordenada.dwCursorPosition.Y-1; x++){
                        verificaLinha = verificaLinha->proxLinha;
                        if(verificaLinha == NULL) break;
                    }
                    if(verificaLinha == NULL) break;
                    Line *verificaFront = verificaLinha->linha;

                    if(verificaFront != NULL){
                        int caracter = 0;
                        for(int x = 0; x < getCoordenada.dwCursorPosition.X; x++){
                            verificaFront = verificaFront->nextCharacter;
                            caracter++;
                            if(verificaFront == NULL) break;
                            if(verificaFront->character == '\n') break;
                        }
                        getCoordenada.dwCursorPosition.X = caracter;
                        if(getCoordenada.dwCursorPosition.Y > 0)
                            getCoordenada.dwCursorPosition.Y--;
                        gotoxy_coord(getCoordenada.dwCursorPosition);
                    }
                }
                break;
            case KB_SETA_BAIXO:
                if(TRUE){
                    Text *verificaLinha = primeiraLinha;
                    for(int x = 0; x < getCoordenada.dwCursorPosition.Y+1; x++){
                        verificaLinha = verificaLinha->proxLinha;
                        if(verificaLinha == NULL) break;
                    }
                    if(verificaLinha == NULL) break;
                    Line *verificaFront = verificaLinha->linha;

                    if(verificaFront != NULL){
                        int caracter = 0;
                        for(int x = 0; x < getCoordenada.dwCursorPosition.X; x++){
                            verificaFront = verificaFront->nextCharacter;
                            caracter++;
                            if(verificaFront == NULL) break;
                            if(verificaFront->character == '\n') break;
                        }
                        getCoordenada.dwCursorPosition.X = caracter;
                        getCoordenada.dwCursorPosition.Y++;
                        gotoxy_coord(getCoordenada.dwCursorPosition);
                    }
                    else{
                        moveCurHome();
                        moveCurXY(0,1);
                    }
                }
                break;
            case KB_HOME:
                moveCurHome();
                break;
            case KB_END:
                if(TRUE){//verifica qual é o ultimo caracter da linha atual para se posicionar após o ultimo
                    Text *verificaLinha = primeiraLinha;
                    for(int x = 0; x < getCoordenada.dwCursorPosition.Y; x++)
                        verificaLinha = verificaLinha->proxLinha;
                    Line *verificaFront = verificaLinha->linha;

                    //verifica se existe algum caractere na linha atual
                    if(verificaFront != NULL){
                        int caracter = 1;
                        while(verificaFront->nextCharacter != NULL){
                            verificaFront = verificaFront->nextCharacter;
                            caracter++;
                        }
                        //previne de ficar na frente do '\n', decrementando 'caracter' quando encontrado
                        if(verificaFront->character == '\n') caracter--;
                        moveCurEndPos(caracter);
                    }
                }
                break;
            case KB_PAGE_UP:
                //moveCurPageUp();
                break;
            case KB_PAGE_DOWN:
                gotoxyAndBack(10, 10, "Oi, %s bem, que %s te ver", "tudo", "bom");
                //moveCurPageDown();
                break;
            }
        }
        else{
            //aloca memoria para um novo caractere digitado
            novoCaracter = (Line *) malloc(sizeof(Line));

            //verifica se o ultimo caractere digitado(antes do atual) foi um enter, para começar uma nova linha
            if(atualLinha->linha == NULL){
                atualLinha->linha = novoCaracter;
                primeiroCaracter = NULL;
            }

            //se não existir nenhum caracter na linha atual, o ponteiro passa a ser o primeiro
            if(primeiroCaracter == NULL){
                primeiroCaracter = atualCaracter = novoCaracter;
                primeiroCaracter->nextCharacter = NULL;
            }
            else{
                atualCaracter = primeiroCaracter;
                while(atualCaracter->nextCharacter != NULL)
                    atualCaracter = atualCaracter->nextCharacter;
                atualCaracter->nextCharacter = novoCaracter;
                atualCaracter = novoCaracter;
            }

            //se foi pressionado a tecla ENTER é criada uma nova 'Lista' de linha, para criar outra linha
            if(ch == 13){
                atualCaracter->character = '\n';
                atualCaracter->nextCharacter = NULL;
                printf("\n");

                novaLinha = (Text *) malloc(sizeof(Text));
                atualLinha = primeiraLinha;
                while(atualLinha->proxLinha != NULL)
                    atualLinha = atualLinha->proxLinha;
                atualLinha->proxLinha = novaLinha;
                atualLinha = novaLinha;
                atualLinha->linha = NULL;
                atualLinha->proxLinha = NULL;
            }
            else if(caractereNormal(ch)){ //chama a função para verificar se é um caractere "normal", ou seja, previne de ser armazenado as teclas especiais no arquivo como F1, ESC, setas, etc.
                atualCaracter->character = ch;
                atualCaracter->nextCharacter = NULL;
                printf("%c", ch);
            }
            else{
                free(novoCaracter);
                //continue;
            }
        }
    }
    printf("\n - Foi digitado:\n");
    atualLinha = primeiraLinha;

    while(atualLinha->proxLinha != NULL){
        atualCaracter = atualLinha->linha;
        while(atualCaracter->nextCharacter != NULL){
            printf("%c", atualCaracter->character);
            atualCaracter = atualCaracter->nextCharacter;
        }
        printf("%c", atualCaracter->character);
        atualLinha = atualLinha->proxLinha;
    }
    if(atualCaracter != NULL)
        printf("%c", atualCaracter->character);

    return 0;
}
