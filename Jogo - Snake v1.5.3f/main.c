#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h> // time()
#include <conio.h> // getch(), kbhit()
#include <stdarg.h> // Criar função com parametros variaveis
#include <string.h> // funções de strings

// Definindo os cantos limites da tela
#define canteiro_EB 187     // -,
#define canteiro_EC 188     // -'
#define canteiro_CD 200     //  '-
#define canteiro_BD 201     //  ,-
#define canteiro_V 186      //  |
#define canteiro_H 205      // ---
#define canteiro_CBD 204    //  |-
#define canteiro_ECBD 206   //  +
#define canteiro_ECB 185    // -|
#define canteiro_ECD 202    // -'-
#define canteiro_EBD 203    // -,-

#define corpo 219   // Corpo do personagem
#define coracao 3

#define seta_C 72
#define seta_B 80
#define seta_D 77
#define seta_E 75

const char versao[] = "Beta v. 1.5.3";

typedef enum{
    false, true
}bool;

typedef struct{ // esse struct vai definir a localização de um "bit"(uma parte do corpo) do personagem ou objeto
    int local_X; // local x da tela
    int local_Y; // local y da tela
}local_snake;

typedef struct{ // esse struct vai definir a localização de um "bit"(uma parte do corpo) do personagem ou objeto
    int local_X; // local x da tela
    int local_Y; // local y da tela
    bool existe; // usado somente para obj tipo 'paredes'
}obj;

typedef struct{
    int local_X;
    int local_Y;
    int cor;    //Definindo a cor da comida para um bonus no modo Versus
    bool existe; // usado somente para comida tipo 'food' Modo Versus
    bool vida;
}comida;

typedef struct{ // esse struct vai pegar o score dos jogadores no arquivo
    char nome[6];
    int score;
}pont;

// Prótotipo
void fase();
bool hit_wall();
void cabecalho();

void textcolor(int newcolor){ // FUNÇÃO RESPONSAVEL POR SETAR A COR PARA LETRAS
   CONSOLE_SCREEN_BUFFER_INFO csbi;

   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),(csbi.wAttributes & 0xf0) | newcolor);
}

void gotoxy(int linha_X, int coluna_Y){ // FUNÇÃO RESPONSAVEL POR MUDAR O CURSOR DE POSIÇÃO
    coluna_Y = coluna_Y == -1 ? 78/2 : coluna_Y;
    COORD POINT;
    POINT.X = coluna_Y;
    POINT.Y = linha_X;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), POINT);
}

void HideCursor(bool estado){ // FUNÇÃO RESPONSAVEL POR ESCONDER O CURSOR PISCANTE DA TELA
    HANDLE hOut;
    CONSOLE_CURSOR_INFO ConCurInf;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if ( estado == true ) { // Quando a função tem o parametro 1/true ela esconde o cursor
        ConCurInf.dwSize = 10;
        ConCurInf.bVisible = FALSE;
        SetConsoleCursorInfo(hOut, &ConCurInf);
    } else { // Caso contrario faz aparecer
        ConCurInf.dwSize = 10;
        ConCurInf.bVisible = TRUE;
        SetConsoleCursorInfo(hOut, &ConCurInf);
    }
}

