#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include "logicaSimon.h"

//LOGICA: Recebe um array de struct carta, e para cada slot (52 cartas), atribui o valor e naipe de forma consecutiva
void cria_baralho(struct carta *baralho)
{
    int val = 1;
    int np = 0;

    for(int idx=0; idx<52; idx++)
    {
        baralho[idx].valor = val;
        baralho[idx].naipe = np;

        val++;

        if(val>13){
            val = 1;
            np++;
        }
    }
}

//LOGICA: Pega num baralho ja preenhido e mistura a ordem das cartas de forma aleatoria
void shuffle_baralho(struct carta *baralho)
{
    srand(time(NULL)); //seed para randomizar baralho (segundos atuais)
    int idx;
    
    for(int i=0; i<52; i++)
    {
        struct carta temp;
       
        idx = rand() % 52;
        
        //guarda a carta e troca as posicoes para nao repetir cartas
        temp = baralho[idx];
        baralho[idx] = baralho[i];
        baralho[i]= temp;   
    }

}