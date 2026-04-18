#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
// É uma má prática incluir ficheiros .c. Inclua apenas os .h
// e compile todos os .c juntos.
#include "cartasSimon.h"
#include "logicaSimon.h"

int main(void)
{
    //declarar variaveis
    struct carta baralho[52];
    int contagemBaralho = 0;
    int tamPilhas[] = {8,8,8,7,6,5,4,3,2,1};
    int gameOver = 0;
    Pilhas celula = NULL; // Declarado como ponteiro simples e inicializado
    
    // Passa o endereço de 'celula' para que a função possa modificá-la
    iniciar_jogo(baralho, &celula, &contagemBaralho, tamPilhas, &gameOver);
    
    while (!(gameOver==1 || gameOver==2))
    {
        print_pilhas(celula, acharLimite(celula));
        processar_jogada(baralho, &celula, &contagemBaralho, tamPilhas, &gameOver);
    }
    
    return EXIT_SUCCESS;    
}