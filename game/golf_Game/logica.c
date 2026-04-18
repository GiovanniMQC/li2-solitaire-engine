#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include "cartas.h"

// Recebe um array de struct carta, e para cada slot (52 cartas), atribui o valor e naipe de forma consecutiva
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

// Pega num baralho ja preenhido e mistura a ordem das cartas de forma aleatoria
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

// Inicializa o jogo, atribuindo valor as variaveis iniciais, gerando e dando shuffle em um baralho
void iniciar_jogo(struct carta baralho[], int *stock, struct carta *ultima, int *gameOver) 
{
    cria_baralho(baralho);
    shuffle_baralho(baralho);
    
    *stock = 16;
    *gameOver = 0;
    *ultima = baralho[51]; 
}

// Pega na carta atual da mao e na carta correspondente a base da coluna escolhida e     
//        checka se os valores das duas estao em um intervalo de -1/+1
int valores_Check(struct carta baralho[], struct carta *ultima, int jogadaEscolhida)
{    
    int base_idx = achar_base_coluna(baralho, jogadaEscolhida);

        if(ultima->valor == baralho[base_idx].valor-1 || ultima->valor == baralho[base_idx].valor+1)
        {   
            return 1;
        }      
        else
            return 0;
}

// Recebe o n de uma coluna (1 a 7) e verifica se os valores da carta na mao e da carta na base da coluna 
//        estao em um intervalo valido para realizar a jogada, caso contrario a jogada nao e feita
void jogada_Coluna(struct carta baralho[], int *stock, struct carta *ultima, int jogadaEscolhida)
{    
    int base_idx = achar_base_coluna(baralho, jogadaEscolhida);

        if(valores_Check(baralho,&*ultima,jogadaEscolhida))
        {   
            *ultima = baralho[base_idx];
            baralho[base_idx].valor = 0;
        }      
        else
        {
            printf("Valor das cartas não compatíveis - Tente Novamente: \n");
            
        }
}

// Recebe o baralho, quantidade de cartas do stock e jogadaEscolhida para definir o estado do gameOver dependendo
//         se o player perdeu, ganhou ou apenas saiu do jogo. 
void checa_gameOver(struct carta baralho[], int *stock, int *gameOver, unsigned int jogadaEscolhida)
{
    int naoTemCartas = 1;
    for(int i = 0; i < 7; i++)
    {
        if (baralho[i].valor != 0)
        {
            naoTemCartas = 0;
        }
    }
    //SAIR
    if (naoTemCartas == 1)
        *gameOver = 2;
    else if((naoTemCartas == 0 && (*stock) <= 0) || jogadaEscolhida == 0)
        *gameOver = 1;
}

// A partir da jogada selecionada, processa a jogada correta para o numero dado
void processar_jogada(struct carta baralho[], int *stock, struct carta *ultima, int *gameOver)
{
    unsigned int jogadaEscolhida = pedir_jogada();
    
    //RESTART
    if(jogadaEscolhida == 9)
    {
        iniciar_jogo(baralho, &*stock,&*ultima,&*gameOver);
    }

    //Jogar
    else if(jogadaEscolhida<=7 && jogadaEscolhida>=1)
    {
        jogada_Coluna(baralho,&*stock,&*ultima,jogadaEscolhida);
        return;
    }
    
    //Checar se deu gameOver
    checa_gameOver(baralho, stock, gameOver, jogadaEscolhida);
    
    //Puxar carta
    if (jogadaEscolhida == 8)
    {
        puxar_Stock(baralho,&*stock,&*ultima);
    }
}