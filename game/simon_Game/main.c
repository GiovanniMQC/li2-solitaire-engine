#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include "cartasSimon.h"
#include "logicaSimon.h"
#include "cartasSimon.c"
#include "logicaSimon.c"

int main(void)
{
    //declarar variaveis
    struct carta baralho[52];
    cria_baralho(baralho);
    shuffle_baralho(baralho);

    int j = 0;
    int capacidade[] = {8,8,8,7,6,5,4,3,2,1};
    Pilhas celula = cria_pilhas(baralho, capacidade, 10);
    print_pilhas(celula, 8);
    
    // Movimentação de teste de cartas
    int posOrigem[2] = {5,4};
    int posDestino[2] = {1,8};
    mover_cartas(&celula, posOrigem, posDestino, 1);
    // printf("\n");
    print_pilhas(celula, 8);

    return EXIT_SUCCESS;    
}