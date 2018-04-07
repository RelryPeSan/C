#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

//Operações de entreda/saída
#define READ    10 // Lê um inteiro e armazena na memória
#define WRITE   11 // Imprime o inteiro que está na memória

//Operações de carregamento/armazenamento
#define LOAD    20 // Carrega um inteiro da memória para o 'acumulador'
#define STORE   21 // Armazena um inteiro do 'acumulador' para a memória
#define ZERO    22 // Zera um lugar do armazenamento

//Operações aritiméticas
#define ADD         30 // Soma o 'acumulador' e o valor da memória e armazena no 'acumulador'
#define SUBTRACT    31 // Subtrai do 'acumulador' o valor da memória e armazena no 'acumulador'
#define DIVIDE      32 // Divide o 'acumulador' pelo valor da memória e armazena no 'acumulador'
#define MULTIPLY    33 // Multiplica o 'acumulador' e o valor da memória e armazena no 'acumulador'
#define INCREMENT   34 // Acrescenta mais 1(um) no acumulador
#define DECREMENT   35 // Decrementa mais 1(um) no acumulador

//Operações de transferência de controle
#define BRANCH      40 // A execução pula para o espaço de memória especificado
#define BRANCHNEG   41 // A execução pula para o espaço de memória especificado, se o 'acumulador' for negativo
#define BRANCHZERO  42 // A execução pula para o espaço de memória especificado, se o 'acumulador' for 0(zero)
#define HALT        43 // Finaliza a execução do programa

//Tamanho da memória de Simpleston
#define MAX_MEMORY_SIMPLESTON 100

//Variáveis globais do sistema
int memoria[MAX_MEMORY_SIMPLESTON];
int acumulador = 0;
int contInstrucao = 0;
int regisInstrucao = 0;
int codOperacao = 0;
int operando = 0;

//"enumera" FALSE como 0 e TRUE como 1
typedef enum{
    false, true
}bool;

/* Protótipos */
void boasVindas();
void menuInicial();
void escreverCodigo(int cont);
int verificaValor(int valor);
void executarMemoria();
void erroFatal(char erroEspecifico[]);
void visualizarDump();
void zerarMemoria();

int main(){
    system("mode con:cols=80 lines=40");
    setlocale(LC_ALL, "");

    //Inicia com mensagem bem vindo
    boasVindas();

    //Inicia o menu inicial de opções
    while(true)
        menuInicial(); //só encerra programa opção sair desta função

}

void boasVindas(){
    printf("´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´\n");
    printf("´     Bem vindo ao Simpleston!  v%.1f     ´\n", 1.0);
    printf("´ Insira as instruções do programa a ser ´\n");
    printf("´ realizado, seus dois primeiros números ´\n");
    printf("´ são seus operandos e seus dois ultimos ´\n");
    printf("´ são seus endereços de memória.         ´\n");
    printf("´                                        ´\n");
    printf("´ Para finalizar um código utilize o co- ´\n");
    printf("´ mando -99999 no fim do último comando. ´\n");
    printf("´                                        ´\n");
    printf("´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´\n");
    printf(" Max Memory: %i\n", MAX_MEMORY_SIMPLESTON);

}

void menuInicial(){
    int opcao = 0;

    printf("\n\n");
    printf(" 1. Escrever código(subscreve memória)\n");
    printf(" 2. Escrever a partir de...\n");
    printf(" 3. Executar memória\n");
    printf(" 4. Visualizar Dump\n");
    printf(" 5. Zerar memória\n");
    printf(" 0. Encerrar sessão\n");
    printf("\n");

    do{
        printf(" Opção # ");
        scanf("%i%*c", &opcao);
        if(opcao < 0 || opcao > 5)
            printf(" Digite uma opção válida!\n");
    }while(opcao < 0 || opcao > 5);

    switch(opcao){
    case 1:
        escreverCodigo(0);
        break;
    case 2:
        if(true){ // if criado somente para declarar variavel dentro do switch
            int pularMemoria;
            printf(" Qual lugar da memória deseja ir?(0 - 99)\n Digite um inteiro # ");
            scanf("%i%*c", &pularMemoria);
            escreverCodigo(pularMemoria);

        }
        break;
    case 3:
        executarMemoria();
        break;
    case 4:
        visualizarDump();
        break;
    case 5:
        zerarMemoria();
        break;
    case 0:
        printf("\nSessão encerrada...\n");
        exit(0);
    }
}

void escreverCodigo(int cont){
    bool fimCodigo = false;
    contInstrucao = cont;

    if(cont >= 100 || cont < 0){
        erroFatal("Valor especificado inválido da memória.");
        fimCodigo = true;
    }

    printf("\n\n");

    while(!fimCodigo){
        //passa a instrução do usuario primeiro para esta variavel 'auxMemoria'
        //afim de quando passar a instrução -9999 não subscrever nehum código
        int auxMemoria;

        //esse bool verifica se o valor esta num intervalo pré-determinado -9999 a 9999
        bool valorFora = false;
        do{
            // Caso a memória tenha "residuos" ela será mostrada
            if(memoria[contInstrucao] != 0)
                printf("{Memory[%02i] = %+05i} # ", contInstrucao, memoria[contInstrucao]);
            else
                printf(" Memory[%02i] # ", contInstrucao);

            scanf("%i%*c", &auxMemoria);

            valorFora = verificaValor(auxMemoria);

            if(auxMemoria == -99999)
                break;

            if(valorFora)
                printf("Digite um valor entre -9999 e 9999.\n");
        }while(valorFora);

        if(auxMemoria == -99999)
            break;

        memoria[contInstrucao] = auxMemoria;

        contInstrucao++;
    }
    /*for(int x = 0; x < contInstrucao; x++)
        printf("\n memoria[%02i] => %+05i", x, memoria[x]);//*/
}

