#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include "cartas.h"
#include "cartas.c"
#include "logica.h"
#include "logica.c"

int main(void)
{
    //instrucoes
    printf("1 a 7 --> Escolhe uma coluna e tenta a jogada \n8 --> Tira uma carta do stock para sua mão \n9 --> Restart \n0 --> Sair\n");
    //declarar variaveis
    struct carta baralho[52];
    struct carta ultima;
    int stock = 0;
    int gameOver = 0;
    
    iniciar_jogo(baralho, &stock, &ultima, &gameOver);
    
    while (!(gameOver==1 || gameOver==2))
    {
        print_game(baralho,stock,ultima);
        processar_jogada(baralho,&stock,&ultima,&gameOver);
        printf("\n");
    }
    
    if (gameOver == 1)
    {
        printf("Não ganhaste\n");
    }
    else
    {
        printf("Você ganhou!!!\n");
    }

    return EXIT_SUCCESS;    
}