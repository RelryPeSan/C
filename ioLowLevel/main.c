#include <io.h>

typedef char* string;

int wVetChar(const string vet){
    int tam = 0;
    while(vet[tam] != '\0') tam++;
    return write(1, vet, tam);
}

int rVetChar(const string *vet){
    char lido[512];
    int tam = read(0, lido, 512);
    vet = lido;
    return tam;
}

int main()
{
    string nome;
    wVetChar("oi");
    rVetChar(nome);
    wVetChar(nome);

    return 0;
}