int verificaValor(int valor){
    if(valor >= -9999 && valor <= 9999 )
        return false;//retorna false caso o valor não esteja fora dos pré-requisito
    else
        return true;
}

void executarMemoria(){
    //este bool verifica quando a execução acaba
    bool fimExecucao = false;

    //zerando o contador de memória
    contInstrucao = 0;

    while(!fimExecucao){
        regisInstrucao = memoria[contInstrucao];
        codOperacao = regisInstrucao / 100; //'codOperacao' pega os dois números da esquerca da memória
        operando = regisInstrucao % 100;//'operando' pega os dois números da direita da memória
        //'operando' mostra em qual parte da memória deve ocorrer a operação

        switch(codOperacao){
        //Operações de entreda/saída - código 10
        case READ: // lê uma entrada do usuário (inteiro)

            printf("Digite um inteiro # ");
            int auxMemoria;
            scanf("%i%*c", &auxMemoria);
            if(auxMemoria == -99999){
                printf("\n Execução interrompida!\n\n");
                fimExecucao = true;
                break;
            }
            else{
                memoria[operando] = auxMemoria;
            }
            break;
        case WRITE: // printa na tela o valor que está na memória
            printf("Memory[%02i] => %i", operando, memoria[operando]);
            break;

        //Operações de carregamento/armazenamento - código 20
        case LOAD: // manda pro acumulador
            acumulador = memoria[operando];
            break;
        case STORE: // armazena em um lugar da memória o valor do acumulador
            memoria[operando] = acumulador;
            break;
        case ZERO: // Zera uma memória especifica
            memoria[operando] = 0;
            break;

        //Operações aritiméticas - código 30
        case ADD: // soma o valor passado com o acumulador e armazena no acumulador
            acumulador += memoria[operando];
            break;
        case SUBTRACT: // subtrai o valor passado do acumulador e armazena no acumulador
            acumulador -= memoria[operando];
            break;
        case DIVIDE: // divide o valor passado do acumulador e armazena no acumulador
            if(memoria[operando] == 0){
                erroFatal("Divisão por 0(ZERO).");
                fimExecucao = true;
                break;
            }
            acumulador /= memoria[operando];
            break;
        case MULTIPLY: // multiplica o valor passado com o acumulador e armazena no acumulador
            acumulador *= memoria[operando];
            break;
        case INCREMENT: // acrescenta mais 1(um) no 'acumulador'
            acumulador++;
            break;
        case DECREMENT: // decrescenta mais 1(um) no 'acumulador'
            acumulador--;
            break;

        //Operações de transferência de controle - código 40
        case BRANCH: // desvia para um lugar da memória
            contInstrucao = operando - 1; // -1 pois no fim do loop ele avança para próxima memória
            break;
        case BRANCHNEG: // desvia para um lugar da memoria caso o acumulador for negativo
            if(acumulador < 0)
                contInstrucao = operando - 1; // -1 pois no fim do loop ele avança para próxima memória
            break;
        case BRANCHZERO: // desvia para um lugar da memoria caso o acumulador for 0(zero)
            if(acumulador == 0)
                contInstrucao = operando - 1; // -1 pois no fim do loop ele avança para próxima memória
            break;
        case HALT: // fim da execução
            printf("\n Fim da execução.\n");
            fimExecucao = true;
            break;
        default:
            erroFatal("Programa abortado.");
            fimExecucao = true;
        }

        //passa para a proxima memória
        contInstrucao++;
    }

}

void erroFatal(char erroEspecifico[]){
    printf("\n ERRO FATAL! %s\n", erroEspecifico);
    visualizarDump();
}

void visualizarDump(){
    printf("\n\n REGISTRADORES\n");
    printf(" acumulador\t%+05i\n", acumulador);
    printf(" contInstrucao\t   %02i\n", contInstrucao);
    printf(" regisInstrucao\t%+05i\n", regisInstrucao);
    printf(" codOperacao\t   %02i\n", codOperacao);
    printf(" operando\t   %02i\n", operando);

    printf("\n\n MEMORY:\n      ");
    for(int i = 0; i < 10; i++)
        printf("    [%i]", i);

    for(int i = 0; i < 10; i++){
        printf("\n  [%02i] ", i * 10);
        for(int j = 0; j < 10; j++){
            printf(" %+05i ", memoria[(i*10) + j]);
        }
    }
}

void zerarMemoria(){
    char opcao;

    printf("\n\nIsto apagará todos os comandos já passados para a memória do programa.\nContinuar? ( s / n )\n");

    do{
        printf(" Opção # ");
        scanf("%c%*c", &opcao);

        if((opcao != 's' && opcao != 'S') && (opcao != 'n' && opcao != 'N'))
            printf("Opção inválida! Tente novamente.\n");
    }while((opcao != 's' && opcao != 'S') && (opcao != 'n' && opcao != 'N'));

    if(opcao == 's' || opcao == 'S'){
        for(int x = 0; x < 100; x++)
            memoria[x] = 0;
        printf("\nMemória foi zerada!");
    }
    else
        printf("\nNada foi feito.");

    scanf("%*c");
    printf("\n");
}
