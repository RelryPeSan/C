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
#define MODULE      36 // Resto da divisão entre acumulador e um valor da memória e armazena no 'acumulador'
#define EXPONENTIAL 37 // Potência do acumulador elevado a um valor da memória e armazena no 'acumulador'

//Operações de transferência de controle
#define BRANCH      40 // A execução pula para o espaço de memória especificado
#define BRANCHNEG   41 // A execução pula para o espaço de memória especificado, se o 'acumulador' for negativo
#define BRANCHZERO  42 // A execução pula para o espaço de memória especificado, se o 'acumulador' for 0(zero)
#define HALT        43 // Finaliza a execução do programa

//Constantes de Simpleston
#define MAX_MEMORY_SIMPLESTON 1024
#define ENCERRAR    -999999
#define INICIO      0
#define SISTEMA_NUMERICO 16 // Hexadecimal

//Variáveis globais do sistema
float memoria[MAX_MEMORY_SIMPLESTON];
float acumulador = 0x0;
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
bool verificaValor(int valor);
void executarMemoria(int contInicio);
void erroFatal(char erroEspecifico[]);
void visualizarDump();
void zerarMemoria();

int main(){
    //system("mode con:cols=80 lines=80");
    setlocale(LC_ALL, "");

    //Inicia com mensagem bem vindo
    boasVindas();

    //Inicia o menu inicial de opções
    while(true)
        menuInicial(); //só encerra programa opção sair desta função

}

void boasVindas(){
    printf("´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´\n");
    printf("´      Bem vindo ao Simpleston!  v%.1f      ´\n", 1.1);
    printf("´ Insira as instruções do programa a ser   ´\n");
    printf("´ realizado, seus dois primeiros números   ´\n");
    printf("´ são seus operandos e seus dois ultimos   ´\n");
    printf("´ são seus endereços de memória.           ´\n");
    printf("´                                          ´\n");
    printf("´ Para finalizar um código utilize o co-   ´\n");
    printf("´ mando -999999 no fim do último comando.  ´\n");
    printf("´                                          ´\n");
    printf("´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´\n");
    printf(" Max Memory: %i(10)\t%X(16)\n", MAX_MEMORY_SIMPLESTON, MAX_MEMORY_SIMPLESTON);

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

    printf("\n\n");

    switch(opcao){
    case 1:
        escreverCodigo(INICIO);
        break;
    case 2:
        if(true){ // if criado somente para declarar variavel dentro do switch
            int pularMemoria;
            printf(" Qual lugar da memória deseja ir?(0 - 999)\n Digite um inteiro # ");
            scanf("%X%*c", &pularMemoria);
            escreverCodigo(pularMemoria);
        }
        break;
    case 3:
        executarMemoria(INICIO);
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
    contInstrucao = (int) cont;

    if(cont >= MAX_MEMORY_SIMPLESTON || cont < 0){
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
                printf("{Memory[%03X] = %+05X} # ", contInstrucao, memoria[contInstrucao]);
            else
                printf(" Memory[%03X] # ", contInstrucao);

            scanf("%i", &auxMemoria);
            //setbuf(stdin, NULL);

            valorFora = verificaValor(auxMemoria);

            if(auxMemoria == ENCERRAR)
                break;

            if(valorFora)
                printf("Digite um valor entre -99999 e 99999.\n");
        }while(valorFora);

        if(auxMemoria == ENCERRAR)
            break;

        memoria[contInstrucao] = auxMemoria;

        if(contInstrucao >= MAX_MEMORY_SIMPLESTON - 1){
            printf("\n - Memória chegou ao fim! -\n");
            fimCodigo = true;
        }
        else
            contInstrucao++;
    }
    /*for(int x = cont; x < contInstrucao; x++)
        printf("\n memoria[%03f] => %+05i", x, memoria[x]);//*/
}

bool verificaValor(int valor){
    if(valor >= -99999 && valor <= 99999 )
        return false;//retorna false caso o valor não esteja fora dos pré-requisito
    else
        return true;
}

void executarMemoria(int contInicio){
    //este bool verifica quando a execução acaba
    bool fimExecucao = false;

    //seta o contador de memória
    contInstrucao = contInicio;

    while(!fimExecucao){
        regisInstrucao = memoria[contInstrucao];
        codOperacao = regisInstrucao / 1000; //'codOperacao' pega os dois números da esquerca da memória
        operando = regisInstrucao % 1000;//'operando' pega os dois números da direita da memória
        //'operando' mostra em qual parte da memória deve ocorrer a operação

        switch(codOperacao){
        //Operações de entreda/saída - código 10
        case READ: // lê uma entrada do usuário (inteiro)
            printf("Digite um número # ");
            float auxMemoria;
            scanf("%f%*c", &auxMemoria);
            if(auxMemoria == ENCERRAR){
                printf("\n Execução interrompida!\n\n");
                fimExecucao = true;
            }
            else
                memoria[operando] = auxMemoria;

            break;
        case WRITE: // printa na tela o valor que está na memória
            printf("Memory[%03X] => %i", operando, memoria[operando]);
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
            erroFatal("Código da operação não encontrado.");
            fimExecucao = true;
        }

        //passa para a proxima memória
        if(codOperacao != HALT)
            contInstrucao++;

        if(contInstrucao >= MAX_MEMORY_SIMPLESTON){
            erroFatal("Fim da memória do programa.");
            fimExecucao = true;
        }
    }

}

void erroFatal(char erroEspecifico[]){
    printf("\n ERRO FATAL! %s\n", erroEspecifico);
    visualizarDump();
}

void visualizarDump(){
    printf("\n\n REGISTRADORES\n");
    printf(" acumulador\t%+06i\n", acumulador);
    printf(" contInstrucao\t   %03i\n", contInstrucao);
    printf(" regisInstrucao\t%+06i\n", regisInstrucao);
    printf(" codOperacao\t    %02i\n", codOperacao);
    printf(" operando\t   %03i\n", operando);

    printf("\n\n MEMORY:\n      ");
    for(int i = 0; i < SISTEMA_NUMERICO; i++)
        printf("    [%X] ", i);

    bool fimMemoria = false;
    for(int i = 0; i < MAX_MEMORY_SIMPLESTON/(SISTEMA_NUMERICO-1); i++){
        printf("\n [%03X]", i * SISTEMA_NUMERICO);
        for(int j = 0; j < SISTEMA_NUMERICO; j++){
            printf(" %+06.f ", memoria[(i*SISTEMA_NUMERICO) + j]);
            if((i*SISTEMA_NUMERICO)+j >= MAX_MEMORY_SIMPLESTON-1){
                fimMemoria = true;
                break;
            }
        }
        if(fimMemoria)
            break;
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
        for(int x = 0; x < MAX_MEMORY_SIMPLESTON; x++)
            memoria[x] = 0;
        printf("\nMemória foi zerada!");
    }
    else
        printf("\nNada foi feito.");

    scanf("%*c");
    printf("\n");
}
