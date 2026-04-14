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