int main(){
    system("title Snake");
    system("mode con:cols=80 lines=30");
    system("color 07");

    // Evitando Erros quando inicia o programa sem um arquivo "score.sco"
    if (1) {
        FILE *arqScore;
        arqScore = fopen("score.sco", "r");
        if ( arqScore == NULL ) {
            arqScore = fopen("score.sco", "w");
            for (int x = 0; x < 10; x++) {
                fprintf(arqScore, "AAAAA 0\n");
            }
        }
        fclose(arqScore);
    }

    //variaveis auxiliar de programa
    int tecla, teclaAntiga;
    int opcao = 1, opcaoMenu = 1;

    do {
        textcolor(7);
        cabecalho(4,-1, "Menu"); // usando -1 centraliza o titulo
        rodape();
        do {
            opcaoMenu = escolha(34, 7, "Jogar", "Score", "Versus","Sair", NULL);
        } while ( opcaoMenu == 0 ); // enquanto precionar ESC na função escolha fica no loop pois não tem opção
        if ( opcaoMenu == 1 ) {
            do {
                // variaveis dos objetos e do jogo
                const int velocidade_const = 600;
                int tamanho_snake = 3, tamanho_limite = 35, velocidade, nivel_velocidade = 5, score = 0;
                long int tempo;
                bool food_campo = false; // essa var bool identifica se tem comida no campo ou se já foi comida
                bool morreu = false;

                int nivel_fase; // var nivel da fase, se 0 modo Classico
                do {
                    cabecalho(4,-1,"Escolha o modo de jogo");
                    gotoxy(20,8);
                    printf("Pressione ESC para voltar.");
                    rodape();
                    nivel_fase = escolha(33,7,"Classico","Campanha",NULL) - 1;
                    if ( nivel_fase == -1 ) {
                        opcaoMenu = 0; // ESC pressionado
                        break;
                    }
                    char titulo_modo[9];
                    if ( nivel_fase == 0 ) strcpy(titulo_modo, "Classico");
                    else if ( nivel_fase == 1 ) strcpy(titulo_modo, "Campanha");

                    HideCursor(true); // esconde cursor

                    cabecalho(4,-1, &titulo_modo);
                    rodape();
                    gotoxy(6,25);
                    printf("Defina o nivel de velocidade ");
                    gotoxy(7,25);
                    printf("Nivel:");
                    gotoxy(9,25);
                    printf("PRONTO!");
                    gotoxy(20,8);
                    printf("Pressione ESC para voltar.");

                    nivel_velocidade = 3;
                    opcao = 1;
                    do{
                        gotoxy(7,23);
                        printf("  ");
                        gotoxy(9,23);
                        printf("  ");

                        gotoxy(7,32);
                        printf("%2i", nivel_velocidade);

                        if (opcao == 1) {
                            gotoxy(7,23);
                            printf("=>");
                            gotoxy(7,34);

                            for ( int x = 1; x <= nivel_velocidade; x++ ){
                                printf("%c%c", 219, 219);
                            }
                            for ( int x = 1; x <= 10-nivel_velocidade; x++){
                                printf("--");
                            }
                        }
                        else if (opcao == 2) {
                            gotoxy(9,23);
                            printf("=>");
                        }

                        switch ( tecla = getch() ){
                        case 77: //direita
                            if ( opcao == 1 )
                                if (nivel_velocidade < 10) nivel_velocidade++;
                            break;
                        case 75: //esquerda
                            if ( opcao == 1 )
                                if (nivel_velocidade > 1) nivel_velocidade--;
                            break;
                        case 80: //baixo
                            if ( opcao < 2 ) opcao++;
                            break;
                        case 72: //cima
                            if ( opcao > 1 ) opcao--;
                            break;
                        case 27: // ESC
                            break;
                        case 13: //enter
                            opcao++;
                            break;
                        }
                    }while( (tecla != 13 || opcao <= 2) && tecla != 27 );

                } while ( tecla == 27 );
                if (opcaoMenu == 0) break;

                if ( nivel_fase == 0 ) tamanho_limite = 2000; //snake com tamanho "ilimitado"
                obj snake[tamanho_limite], food; // criando a variavel cobra com limite do tamanho do corpo e a var comida
                for ( int i = 0; i < tamanho_limite; i++) { // setando as localizações iniciais da snake para não interferir no programa
                    snake[i].local_X = 0;
                    snake[i].local_Y = 0;
                }

                opcao = 1;
                do {
                    system("cls");
                    gotoxy(8,31);
                    printf("Nivel %d!", nivel_fase);
                    getch();
                    fase(nivel_fase);
                    tamanho_snake = 3;

                    for (int i = 0; i < tamanho_snake; i++) {
                        snake[i].local_X = 12;
                        snake[i].local_Y = 33-i;
                        gotoxy(snake[i].local_X, snake[i].local_Y);
                        printf("%c", corpo);
                    }
                    gotoxy(26,0);
                    printf("SCORE: %04i", score);
                    rodape();

                    getch();

                    //definindo a velocidade para variavel velocidade_inicial conforme o nivel informado anterior
                    const int velocidade_inicial = velocidade_const/nivel_velocidade;
                    tempo = GetTickCount();
                    tecla = 77;
                    food_campo = false;
                    morreu = false;
                    while ( tamanho_snake <= tamanho_limite && !morreu ) {
                        gotoxy(0,0);

                        if ( food_campo == false ){ // verifica quando nao tem comida no campo
                            srand(time(NULL));
                            while ( 1 ){
                                food.local_X = rand()%25;
                                food.local_Y = rand()%80;

                                // este for identifica se a comida não foi gerada em cima do corpo da snake, evitando bugs
                                for (int x = 0; x < tamanho_snake; x++) {
                                    if (food.local_X == snake[x].local_X && food.local_Y == snake[x].local_Y)
                                        continue; // se esta em cima volta no while
                                }
                                if (hit_wall(food.local_X, food.local_Y) == true)
                                    continue; // se esta em cima de alguma pareda
                                break; // senão para o while e continua o programa
                            }
                            gotoxy(food.local_X,food.local_Y);
                            if (tamanho_snake < tamanho_limite) printf("*");
                            else printf("%c", 127);
                            food_campo = true;
                        }else if ( (food.local_X == snake[0].local_X) && (food.local_Y == snake[0].local_Y) ) { //identifica se "comeu"
                            tamanho_snake++;
                            score += 1*nivel_velocidade;
                            gotoxy(26,7);
                            printf("%04i", score);
                            food_campo = false;
                        }

                        if (kbhit()){ // verifica se alguma tecla foi pressionada
                            teclaAntiga = tecla;
                            tecla = getch();
                            fflush(stdin);

                            if (tecla == 224) { // Caso for tecla especial pressionada
                                switch(tecla = getch()) {
                                case seta_C: // seta para cima
                                    if (snake[0].local_X-1 == snake[1].local_X && snake[0].local_Y == snake[1].local_Y) tecla = teclaAntiga;
                                    if (teclaAntiga == 80) tecla = 80;
                                    break;
                                case seta_B: // seta para baixo
                                    if (snake[0].local_X+1 == snake[1].local_X && snake[0].local_Y == snake[1].local_Y) tecla = teclaAntiga;
                                    if (teclaAntiga == 72) tecla = 72;
                                    break;
                                case seta_E: // seta pra esquerda
                                    if (snake[0].local_X == snake[1].local_X && snake[0].local_Y-1 == snake[1].local_Y) tecla = teclaAntiga;
                                    if (teclaAntiga == 77) tecla = 77;
                                    break;
                                case seta_D: // seta para direita
                                    if (snake[0].local_X == snake[1].local_X && snake[0].local_Y+1 == snake[1].local_Y) tecla = teclaAntiga;
                                    if (teclaAntiga == 75) tecla = 75;
                                    break;
                                default:
                                    tecla = teclaAntiga;
                                    break;
                                }
                            }
                            else{ // Caso for tecla comum pressionada
                                switch(tecla){
                                case 27: // ESC - PAUSE do jogo
                                    gotoxy(12,35);
                                    printf("PAUSE!");
                                    getch();
                                    gotoxy(12,35);
                                    printf("      ");
                                    for ( int x = 0; x < tamanho_snake; x++ ) { //Recolocando a snake na tela
                                        gotoxy(snake[x].local_X,snake[x].local_Y);
                                        printf("%c", corpo);
                                    }
                                    gotoxy(food.local_X,food.local_Y);//Recolocando a comida na tela
                                    if (tamanho_snake < tamanho_limite) printf("*");
                                    else printf("%c", 127);
                                    tecla = teclaAntiga;
                                    break;
                                case 32: // space
                                    velocidade = velocidade_inicial/(nivel_velocidade*2);
                                    tecla = teclaAntiga;
                                    break;
                                default:
                                    tecla = teclaAntiga;
                                    velocidade = velocidade_inicial;
                                    break;
                                }
                            }
                        }else velocidade = velocidade_inicial;

                        if (GetTickCount() - tempo > velocidade) {
                            for (int x = tamanho_snake; x > 0; x--){ // repassando os antigos lugares do "corpo" para frente (andar)
                                snake[x].local_Y = snake[x-1].local_Y;
                                snake[x].local_X = snake[x-1].local_X;
                            }

                            gotoxy(snake[tamanho_snake].local_X,snake[tamanho_snake].local_Y); // apaga o ultimo lugar do corpo (rabo)
                            printf(" ");

                            switch(tecla){
                            case 72:
                                if (--snake[0].local_X < 0) snake[0].local_X = 24; // cima
                                break;
                            case 80:
                                if (++snake[0].local_X > 24) snake[0].local_X = 0; // baixo
                                break;
                            case 75:
                                if (--snake[0].local_Y < 0) snake[0].local_Y = 79; // esquerda
                                break;
                            case 77:
                                if (++snake[0].local_Y > 79) snake[0].local_Y = 0; // direita
                                break;
                            default: break;
                            }
                            gotoxy(snake[0].local_X,snake[0].local_Y);
                            printf("%c", corpo);

                            for (int x = 1; x < tamanho_snake; x++) { // verifica se o personagem bateu nele mesmo
                                if ( snake[0].local_X == snake[x].local_X && snake[0].local_Y == snake[x].local_Y ) {
                                    morreu = true;
                                    break;
                                }
                            }
                            if ( hit_wall(snake[0].local_X, snake[0].local_Y) == true && nivel_fase != 0 ) { // verifica se o personagem bateu na parede
                                morreu = true;
                                break;
                            }
                            tempo = GetTickCount(); // esta função retorna em milissegundos desde a ultima vez q a maquina foi iniciada
                        }
                    }

                    if (morreu == true) {
                        if ( nivel_fase != 0 ) nivel_fase = 1;

                        gotoxy(snake[0].local_X,snake[0].local_Y);
                        printf("%c", 177); // destaca o lugar onde bateu
                        gotoxy(8,31);
                        printf("Fim de jogo!!");
                        gotoxy(9,32);
                        printf("Score: %04i", score);

                        char nome[5];
                        for (int x = 0; x < 5; x++) nome[x] = "";
                        do {
                            strcpy(nome, sel_nome(11,-1));
                            gotoxy(16,29);
                            printf("Este nome mesmo?");
                            opcao = escolha(33,18, "Sim", "Nao", NULL);
                            gotoxy(16,29);
                            printf("                ");
                            gotoxy(18,33);
                            printf("      ");
                            gotoxy(19,33);
                            printf("      ");
                        } while (opcao == 2);
                        gravar_score(nome, score);

                        score = 0; // zera o contador score

                        gotoxy(16,29);
                        printf("Deseja reiniciar?");
                        opcao = escolha(33,18, "Sim", "Nao", NULL);
                    }
                    else {
                        gotoxy(8,31);
                        printf("Nivel %d finalizado!", nivel_fase);
                        nivel_fase++;
                    }
                } while ( nivel_fase <= 3 && opcao != 2 );
                if (nivel_fase > 3) {
                    gotoxy(15,31);
                    printf("Voce venceu!");
                    char nome[5];
                    strcpy(nome, sel_nome(11,-1));
                    gravar_score(nome, score);
                    score = 0; // zera o contador score
                }
            } while ( opcao != 2 ); // confere se o jogador escolheu opcao 2 = Nao para encerrar o programa
        }
        else if ( opcaoMenu == 2 ) {
            cabecalho(4,-1,"Score");
            rodape();
            ver_score();
            gotoxy(20,8);
            printf("Pressione qualquer tecla para voltar.");
            getch();
        }
        else if (opcaoMenu == 3) {
            int cor_P1 = 1, cor_P2 = 1;
            char nome_P1[6], nome_P2[6];
            bool confirm_P1 = false, confirm_P2 = false;

            cabecalho(4,-1,"Versus");
            rodape();
            gotoxy(6,31);
            printf("Selecione sua cor");
            gotoxy(8,14);
            printf("PLAYER 1");
            gotoxy(8,54);
            printf("PLAYER 2");
            gotoxy(20,8);
            printf("Pressione ESC para voltar.");
            do {
                gotoxy(10,16);
                textcolor(cor_P1);
                printf("%c%c%c%c", corpo, corpo, corpo, corpo);

                gotoxy(10,56);
                textcolor(cor_P2);
                printf("%c%c%c%c", corpo, corpo, corpo, corpo);

                tecla = getch();
                if(tecla == 224) {
                    switch(getch()){
                    case seta_C:
                        if(!confirm_P2)
                            if(--cor_P2 < 1) cor_P2 = 15;
                        break;
                    case seta_B:
                        if(!confirm_P2)
                            if(++cor_P2 > 15) cor_P2 = 1;
                        break;
                    case seta_D:
                    case seta_E:
                        srand(time(NULL));
                        do{
                            cor_P2 = rand()%15+1;
                        }while(cor_P2 == cor_P1);
                        break;
                    }
                }
                else{
                    switch(tecla){
                    case 13: // Confirmação do PLAYER 2
                        confirm_P2 = !confirm_P2;
                        break;
                    case 32: // Confirmação do PLAYER 1
                        confirm_P1 = !confirm_P1;
                        break;
                    case 'w':
                    case 'W':
                        if(!confirm_P1)
                            if(--cor_P1 < 1) cor_P1 = 15;
                        break;
                    case 's':
                    case 'S':
                        if(!confirm_P1)
                            if(++cor_P1 > 15) cor_P1 = 1;
                        break;
                    case 'a':
                    case 'A':
                    case 'd':
                    case 'D':
                        srand(time(NULL));
                        do{
                            cor_P1 = rand()%15+1;
                        }while(cor_P1 == cor_P2);
                        break;
                    }
                }

                textcolor(7);
                if(confirm_P1 && confirm_P2){
                    if(cor_P1 == cor_P2){
                        gotoxy(15,34);
                        printf("Cores iguais!");
                        getch();
                        gotoxy(15,34);
                        printf("             ");
                        gotoxy(10,20);
                        printf("  ");
                        confirm_P1 = false;
                        gotoxy(10,60);
                        printf("  ");
                        confirm_P2 = false;
                        continue;
                    }
                }

                gotoxy(10,20);
                if(confirm_P1) printf("OK");
                else printf("  ");

                gotoxy(10,60);
                if(confirm_P2) printf("OK");
                else printf("  ");
            }while( (!confirm_P1 || !confirm_P2) && tecla != 27);

            if(tecla != 27){
                strcpy(nome_P1, sel_nome(16,16));
                strcpy(nome_P2, sel_nome(16,56));

                system("cls");
                gotoxy(14,31);
                printf("%s vs %s", nome_P1, nome_P2);
                HideCursor(true);
                getch();
                fase(0);
                rodape();

                const int tamanho_limite = 1000;
                local_snake snake_P1[tamanho_limite], snake_P2[tamanho_limite];
                int tam_snake_P1 = 3, tam_snake_P2 = 3, score_P1 = 0, score_P2 = 0;

                for (int x = 0; x < tamanho_limite; x++) { // Setando variaveis para não interferir
                    snake_P1[x].local_X = -1;
                    snake_P1[x].local_Y = -1;
                    snake_P2[x].local_X = -1;
                    snake_P2[x].local_Y = -1;
                }

                for (int i = 0; i < tam_snake_P1; i++) {
                    snake_P1[i].local_X = 5; // Colocando PLAYER 1 no jogo
                    snake_P1[i].local_Y = 19-i;
                    gotoxy(snake_P1[i].local_X, snake_P1[i].local_Y);
                    textcolor(cor_P1);
                    printf("%c", corpo);

                    snake_P2[i].local_X = 17; // Colocando PLAYER 2 no jogo
                    snake_P2[i].local_Y = 59+i;
                    gotoxy(snake_P2[i].local_X, snake_P2[i].local_Y);
                    textcolor(cor_P2);
                    printf("%c", corpo);
                }

                //Variaveis que auxiliaram no programa do jogo
                const int velocidade_inicial = 50;
                long int tempo_P1 = GetTickCount(), tempo_P2 = GetTickCount();
                int tecla_P1 = 'd', tecla_P2 = seta_E;
                int teclaAntiga_P1 = tecla_P1, teclaAntiga_P2 = tecla_P2;
                bool morreu_P1 = false, morreu_P2 = false;
                long int tempo_renascer_P1, tempo_renascer_P2;
                int vida_P1 = 5, vida_P2 = 5;

                const int maxFood = 35;
                int food_campo = 0;
                long int tempo_food = 0;
                comida food[35];   // Max de comida no campo

                for(int x = 0; x < maxFood; x++) {
                    food[x].local_X = -1;
                    food[x].local_Y = -1;
                    food[x].existe = false;
                }

                textcolor(cor_P1);
                gotoxy(26,1);
                printf("SCORE: %04i", score_P1);
                gotoxy(27,1);
                printf("VIDAS: %i", vida_P1);
                textcolor(cor_P2);
                gotoxy(26,68);
                printf("SCORE: %04i", score_P2);
                gotoxy(27,68);
                printf("VIDAS: %i", vida_P2);

                // COMEÇANDO JOGO -------------------------------------------------------------------------------------------------
                while (vida_P1 >= 0 && vida_P2 >= 0) {
                    // Renascendo PLAYER 1 ----------------------------------------------------------------------------------------
                    if(morreu_P1){
                        if(GetTickCount() - tempo_renascer_P1 > 5000){
                            textcolor(cor_P1);
                            gotoxy(27,8);
                            printf("%i", vida_P1);
                            tam_snake_P1 = 3;
                            bool emcima = true;

                            // Verificando se o renascimento não foi gerado em cima do outro player
                            while(emcima) {
                                static int x = 1;
                                emcima = false;

                                srand(time(NULL) + x);
                                snake_P1[0].local_X = rand()%22 + 3;
                                snake_P1[0].local_Y = rand()%77 + 3;

                                for(int x = 1; x < tam_snake_P1; x++) { //definindo local da snake
                                    snake_P1[x].local_X = snake_P1[0].local_X;
                                    snake_P1[x].local_Y = snake_P1[0].local_Y - x;
                                }

                                for(int p1 = 0; p1 < tam_snake_P1; p1++) { // verifica se não esta em cima do outro player
                                    for(int p2 = 0; p2 < tam_snake_P2; p2++) {
                                        if(snake_P1[p1].local_X == snake_P2[p2].local_X && snake_P1[p1].local_Y == snake_P2[p2].local_Y){
                                            emcima = true;
                                            break;
                                        }
                                    }
                                    for(int comida = 0; comida < maxFood; comida++){
                                        if(snake_P1[p1].local_X == food[comida].local_X && snake_P1[p1].local_Y == food[comida].local_Y && food[comida].existe){
                                            emcima = true;
                                            break;
                                        }
                                    }
                                    if(emcima) break;
                                }
                                x += rand() % 100 + 1;
                            }

                            for(int x = 0; x < tam_snake_P1; x++) {
                                gotoxy(snake_P1[x].local_X,snake_P1[x].local_Y);
                                printf("%c", corpo);
                            }

                            teclaAntiga_P1 = 'd';
                            tecla_P1 = teclaAntiga_P1;
                            morreu_P1 = false;
                        }
                    }

                    // Renascendo PLAYER 2 ----------------------------------------------------------------------------------------
                    if(morreu_P2){
                        if(GetTickCount() - tempo_renascer_P2 > 5000){
                            textcolor(cor_P2);
                            gotoxy(27,75);
                            printf("%i", vida_P2);
                            tam_snake_P2 = 3;
                            bool emcima = true;

                            // Verificando se o renascimento não foi gerado em cima do outro player
                            while(emcima) {
                                static int x = 1;
                                emcima = false;

                                srand(time(NULL) + x);
                                srand(time(NULL));
                                snake_P2[0].local_X = rand()%22 + 3;
                                snake_P2[0].local_Y = rand()%77 + 3;

                                for(int x = 1; x < tam_snake_P2; x++) {
                                    snake_P2[x].local_X = snake_P2[0].local_X;
                                    snake_P2[x].local_Y = snake_P2[0].local_Y + x;
                                }

                                for(int p2 = 0; p2 < tam_snake_P2; p2++) {
                                    for(int p1 = 0; p1 < tam_snake_P1; p1++) {
                                        if(snake_P1[p1].local_X == snake_P2[p2].local_X && snake_P1[p1].local_Y == snake_P2[p2].local_Y){
                                            emcima = true;
                                            break;
                                        }
                                    }
                                    for(int comida = 0; comida < maxFood; comida++){
                                        if(snake_P2[p2].local_X == food[comida].local_X && snake_P2[p2].local_Y == food[comida].local_Y && food[comida].existe){
                                            emcima = true;
                                            break;
                                        }
                                    }
                                    if(emcima) break;
                                }
                                x += rand() % 100 + 1;
                            }

                            for(int x = 0; x < tam_snake_P2; x++) {
                                gotoxy(snake_P2[x].local_X,snake_P2[x].local_Y);
                                printf("%c", corpo);
                            }

                            teclaAntiga_P2 = seta_E;
                            tecla_P2 = teclaAntiga_P2;
                            morreu_P2 = false;
                        }
                    }

                    // COMIDA --------------------------------------------------------------------------------------------------------
                    if(GetTickCount() - tempo_food > 2500 && food_campo < 20) {
                        srand(time(NULL));
                        for(int a = 0; a < maxFood; a++){
                            if(food[a].existe == false){
                                bool comida_sobrepondo = true;
                                while (comida_sobrepondo) {
                                    comida_sobrepondo = false;
                                    food[a].local_X = rand()%25;
                                    food[a].local_Y = rand()%80;

                                    if(!(rand()%35)) food[a].vida = true;
                                    else food[a].vida = false;

                                    // verifica se a comida não foi gerada em cima de um player
                                    for(int x = 0; x < (tam_snake_P1 < tam_snake_P2 ? tam_snake_P2 : tam_snake_P1); x++) {
                                        if(food[a].local_X == snake_P1[x].local_X && food[a].local_Y == snake_P1[x].local_Y ||
                                           food[a].local_X == snake_P2[x].local_X && food[a].local_Y == snake_P2[x].local_Y) {
                                            comida_sobrepondo = true;
                                            break;
                                        }
                                    }

                                    // verifica se a comida não foi gerada em cima de outra comida
                                    for(int x = 0; x < maxFood; x++) {
                                        if(x != a) {
                                            if(food[x].local_X == food[a].local_X && food[x].local_Y == food[a].local_Y && food[x].existe) {
                                                comida_sobrepondo = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                food[a].cor = rand() % 15 + 1;
                                gotoxy(food[a].local_X, food[a].local_Y);
                                textcolor(food[a].cor);
                                if(food[a].vida) printf("%c", coracao);
                                else printf("*");
                                food[a].existe = true;
                                food_campo++;

                                break;
                            }
                        }

                        tempo_food = GetTickCount();
                    }

                    // VERIFICA TECLA PRESSIONADA -----------------------------------------------------------------------------------------
                    if (kbhit()) {
                        tecla = getch();

                        if(tecla == 224) { // tecla especial pressionada
                            teclaAntiga_P2 = tecla_P2;

                            switch(tecla_P2 = getch()) {
                            case seta_C: // seta para cima
                                if (snake_P2[0].local_X-1 == snake_P2[1].local_X && snake_P2[0].local_Y == snake_P2[1].local_Y) tecla_P2 = teclaAntiga_P2;
                                if (teclaAntiga_P2 == seta_B) tecla_P2 = seta_B;
                                break;
                            case seta_B: // seta para baixo
                                if (snake_P2[0].local_X+1 == snake_P2[1].local_X && snake_P2[0].local_Y == snake_P2[1].local_Y) tecla_P2 = teclaAntiga_P2;
                                if (teclaAntiga_P2 == seta_C) tecla_P2 = seta_C;
                                break;
                            case seta_E: // seta pra esquerda
                                if (snake_P2[0].local_X == snake_P2[1].local_X && snake_P2[0].local_Y-1 == snake_P2[1].local_Y) tecla_P2 = teclaAntiga_P2;
                                if (teclaAntiga_P2 == seta_D) tecla_P2 = seta_D;
                                break;
                            case seta_D: // seta para direita
                                if (snake_P2[0].local_X == snake_P2[1].local_X && snake_P2[0].local_Y+1 == snake_P2[1].local_Y) tecla_P2 = teclaAntiga_P2;
                                if (teclaAntiga_P2 == seta_E) tecla_P2 = seta_E;
                                break;
                            default:
                                tecla_P2 = teclaAntiga_P2;
                                break;
                            }
                        }
                        else { // tecla comum pressionada
                            teclaAntiga_P1 = tecla_P1;
                            tecla_P1 = tecla;
                            switch(tecla_P1) {
                            case 'w': // seta para cima
                            case 'W':
                                if (snake_P1[0].local_X-1 == snake_P1[1].local_X && snake_P1[0].local_Y == snake_P1[1].local_Y) tecla_P1 = teclaAntiga_P1;
                                if (teclaAntiga_P1 == 's' || teclaAntiga_P1 == 'S') tecla_P1 = 's';
                                break;
                            case 's': // seta para baixo
                            case 'S':
                                if (snake_P1[0].local_X+1 == snake_P1[1].local_X && snake_P1[0].local_Y == snake_P1[1].local_Y) tecla_P1 = teclaAntiga_P1;
                                if (teclaAntiga_P1 == 'w' || teclaAntiga_P1 == 'W') tecla_P1 = 'w';
                                break;
                            case 'a': // seta pra esquerda
                            case 'A':
                                if (snake_P1[0].local_X == snake_P1[1].local_X && snake_P1[0].local_Y-1 == snake_P1[1].local_Y) tecla_P1 = teclaAntiga_P1;
                                if (teclaAntiga_P1 == 'd' || teclaAntiga_P1 == 'D') tecla_P1 = 'd';
                                break;
                            case 'd': // seta para direita
                            case 'D':
                                if (snake_P1[0].local_X == snake_P1[1].local_X && snake_P1[0].local_Y+1 == snake_P1[1].local_Y) tecla_P1 = teclaAntiga_P1;
                                if (teclaAntiga_P1 == 'a' || teclaAntiga_P1 == 'A') tecla_P1 = 'a';
                                break;
                            default:
                                tecla_P1 = teclaAntiga_P1;
                                break;
                            }
                        }

                        if (tecla == 27 || tecla == 'p' || tecla == 'P') {    // PAUSE
                            textcolor(7);
                            gotoxy(12,35);
                            printf("PAUSE!");
                            getch();
                            gotoxy(12,35);
                            printf("      ");

                            textcolor(cor_P1);
                            for(int x = 0; x < tam_snake_P1; x++) {
                                gotoxy(snake_P1[x].local_X,snake_P1[x].local_Y);
                                printf("%c", corpo);
                            }

                            textcolor(cor_P2);
                            for(int x = 0; x < tam_snake_P2; x++) {
                                gotoxy(snake_P2[x].local_X,snake_P2[x].local_Y);
                                printf("%c", corpo);
                            }

                            for(int x = 0; x < maxFood; x++) {
                                if(food[x].existe) {
                                    textcolor(food[x].cor);
                                    gotoxy(food[x].local_X,food[x].local_Y);
                                    if(food[x].vida) printf("%c", coracao);
                                    else printf("*");
                                }
                            }
                        }
                    }

                    // PLAYER 1 ---------------------------------------------------------------------------------------------------------
                    if (GetTickCount() - tempo_P1 > velocidade_inicial + tam_snake_P1 && !morreu_P1) {
                        textcolor(cor_P1); // Setando para a cor do player 1
                        for (int x = tam_snake_P1; x > 0; x--){ // repassando os antigos lugares do "corpo" para frente (andar)
                            snake_P1[x].local_Y = snake_P1[x-1].local_Y;
                            snake_P1[x].local_X = snake_P1[x-1].local_X;
                        }
                        gotoxy(snake_P1[tam_snake_P1].local_X,snake_P1[tam_snake_P1].local_Y); // apaga o ultimo lugar do corpo (rabo)
                        printf(" ");

                        switch(tecla_P1){
                        case 'w':
                        case 'W':
                            if (--snake_P1[0].local_X < 0) snake_P1[0].local_X = 24; // cima
                            break;
                        case 's':
                        case 'S':
                            if (++snake_P1[0].local_X > 24) snake_P1[0].local_X = 0; // baixo
                            break;
                        case 'a':
                        case 'A':
                            if (--snake_P1[0].local_Y < 0) snake_P1[0].local_Y = 79; // esquerda
                            break;
                        case 'd':
                        case 'D':
                            if (++snake_P1[0].local_Y > 79) snake_P1[0].local_Y = 0; // direita
                            break;
                        default: break;
                        }

                        // COMENDO
                        for(int x = 0; x < maxFood; x++) {  //Se cabeça estiver em cima da comida '*', então "come"
                            if (food[x].local_X == snake_P1[0].local_X && food[x].local_Y == snake_P1[0].local_Y && food[x].existe) {
                                if(food[x].vida) { // verifica se for vida soma mais um na vida do player e break;
                                    vida_P1++;
                                    gotoxy(27,8);
                                    printf("%i", vida_P1);
                                }
                                else{
                                    score_P1     += food[x].cor == cor_P1 ? 2 : 1;
                                    tam_snake_P1 += food[x].cor == cor_P1 ? 2 : 1;
                                    if(food[x].cor == cor_P1) {
                                        snake_P1[tam_snake_P1 - 1].local_X = snake_P1[tam_snake_P1 - 2].local_X;
                                        snake_P1[tam_snake_P1 - 1].local_Y = snake_P1[tam_snake_P1 - 2].local_Y;
                                    }
                                }
                                food[x].existe = false;
                                food_campo--;
                                break;
                            }
                        }

                        // Bateu em cima do PLAYER 2 e morreu, seu corpo vira restos de comida
                        for(int x = 0; x < tam_snake_P2; x++) {
                            if(snake_P1[0].local_X == snake_P2[x].local_X && snake_P1[0].local_Y == snake_P2[x].local_Y) {
                                morreu_P1 = true;
                                break;
                            }
                        }
                        for(int x = 1; x < tam_snake_P1; x++) {
                            if(snake_P1[0].local_X == snake_P1[x].local_X && snake_P1[0].local_Y == snake_P1[x].local_Y) {
                                morreu_P1 = true;
                                break;
                            }
                        }

                        gotoxy(26,8);
                        printf("%04i", score_P1);

                        gotoxy(snake_P1[0].local_X,snake_P1[0].local_Y);
                        if(!morreu_P1) printf("%c", corpo);
                        else{
                            vida_P1--;
                            for(int x = 1; x < tam_snake_P1; x++){
                                gotoxy(snake_P1[x].local_X,snake_P1[x].local_Y);
                                printf(" ");
                            }
                            score_P1 = 0;

                            for(int x = 1; x < tam_snake_P1; x++) {
                                if(food_campo >= 30) break;
                                for(int i = 0; i < maxFood; i++){
                                    if(food[i].existe == false){
                                        food[i].existe  = true;
                                        food[i].vida    = false;
                                        food[i].cor     = cor_P1;
                                        food[i].local_X = snake_P1[x].local_X;
                                        food[i].local_Y = snake_P1[x].local_Y;
                                        gotoxy(food[i].local_X,food[i].local_Y);
                                        printf("*");
                                        food_campo++;
                                        break;
                                    }
                                }
                            }

                            for(int i = 0; i <= tam_snake_P1; i++) {
                                snake_P1[i].local_X = -1;
                                snake_P1[i].local_Y = -1;
                            }

                            tempo_renascer_P1 = GetTickCount();
                        }

                        tempo_P1 = GetTickCount();
                    }

                    // PLAYER 2 ---------------------------------------------------------------------------------------------------------
                    if (GetTickCount() - tempo_P2 > velocidade_inicial + tam_snake_P2 && !morreu_P2) {
                        textcolor(cor_P2); // setando para cor do player 2
                        for (int x = tam_snake_P2; x > 0; x--){ // repassando os antigos lugares do "corpo" para frente (andar)
                            snake_P2[x].local_Y = snake_P2[x-1].local_Y;
                            snake_P2[x].local_X = snake_P2[x-1].local_X;
                        }
                        gotoxy(snake_P2[tam_snake_P2].local_X,snake_P2[tam_snake_P2].local_Y); // apaga o ultimo lugar do corpo (rabo)
                        printf(" ");

                        switch(tecla_P2){
                        case seta_C:
                            if (--snake_P2[0].local_X < 0) snake_P2[0].local_X = 24; // cima
                            break;
                        case seta_B:
                            if (++snake_P2[0].local_X > 24) snake_P2[0].local_X = 0; // baixo
                            break;
                        case seta_E:
                            if (--snake_P2[0].local_Y < 0) snake_P2[0].local_Y = 79; // esquerda
                            break;
                        case seta_D:
                            if (++snake_P2[0].local_Y > 79) snake_P2[0].local_Y = 0; // direita
                            break;
                        default: break;
                        }

                        // COMENDO
                        for(int x = 0; x < maxFood; x++) {  //Se cabeça estiver em cima da comida '*', então "come"
                            if (food[x].local_X == snake_P2[0].local_X && food[x].local_Y == snake_P2[0].local_Y && food[x].existe) {
                                if(food[x].vida) { // verifica se for vida soma mais um na vida do player e break;
                                    vida_P2++;
                                    gotoxy(27,75);
                                    printf("%i", vida_P2);
                                }
                                else {
                                    score_P2     += food[x].cor == cor_P2 ? 2 : 1;
                                    tam_snake_P2 += food[x].cor == cor_P2 ? 2 : 1;
                                    if(food[x].cor == cor_P2) {
                                        snake_P2[tam_snake_P2 - 1].local_X = snake_P2[tam_snake_P2 - 2].local_X;
                                        snake_P2[tam_snake_P2 - 1].local_Y = snake_P2[tam_snake_P2 - 2].local_Y;
                                    }
                                }
                                food[x].existe = false;
                                food_campo--;
                                break;
                            }
                        }

                        // Bateu em cima do PLAYER 1 e morreu, seu corpo vira restos de comida
                        for(int x = 0; x < tam_snake_P1; x++) {
                            if(snake_P2[0].local_X == snake_P1[x].local_X && snake_P2[0].local_Y == snake_P1[x].local_Y) {
                                morreu_P2 = true;
                                break;
                            }
                        }

                        for(int x = 1; x < tam_snake_P2; x++) {
                            if(snake_P2[0].local_X == snake_P2[x].local_X && snake_P2[0].local_Y == snake_P2[x].local_Y) {
                                morreu_P2 = true;
                                break;
                            }
                        }

                        gotoxy(26,75);
                        printf("%04i", score_P2);

                        gotoxy(snake_P2[0].local_X,snake_P2[0].local_Y);
                        if(!morreu_P2) printf("%c", corpo);
                        else{
                            vida_P2--;
                            for(int x = 1; x < tam_snake_P2; x++){
                                gotoxy(snake_P2[x].local_X,snake_P2[x].local_Y);
                                printf(" ");
                            }
                            score_P2 = 0;

                            for(int x = 1; x < tam_snake_P2; x++) {
                                if(food_campo >= 35) break;
                                for(int i = 0; i < maxFood; i++){
                                    if(food[i].existe == false){
                                        food[i].existe  = true;
                                        food[i].vida    = false;
                                        food[i].cor     = cor_P2;
                                        food[i].local_X = snake_P2[x].local_X;
                                        food[i].local_Y = snake_P2[x].local_Y;
                                        gotoxy(food[i].local_X,food[i].local_Y);
                                        printf("*");
                                        food_campo++;
                                        break;
                                    }
                                }
                            }

                            for(int i = 0; i <= tam_snake_P2; i++) {
                                snake_P2[i].local_X = -1;
                                snake_P2[i].local_Y = -1;
                            }

                            tempo_renascer_P2 = GetTickCount();
                        }

                        tempo_P2 = GetTickCount();
                    }
                }

                gotoxy(12,32);
                textcolor(7);
                if(morreu_P2) printf("PLAYER 1: %s", nome_P1);
                else printf("PLAYER 2: %s", nome_P2);

                long int tempo_pisca_win = GetTickCount();
                bool aparece_win = false;
                tecla = NULL;
                do {
                    if(GetTickCount() - tempo_pisca_win > 400) {
                        aparece_win = !aparece_win;
                        gotoxy(14,36);

                        if(aparece_win) printf("WIN!!!");
                        else printf("      ");
                        tempo_pisca_win = GetTickCount();
                    }

                    if(kbhit()) tecla = getch();
                } while(tecla != 13);
                gotoxy(14,36);
                printf("WIN!!!");

                gotoxy(20,8);
                printf("Pressione qualquer tecla para voltar.");

                getch();
            }

        }
        if (opcaoMenu != 4) opcaoMenu = 0;
    }while (opcaoMenu == 0);
    gotoxy(15,12);

    return 0;
}

void cabecalho(int x, int y, char titulo[]) { // Pode ser usado NULL na var y para centralizar na tela
    system("cls");
    y = y == -1 ? 35 - (strlen(titulo)/2) : y; // 35 pois desconsidera os caracteres "-== "
    gotoxy(x,y);
    printf("-== %s ==-", titulo);
    gotoxy(0,0);
}

obj parede[500];
int numeroParedes;

bool hit_wall( int local_X, int local_Y ) { // verifica se determinado lugar passado como parametro esta em conflito(bateu) com a parede do jogo
    for ( int x = 0; x < numeroParedes; x++ ) {
        if ( parede[x].local_X == local_X && parede[x].local_Y == local_Y )
            return true;
    }
    return false;
}

void fase( int level ) {
    system("cls");
    //Este ponteiro busca as fases
    FILE *arqFase;
    bool fim = false;

    int pos = 0;

    if(level == 1) arqFase = fopen("Fase 1.fase", "r"); // Fase 1 = Inicial
    else if (level == 2) arqFase = fopen("Fase 2.fase", "r");
    else if (level == 3) arqFase = fopen("Fase 3.fase", "r");
    else if (level == 0);
    else{
        fim = true;
        printf("Fim de jogo, voce venceu!");
    }

    if (level != 0 && arqFase == NULL) {
        gotoxy(12,20);
        printf("ERRO! Arquivo da fase %d nao encontrado.", level);
    }
    else if ( level != 0 && fim == false ) {
        int c;
        while ( (c = fgetc(arqFase)) != EOF ) if ( c == '\n' ) pos++;
        fseek(arqFase,0,0);
        for ( int x = 0; x < pos; x++) {
            fscanf(arqFase, "%i %i", &parede[x].local_X, &parede[x].local_Y);
            parede[x].existe = true;
        }
        fclose(arqFase);

        for ( int i = 0; i < pos; i++ ) {
            if ( parede[i].existe == true ) {
                bool paredeCima = false, paredeBaixo = false, paredeEsquerda = false, paredeDireita = false;
                gotoxy(parede[i].local_X,parede[i].local_Y);
                for ( int x = 0; x < pos; x++) {
                    if ( parede[x].existe == true ) { // identifica se possui alguma parede aos arredores para "animação" do obj parede
                        if ( parede[i].local_X-1 == parede[x].local_X && parede[i].local_Y == parede[x].local_Y ) // P. em cima
                            paredeCima = true;
                        else if ( parede[i].local_X+1 == parede[x].local_X && parede[i].local_Y == parede[x].local_Y ) // P. em baixo
                            paredeBaixo = true;
                        else if ( parede[i].local_X == parede[x].local_X && parede[i].local_Y-1 == parede[x].local_Y ) // P. na esquerda
                            paredeEsquerda = true;
                        else if ( parede[i].local_X == parede[x].local_X && parede[i].local_Y+1 == parede[x].local_Y ) // P. na direita
                            paredeDireita = true;
                    }
                }
                if ( paredeBaixo && paredeCima && paredeDireita && paredeEsquerda ) printf("%c", canteiro_ECBD);

                else if ( paredeEsquerda && paredeCima && paredeDireita ) printf("%c", canteiro_ECD);
                else if ( paredeCima && paredeDireita && paredeBaixo ) printf("%c", canteiro_CBD);
                else if ( paredeEsquerda && paredeBaixo && paredeDireita ) printf("%c", canteiro_EBD);
                else if ( paredeEsquerda && paredeCima && paredeBaixo ) printf("%c", canteiro_ECB);

                else if ( paredeEsquerda && paredeCima ) printf("%c", canteiro_EC);
                else if ( paredeCima && paredeDireita ) printf("%c", canteiro_CD);
                else if ( paredeBaixo && paredeDireita ) printf("%c", canteiro_BD);
                else if ( paredeEsquerda && paredeBaixo ) printf("%c", canteiro_EB);

                else if ( paredeCima && paredeBaixo ) printf("%c", canteiro_V);
                else if ( paredeDireita && paredeEsquerda ) printf("%c", canteiro_H);
                else printf("o");
            }
        }
        numeroParedes = pos;
        gotoxy(0,0);
    }
}

int sel_nome(int x, int y) {
    #define carac_SC 30 // definindo caracteres coringa para uma "imagem" melhorada do programa
    #define carac_SB 31
    y = y == -1 ? (78/2)-4 : y;
    int pos_caracter = 0;
    const int posY_Max = y + 4, posY_Min = y;
    char stringNome[6];
    for (int x = 0; x < 5; x++) stringNome[x] = 65;
    stringNome[5] = '\0';

    HideCursor(true); //escondendo cursor

    for (int i = 0; i < 5; i++) { // Apaga vestigios de retorno
        gotoxy(x-1,y+i);
        printf(" ");
        gotoxy(x+1,y+i);
        printf(" ");
    }
    gotoxy(x,y);
    printf("AAAAA");
    gotoxy(x-1,y);
    printf("%c", carac_SC);
    gotoxy(x+1,y);
    printf("%c", carac_SB);

    int tecla;
    do {
        tecla = getch();
        if (tecla == -32 || tecla == 224 || tecla == 0) { //Tecla especial, seta_D retorna valor -32 ou 224 ou 0 e em seguida 77
            switch (getch()) {              // Dependendo da maquina retorna valores diferentes de teclas especiais
            case seta_D:
                if (y < posY_Max) {
                    gotoxy(x-1,y);
                    printf(" ");
                    gotoxy(x+1,y);
                    printf(" ");

                    gotoxy(x-1,++y);
                    printf("%c", carac_SC);
                    gotoxy(x+1,y);
                    printf("%c", carac_SB);

                    pos_caracter++;
                }
                break;
            case seta_E:
                if (y > posY_Min) {
                    gotoxy(x-1,y);
                    printf(" ");
                    gotoxy(x+1,y);
                    printf(" ");

                    gotoxy(x-1,--y);
                    printf("%c", carac_SC);
                    gotoxy(x+1,y);
                    printf("%c", carac_SB);

                    pos_caracter--;
                }
                break;
            case seta_C:
                gotoxy(x,y);
                if (--stringNome[pos_caracter] < 48) stringNome[pos_caracter] = 90; // Se menor que '0' vai p 'Z'
                if (stringNome[pos_caracter] < 65 && stringNome[pos_caracter] > 57) stringNome[pos_caracter] = 57;//Se menor'A' vai p '9'
                printf("%c", stringNome[pos_caracter]);
                break;
            case seta_B:
                gotoxy(x,y);
                if (++stringNome[pos_caracter] > 57 && stringNome[pos_caracter] < 65) stringNome[pos_caracter] = 65;//Se maior'9' vai p 'A'
                if (stringNome[pos_caracter] > 90) stringNome[pos_caracter] = 48; // Se maior que 'Z' vai p '0'
                printf("%c", stringNome[pos_caracter]);
                break;
            }
        }
        else {
            //Se tecla for entre 'a' a 'z' ou '0' a '9' ou 'A' a 'Z'
            if ( (tecla >= 97 && tecla <= 122) || (tecla >= 48 && tecla <= 57) || (tecla >= 65 && tecla <= 90) ) {
                if (tecla >= 97 && tecla <= 122) tecla -= 32;
                gotoxy(x,y);
                stringNome[pos_caracter] = tecla;
                printf("%c", stringNome[pos_caracter]);

                // Logo após digitar uma letra ele mesmo faz a função da seta_D para ir ao proximo caractere
                if (y < posY_Max) {
                    gotoxy(x-1,y);
                    printf(" ");
                    gotoxy(x+1,y);
                    printf(" ");

                    gotoxy(x-1,++y);
                    printf("%c", carac_SC);
                    gotoxy(x+1,y);
                    printf("%c", carac_SB);

                    pos_caracter++;
                }
            }
        }
    } while (tecla != 13);

    HideCursor(false); //resetando cursor
    return(stringNome);
}

void ver_score() {
    FILE *arqScore;
    int linha = 0;

    arqScore = fopen("score.sco", "r");
    gotoxy(8,30);
    if ( arqScore == NULL ) printf("Sem registro de pontuacao!");
    else{
        char c;
        while ( (c = fgetc(arqScore)) != EOF ) if ( c == '\n' ) linha++;
        for ( int x = 0; x < linha; x++ ) {
            gotoxy(8+x,33);
            printf("........");
        }

        pont jogador[linha];
        fseek(arqScore,0,0);
        for ( int x = 0; x < linha; x++ ) {
            fscanf(arqScore, "%s %i", &jogador[x].nome, &jogador[x].score);
            gotoxy(8+x,33);
            printf("%s", jogador[x].nome);
            gotoxy(8+x,42);
            printf("%04i", jogador[x].score);
        }
        fclose(arqScore);
    }
}

void gravar_score(char Nome[], int Score) {
    FILE *arqScore; // ponteiro para o arquivo score
    const int maxLinha = 10; // Definindo max de linha do arquivo score

    arqScore = fopen("score.sco", "r");
    if (arqScore == NULL) {
        arqScore = fopen("score.sco", "w");
        for (int x = 0; x < maxLinha; x++) {
            fprintf(arqScore, "AAAAA 0\n");
        }
    }
    fclose(arqScore);

    arqScore = fopen("score.sco", "r");
    pont jogador[10]; // Max de scores para mostrar na tela

    for (int x = 0; x < maxLinha; x++) fscanf(arqScore, "%s %i", &jogador[x].nome, &jogador[x].score);

    if (Score >= jogador[maxLinha-1].score) { //Se a nova pontuação for maior q a ultima, entra no lugar e realinha a ordem
        jogador[maxLinha-1].score = Score;
        strcpy(jogador[maxLinha-1].nome, Nome);
        for (int fim = maxLinha-1; fim > 0; fim--) { //Realinha a ordem decrescente para arquivar
            for (int x = 0; x < fim; x++) {
                if (jogador[x].score <= jogador[x+1].score && !strcmp(jogador[x+1].nome, Nome)) {
                    int auxScore = jogador[x].score;//passando var para aux.
                    char auxNome[6];
                    strcpy(auxNome, jogador[x].nome);

                    jogador[x].score = jogador[x+1].score;//"redefinindo" a ordem
                    strcpy(jogador[x].nome, jogador[x+1].nome);

                    jogador[x+1].score = auxScore;//repasando para var o valor de aux.
                    strcpy(jogador[x+1].nome, auxNome);
                }
            }
        }
    }
    fclose(arqScore);

    arqScore = fopen("score.sco", "w");
    for (int x = 0; x < 10; x++) fprintf(arqScore, "%s %i\n", jogador[x].nome, jogador[x].score);
    fclose(arqScore);
}

void escolha( int coluna, int linha, char *string[], ... ){ // função para seleção de opções "=>"
    HideCursor(true); // Chama a função que esconde o cursor piscante da tela, para sumir da tela
    int linhaMin = linha, linhaMax, tmp="";
    va_list arg;

    gotoxy(linha++,coluna+3);
    printf("%s", string);
    va_start(arg, string);

    bool verifi = false;
    do{
        if(verifi!=false){
            gotoxy(linha++,coluna+3);
            printf("%s", tmp);
        }
        tmp = va_arg(arg, char*);
        verifi = true;
    }while(tmp!=NULL);
    linhaMax = linha-1;
    linha = linhaMin;

    va_end(arg);

    for ( int x = 0; x < 3; x++ ) {
        gotoxy(linha+x,coluna);
        printf("  ");
    }

    gotoxy(linha,coluna);
    printf("=>");
    int opcao = 1;
    for(;;){
        switch(tmp=getch()){
            case 56:  // Tecla 8 seta para cima
            case seta_C: // Tecla Seta para cima
                if(linha<=linhaMin){ // Caso aperte para cima e o cursor estiver no limite não sobe mais
                    break;
                }
                gotoxy(linha,coluna);
                printf("  ");
                gotoxy(--linha,coluna);
                printf("=>");
                opcao--;
                break;
            case 50:  // Tecla 2 seta para baixo
            case seta_B: // Tecla Seta para baixo
                if(linha>=linhaMax){ // Caso aperte para baixo e o cursor estiver no limite não desce mais
                    break;
                }
                gotoxy(linha,coluna);
                printf("  ");
                gotoxy(++linha,coluna);
                printf("=>");
                opcao++;
                break;
            case 27:
                opcao = 0;
            case 13:  // Tecla Enter, aqui acaba a função é quando o usuario preciona Enter para escolher a opção desejada
                return opcao_escolha(opcao);
        }
    }
}
int opcao_escolha( int n_opcao ) { // esta funcao foi criada para retorna o valor da "opcao", pois a funcao acima só aceita tipo VOID
    return n_opcao;              // sendo assim ela não retornaria nenhum valor
}

void rodape() {
    gotoxy(25,0);
    for(int x = 0; x < 80; x++) printf("%c", 238);
    gotoxy(28,0);
    printf("Faculdade Anhanguera Taubate - Unidade 2 - Curso Ci%cncia da Computa%c%co", 136,135,198);
    gotoxy(29,0);
    printf("Criado por: Relry Pereira dos Santos / 2%c Sem", 167);
    int tamStringV = strlen(versao);
    gotoxy(29,79-tamStringV);
    printf("%s", versao);
    gotoxy(0,0);
}
