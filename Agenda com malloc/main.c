#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct
{
    char nome[30];
    char email[30];
    char telefone[20];
} agenda;

agenda* alocar();
void cadastro(agenda*, int *);
int realocar(agenda *, int);
void consultar(agenda *,int);

int main()
{
    int resp;
    int i = 0;

    agenda *p = alocar();

    while (resp != 9)
    {
        printf("\n1 - Cadastrar");
        //printf("\n2 - Alterar");
        printf("\n3 - Consultar");
       // printf("\n4 - Excluir");
       // printf("\n5 - Pesquisar");
        printf("\n9 - Sair\n");
        printf("--> ");
        scanf("%d",&resp);
        fflush(stdin);

        switch(resp){
        case 1:
            cadastro(p,&i);
            break;

        case 3:
            consultar(p,i);
            break;

        case 9:
            break;

        default:
            printf("Opcao invalida! ");
        }

        printf("\n\nPressione ENTER para continuar...");
        getchar();
        system("cls");
    }

    return 0;
}

agenda* alocar()
{
    agenda *p = (agenda*) malloc(1*sizeof(agenda));
    if (p!= NULL)
    {
        return p;
    }
    else
    {
        printf("\nMEMORIA INSUFICIENTE\n");
        exit(1);
    }
}

int realocar(agenda *p,int i)
{
    p = (agenda*) realloc(p,i*sizeof(agenda));
    if(p!= NULL)
    {
        //printf("\nRealocado com sucesso!");
        return 1;
    }
    else
    {
        //printf("\nN foi possivel realocar!");
        return 0;
    }
}

void cadastro(agenda*p, int *i)
{
    if(!realocar(p, *i+1)){ // se não conseguir realocar
        printf("\nN foi possivel adicionar novo contato!");
        return;
    }
    //int resp;
    int j = *i;

    printf("Digite o nome: ");
    gets(p[j].nome);
    printf("Digite o email: ");
    gets(p[j].email);
    printf("Digite um telefone: ");
    gets(p[j].telefone);
    j++;
    *i = j;
    /*printf("\nCadastrar mais (s-1/n-0): ");
    scanf("%d",&resp);
    system("cls");
    while (resp == 1)
    {
        realocar(p,j);
        fflush(stdin);
        printf("\nDigite o nome: ");
        gets(p[j].nome);
        printf("Digite o email: ");
        gets(p[j].email);
        printf("Digite um telefone: ");
        gets(p[j].telefone);
        printf("\nCadastrar mais (s-1/n-0): ");
        scanf("%d",&resp);
    }//*/
}

void consultar(agenda *p, int i)
{
    if(i == 0)
    {
        printf("\nSem registro!\n");
        return;
    }

    int k;
    for (k = 0; k < i; k++)
    {
        printf("\nID: %03d", k);
        printf("\nNome: %s",p[k].nome);
        printf("\nEmail: %s",p[k].email);
        printf("\nTelefone: %s\n",p[k].telefone);
    }
}
