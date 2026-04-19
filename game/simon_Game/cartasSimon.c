#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include "cartasSimon.h"

// Recebe uma carta e le o seu valor, printa o valor correspondente no terminal
void print_valor(struct carta c) 
{
    switch (c.valor) 
    {
        case 1:  printf(" A   "); break;
        case 10: printf(" 10  "); break; 
        case 11: printf(" J   "); break;
        case 12: printf(" Q   "); break;
        case 13: printf(" K   "); break;
        default: printf(" %d   ", c.valor); break;
    }  
}

// Recebe uma carta e le o seu naipe, printa o naipe correspondente no terminal
void print_naipe(struct carta c)
{
    switch (c.naipe)
    {
        case 0: printf("  ♥  "); break;
        case 1: printf("  ♠  "); break;
        case 2: printf("  ♦  "); break;
        case 3: printf("  ♣  "); break;
    }
}

// Recebe uma carta e da print a ela com formato apropriado (valor e naipe com fundo branco)
void print_carta(struct carta c) 
{
    printf("\033[30;47m"); //texto preto (30) fundo branco (47)
    print_valor(c);
    print_naipe(c);  
    printf("\033[0m"); //reseta pro normal
}

void print_pilhas(Pilhas p, int lim){
    Pilhas pTemp = p;
    int linha = 0;
    while (linha<(lim+1)){

        for (int i=0; i< 10; i++){
            if(pTemp == NULL || pTemp->numCartas <= linha)
                printf("          "); //espaco vazio
            else
                print_carta(pTemp->pilha[linha]);
                
            printf(" ");
            if (pTemp != NULL) {
                pTemp = pTemp->prox;
            }
        }
        pTemp = p;
        linha++;
        printf("\n");
    }
}

// Pede o numero da jogada no terminal para o player, nao para ate conseguir um numero valido de jogada
//0 -> sair
//1 a 7 -> fazer a jogada normal
//8 -> puxar do stock
//9 -> restart
int opcao_inicio()
{
    unsigned int jogada_escolhida = 0;
    
    printf("1 - Jogar, 2 - Reiniciar, 3 - Sair: ");
    scanf("%d", &jogada_escolhida);
    
    while(jogada_escolhida > 3 || jogada_escolhida < 1)
    {
        printf("Opção inválida\n");
        printf("1 - Jogar, 2 - Reiniciar, 3 - Sair: ");
        scanf("%d", &jogada_escolhida);
    }

    return jogada_escolhida;
}