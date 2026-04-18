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
    int contagemBaralho = 0;
    int capacidade[] = {8,8,8,7,6,5,4,3,2,1};
    int gameOver = 0;
    Pilhas celula = cria_pilhas(baralho, capacidade, 10);
    iniciar_jogo(baralho, &j, capacidade, &gameOver);
    print_pilhas(celula, acharLimite(celula));
    
    // Movimentação de teste de cartas
    int posOrigem[2] = {8,0};
    int posDestino[2] = {7,2};
    valida_jogada(&celula, posOrigem, posDestino);

    print_pilhas(celula, acharLimite(celula));
    
    return EXIT_SUCCESS;    
}