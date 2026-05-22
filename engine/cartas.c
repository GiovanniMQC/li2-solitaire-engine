#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* escolherPaciencia(char lista_de_caminhos[][512], int quantidade) {

    int escolha = 0;
    printf("\nDigite o numero da paciencia que deseja (1 a %d): ", quantidade);
    
    if (scanf("%d", &escolha) != 1) 
        return NULL;

    if (escolha >= 1 && escolha <= quantidade) 
        return lista_de_caminhos[escolha - 1]; 

    return NULL;
}