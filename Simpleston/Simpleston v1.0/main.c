#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

//Opera��es de entreda/sa�da
#define READ    10 // L� um inteiro e armazena na mem�ria
#define WRITE   11 // Imprime o inteiro que est� na mem�ria

//Opera��es de carregamento/armazenamento
#define LOAD    20 // Carrega um inteiro da mem�ria para o 'acumulador'
#define STORE   21 // Armazena um inteiro do 'acumulador' para a mem�ria
#define ZERO    22 // Zera um lugar do armazenamento

//Opera��es aritim�ticas
#define ADD         30 // Soma o 'acumulador' e o valor da mem�ria e armazena no 'acumulador'
#define SUBTRACT    31 // Subtrai do 'acumulador' o valor da mem�ria e armazena no 'acumulador'
#define DIVIDE      32 // Divide o 'acumulador' pelo valor da mem�ria e armazena no 'acumulador'
#define MULTIPLY    33 // Multiplica o 'acumulador' e o valor da mem�ria e armazena no 'acumulador'
#define INCREMENT   34 // Acrescenta mais 1(um) no acumulador
#define DECREMENT   35 // Decrementa mais 1(um) no acumulador

//Opera��es de transfer�ncia de controle
#define BRANCH      40 // A execu��o pula para o espa�o de mem�ria especificado
#define BRANCHNEG   41 // A execu��o pula para o espa�o de mem�ria especificado, se o 'acumulador' for negativo
#define BRANCHZERO  42 // A execu��o pula para o espa�o de mem�ria especificado, se o 'acumulador' for 0(zero)
#define HALT        43 // Finaliza a execu��o do programa

//Tamanho da mem�ria de Simpleston
#define MAX_MEMORY_SIMPLESTON 100

//Vari�veis globais do sistema
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

/* Prot�tipos */
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

    //Inicia o menu inicial de op��es
    while(true)
        menuInicial(); //s� encerra programa op��o sair desta fun��o

}

void boasVindas(){
    printf("������������������������������������������\n");
    printf("�     Bem vindo ao Simpleston!  v%.1f     �\n", 1.0);
    printf("� Insira as instru��es do programa a ser �\n");
    printf("� realizado, seus dois primeiros n�meros �\n");
    printf("� s�o seus operandos e seus dois ultimos �\n");
    printf("� s�o seus endere�os de mem�ria.         �\n");
    printf("�                                        �\n");
    printf("� Para finalizar um c�digo utilize o co- �\n");
    printf("� mando -99999 no fim do �ltimo comando. �\n");
    printf("�                                        �\n");
    printf("������������������������������������������\n");
    printf(" Max Memory: %i\n", MAX_MEMORY_SIMPLESTON);

}

