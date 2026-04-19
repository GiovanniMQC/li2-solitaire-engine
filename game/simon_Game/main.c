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
    // Declarar variaveis
    struct carta baralho[52];
    int contagemBaralho = 0;
    // Tamanho da array é igual a quantidade de pilhas/colunas no jogo
    int tamPilhas[14];
    int gameOver = 0;
    Pilhas celula = NULL; // Declarado como ponteiro simples e inicializado
    
    // Passa o endereço de 'celula' para que a função possa modificá-la
    iniciar_jogo(baralho, &celula, &contagemBaralho, tamPilhas, &gameOver);
    
    while (!(gameOver==1 || gameOver==2))
    {
        printf("    1          2          3          4          5          6          7          8          9          10\n");
        print_pilhas(celula, acharLimite(celula));
        processar_jogada(baralho, &celula, &contagemBaralho, tamPilhas, &gameOver);
    }
    
    return EXIT_SUCCESS;    
}