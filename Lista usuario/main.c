#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>

typedef struct user{
    char nome[20];
    char senha[15];
    struct user *proxUser;
}user;

user *primeiro, *atual, *novoUser;

void removerPosicao(int posicao){
    atual = primeiro;

    if(posicao <= 1){
        primeiro = atual->proxUser;
        free(atual);
        return;
    }
    else{
        user *removeUser;
        for(int x = 1; x < posicao-1; x++){
            if(atual->proxUser == NULL) break;
            atual = atual->proxUser; //procura novo user
        }
        removeUser = atual->proxUser;
        if(atual->proxUser != NULL){
            atual->proxUser = atual->proxUser->proxUser;
        }
        else{
            primeiro = atual->proxUser;
        }
        free(removeUser);
    }
}

void adicionarNaPosicao(int posicao){
    novoUser = (user *) malloc(sizeof(user));
    novoUser->proxUser = NULL;
    if(primeiro == NULL){ //se estiver vazia, inicializa a lista
        primeiro = atual = novoUser;
    }
    else{
        atual = primeiro;
        if(posicao <= 1){
            novoUser->proxUser = atual;
            primeiro = atual = novoUser;
        }
        else{
            for(int x = 1; x < posicao-1; x++){
                if(atual->proxUser == NULL) break;
                atual = atual->proxUser; //procura novo user
            }
            novoUser->proxUser = atual->proxUser;
            atual->proxUser = novoUser;
            atual = novoUser;
        }
    }
    printf("\nDigite o user.: ");
    gets(atual->nome);
    printf("Digite a senha: ");
    gets(atual->senha);
    //atual->proxUser = NULL;
}

void adicionar(){
    novoUser = (user *) malloc(sizeof(user));
    if(primeiro == NULL){ //se estiver vazia, inicializa a lista
        primeiro = atual = novoUser;
    }
    else{
        atual = primeiro;
        while(atual->proxUser != NULL)
            atual = atual->proxUser; //procura novo user
        atual->proxUser = novoUser;
        atual = novoUser;
    }
    printf("\nDigite o user.: ");
    gets(atual->nome);
    printf("Digite a senha: ");
    gets(atual->senha);
    atual->proxUser = NULL;
}

void mostrar(){
    if(primeiro == NULL){
        printf("\n\n - Lista vazia -\n\n");
        return;
    }

    atual = primeiro;

    int count = 1;
    do{
        printf("\n - User %02i -\n", count++);
        printf("name: %s\n", atual->nome);
        printf("pass: %s\n", atual->senha);
        atual = atual->proxUser;
    }while(atual != NULL);
}

int main()
{
    setlocale(LC_ALL, "");
    int op;

    while(1){
        printf("1. adicionar\n2. mostrar\n3. adicionar na pos...\n4. remover pos\n0. sair\nOpção: ");
        scanf("%i%*c", &op);

        switch(op){
        case 1:
            adicionar();
            break;
        case 2:
            mostrar();
            break;
        case 3:
            if(1){
                int pos;
                printf("\nQual posicao?\n => ");
                scanf("%i%*c", &pos);
                adicionarNaPosicao(pos);
            }
            break;
        case 4:
            if(primeiro == NULL){
            printf("\n - Lista vazia, nao ha o que remover\n");
            }
            else{
                int pos;
                printf("\nQual posicao?\n => ");
                scanf("%i%*c", &pos);
                removerPosicao(pos);
            }
            break;
        case 0:
            exit(0);
            break;
        default:
            printf("\n - Opção inválida -\n");
        }
        printf("\nPressione qualquer tecla...");
        _getch();
        //system("cls");
        printf("\n\n");
    }

    return 0;
}
