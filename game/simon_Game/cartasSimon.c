#include <stdio.h>
#include <stdlib.h>
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