void menuInicial(){
    int opcao = 0;

    printf("\n\n");
    printf(" 1. Escrever c�digo(subscreve mem�ria)\n");
    printf(" 2. Escrever a partir de...\n");
    printf(" 3. Executar mem�ria\n");
    printf(" 4. Visualizar Dump\n");
    printf(" 5. Zerar mem�ria\n");
    printf(" 0. Encerrar sess�o\n");
    printf("\n");

    do{
        printf(" Op��o # ");
        scanf("%i%*c", &opcao);
        if(opcao < 0 || opcao > 5)
            printf(" Digite uma op��o v�lida!\n");
    }while(opcao < 0 || opcao > 5);

    switch(opcao){
    case 1:
        escreverCodigo(0);
        break;
    case 2:
        if(true){ // if criado somente para declarar variavel dentro do switch
            int pularMemoria;
            printf(" Qual lugar da mem�ria deseja ir?(0 - 99)\n Digite um inteiro # ");
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
        printf("\nSess�o encerrada...\n");
        exit(0);
    }
}

void escreverCodigo(int cont){
    bool fimCodigo = false;
    contInstrucao = cont;

    if(cont >= 100 || cont < 0){
        erroFatal("Valor especificado inv�lido da mem�ria.");
        fimCodigo = true;
    }

    printf("\n\n");

    while(!fimCodigo){
        //passa a instru��o do usuario primeiro para esta variavel 'auxMemoria'
        //afim de quando passar a instru��o -9999 n�o subscrever nehum c�digo
        int auxMemoria;

        //esse bool verifica se o valor esta num intervalo pr�-determinado -9999 a 9999
        bool valorFora = false;
        do{
            // Caso a mem�ria tenha "residuos" ela ser� mostrada
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
        return false;//retorna false caso o valor n�o esteja fora dos pr�-requisito
    else
        return true;
}

void executarMemoria(){
    //este bool verifica quando a execu��o acaba
    bool fimExecucao = false;

    //zerando o contador de mem�ria
    contInstrucao = 0;

    while(!fimExecucao){
        regisInstrucao = memoria[contInstrucao];
        codOperacao = regisInstrucao / 100; //'codOperacao' pega os dois n�meros da esquerca da mem�ria
        operando = regisInstrucao % 100;//'operando' pega os dois n�meros da direita da mem�ria
        //'operando' mostra em qual parte da mem�ria deve ocorrer a opera��o

        switch(codOperacao){
        //Opera��es de entreda/sa�da - c�digo 10
        case READ: // l� uma entrada do usu�rio (inteiro)

            printf("Digite um inteiro # ");
            int auxMemoria;
            scanf("%i%*c", &auxMemoria);
            if(auxMemoria == -99999){
                printf("\n Execu��o interrompida!\n\n");
                fimExecucao = true;
                break;
            }
            else{
                memoria[operando] = auxMemoria;
            }
            break;
        case WRITE: // printa na tela o valor que est� na mem�ria
            printf("Memory[%02i] => %i", operando, memoria[operando]);
            break;

        //Opera��es de carregamento/armazenamento - c�digo 20
        case LOAD: // manda pro acumulador
            acumulador = memoria[operando];
            break;
        case STORE: // armazena em um lugar da mem�ria o valor do acumulador
            memoria[operando] = acumulador;
            break;
        case ZERO: // Zera uma mem�ria especifica
            memoria[operando] = 0;
            break;

        //Opera��es aritim�ticas - c�digo 30
        case ADD: // soma o valor passado com o acumulador e armazena no acumulador
            acumulador += memoria[operando];
            break;
        case SUBTRACT: // subtrai o valor passado do acumulador e armazena no acumulador
            acumulador -= memoria[operando];
            break;
        case DIVIDE: // divide o valor passado do acumulador e armazena no acumulador
            if(memoria[operando] == 0){
                erroFatal("Divis�o por 0(ZERO).");
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

        //Opera��es de transfer�ncia de controle - c�digo 40
        case BRANCH: // desvia para um lugar da mem�ria
            contInstrucao = operando - 1; // -1 pois no fim do loop ele avan�a para pr�xima mem�ria
            break;
        case BRANCHNEG: // desvia para um lugar da memoria caso o acumulador for negativo
            if(acumulador < 0)
                contInstrucao = operando - 1; // -1 pois no fim do loop ele avan�a para pr�xima mem�ria
            break;
        case BRANCHZERO: // desvia para um lugar da memoria caso o acumulador for 0(zero)
            if(acumulador == 0)
                contInstrucao = operando - 1; // -1 pois no fim do loop ele avan�a para pr�xima mem�ria
            break;
        case HALT: // fim da execu��o
            printf("\n Fim da execu��o.\n");
            fimExecucao = true;
            break;
        default:
            erroFatal("Programa abortado.");
            fimExecucao = true;
        }

        //passa para a proxima mem�ria
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

    printf("\n\nIsto apagar� todos os comandos j� passados para a mem�ria do programa.\nContinuar? ( s / n )\n");

    do{
        printf(" Op��o # ");
        scanf("%c%*c", &opcao);

        if((opcao != 's' && opcao != 'S') && (opcao != 'n' && opcao != 'N'))
            printf("Op��o inv�lida! Tente novamente.\n");
    }while((opcao != 's' && opcao != 'S') && (opcao != 'n' && opcao != 'N'));

    if(opcao == 's' || opcao == 'S'){
        for(int x = 0; x < 100; x++)
            memoria[x] = 0;
        printf("\nMem�ria foi zerada!");
    }
    else
        printf("\nNada foi feito.");

    scanf("%*c");
    printf("\n");
}
