#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int teste(char *string[],...){
    int opcao, nstring=0;
    char tmp[20];
    size_t len = strlen(string);
    va_list arg;

    va_start(arg, string);

    while((opcao=va_arg(arg,char *))!=0){
        //if(opcao<5)
        printf("\n%i", opcao);
        nstring++;
    }
    printf("\n%i", nstring);
    va_end(arg);


    va_start(arg, string);

    printf("\n%s",string);
    for(int i=0; i<nstring; i++){
        if((opcao = strcpy(tmp, va_arg(arg, char *)))!=0) printf("\n%s",tmp);
        else break;
    }
    va_end(arg);

    scanf("%i", &opcao);

    return opcao;
}

int main(){
    printf("O que deseja fazer?");
    for(;;){
        switch(teste("Digitar","Sair","Outro","blabla")){
            case 1:{
                printf("Deu 1");
                break;
            }
            case 2:{
                printf("Deu 2");
                break;
            }
            default:{
                printf("Deu nada");
                break;
            }
        }
    }
}
