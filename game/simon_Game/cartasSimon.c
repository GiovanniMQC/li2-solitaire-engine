#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include "cartasSimon.h"

//INTERFACE: Recebe uma carta e le o seu valor, printa o valor correspondente no terminal
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

//INTERFACE: Recebe uma carta e le o seu naipe, printa o naipe correspondente no terminal
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

//INTERFACE: Recebe uma carta e da print a ela com formato apropriado (valor e naipe com fundo branco)
void print_carta(struct carta c) 
{
    if(c.valor == 0)
        printf("          ");
    else
    {
        printf("\033[30;47m"); //texto preto (30) fundo branco (47)
        print_valor(c);
        print_naipe(c);  
        printf("\033[0m"); //reseta pro normal
    } 
}

//INTERFACE: Recebe uma pilha e da print dependendo do tamanho. No Simple Simon são 3 pilhas de 8 cartas e o restante decrescente de 7 a 1.
void print_colunas(struct carta baralho[])
{
    printf("    1          2          3          4          5          6          7          8          9          10\n");

    for (int i = 0; i < 24; i++) 
    {
        print_carta(baralho[i]);
        putchar(' ');
    }
    printf("\n");
}

//INTERFACE: Da print no estado atual do jogo
void print_game(struct carta baralho[], int stock, struct carta ultima){
    print_colunas(baralho);
    printf("Stock: %d ", stock);
    print_carta(ultima);
    putchar('\n');
}

//INTERFACE/LOGICA: Pede o numero da jogada no terminal para o player, nao para ate conseguir um numero valido de jogada
//0 -> sair
//1 a 7 -> fazer a jogada normal
//8 -> puxar do stock
//9 -> restart
int pedir_jogada()
{
    unsigned int jogada_escolhida = 10;
    
    printf("Digite o número para sua Jogada: ");
    scanf("%d", &jogada_escolhida);
    
    while(jogada_escolhida > 9 || jogada_escolhida < 0)
    {
       printf("Jogada Inválida - Tente Novamente: ");
       scanf("%d", &jogada_escolhida);
    }

    return jogada_escolhida;
}