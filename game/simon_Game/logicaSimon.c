#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include "cartasSimon.h"

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

// Cria uma pilha a partir de um baralho, um número de cartas na pilha e a quantidade de pilhas. Coloca a nova pilha no fim da lista de células.
// Retorna o endereço da primeira pilha
Pilhas cria_pilha(struct carta *baralho, int numCartas, int *contagemBaralho)
{
    // Um apontador para o início de uma pilha.
    Pilhas p = malloc(sizeof(struct celula));
    p->numCartas = numCartas;
    p->pilha = malloc(sizeof(struct carta) * numCartas);
    p->prox = NULL;

    // Um loop para criar uma pilha
    for(int i = 0; i < numCartas; i++, (*contagemBaralho)++)
    {
        p->pilha[i].naipe = baralho[(*contagemBaralho)].naipe;
        p->pilha[i].valor = baralho[(*contagemBaralho)].valor;
    }
    return p;
}

// Cria uma lista de pilhas a partir de um baralho, número de pilhas e recebe uma array que guarda em sequência os tamanhos das pilhas.
Pilhas cria_pilhas(struct carta *baralho, int numCartas[], int numPilhas)
{
    // Se o número de pilhas é 0 retorna NULL
    if(numPilhas <= 0) return NULL;
    // Variável que guarda quantas cartas foram percorridas no baralho.
    int contagemBaralho = 0;

    // Cria a primeira pilha.
    Pilhas inicioPilha = cria_pilha(baralho, numCartas[0], &contagemBaralho);

    Pilhas tmp = inicioPilha;
    for(int i = 1; i < numPilhas; i++)
    {
        Pilhas nova = cria_pilha(baralho, numCartas[i], &contagemBaralho);
        tmp->prox = nova;
        tmp = nova;
    }

    return inicioPilha;
}


// Recebe uma pilha e uma array de posição que deve ter a [coluna, linha], tanto para origem quanto o destino.
Pilhas mover_cartas(Pilhas *p, int posOrig[], int posDest[], int moveInferiores)
{
    if (p == NULL || *p == NULL) return NULL;

    // Extração dos dados baseados na array
    int orig_col = posOrig[0];
    int orig_linha = posOrig[1];
    int dest_col = posDest[0];
    
    Pilhas pilhaOrigem = *p;
    Pilhas pilhaDestino = *p;
    
    // Mudando o endereço para a coluna correta
    for(int i = 0; i < orig_col && pilhaOrigem != NULL; i++)
    {
        pilhaOrigem = pilhaOrigem->prox;
    }
    // Avança para a coluna que será movida as cartas
    for(int i = 0; i < dest_col && pilhaDestino != NULL; i++)
    {
        pilhaDestino = pilhaDestino->prox;
    }

    // Prevenção de erros caso as colunas não existam ou quantidade seja inválida
    if(pilhaOrigem == NULL || pilhaDestino == NULL || orig_linha < 1) return *p;

    if(moveInferiores)
    {
        // Guarda o número de cartas que serão acrescentadas a pilha destino
        int numMovidas = pilhaOrigem->numCartas - (orig_linha - 1);
        if (numMovidas <= 0) return *p; // Nada a mover

        // Aumenta o espaço da memória para inserir as cartas movidas
        pilhaDestino->pilha = realloc(pilhaDestino->pilha, (pilhaDestino->numCartas + numMovidas) * sizeof(struct carta));

        // Coloca os valores das cartas no novo espaço
        for(int i = 0; i < numMovidas; i++)
        {
            pilhaDestino->pilha[pilhaDestino->numCartas + i] = pilhaOrigem->pilha[(orig_linha - 1) + i];
        }

        // Atualiza quantidade de cartas nas pilhas
        pilhaDestino->numCartas += numMovidas;
        pilhaOrigem->numCartas -= numMovidas;
    }
    else
    {
        // Guarda o número de cartas que serão acrescentadas a pilha destino
        int numMovidas = 1;

        // Copia a carta que será movida
        struct carta cartaMovida = pilhaOrigem->pilha[orig_linha - 1];
        
        // Aumenta o espaço da memória para inserir as cartas movidas
        pilhaDestino->pilha = realloc(pilhaDestino->pilha, (pilhaDestino->numCartas + numMovidas) * sizeof(struct carta));

        // Adiciona a carta na pilha destino
        pilhaDestino->pilha[pilhaDestino->numCartas] = cartaMovida;

        // Atualiza quantidade de cartas nas pilhas
        pilhaDestino->numCartas++;
        pilhaOrigem->numCartas--;
    }
    
    return *p;
}

// Acha a pilha com maior número de carta e retorna o número que há
int acharLimite(Pilhas p){
    Pilhas pTemp = p->prox;
    int maior = p->numCartas;

    for (int i=0; i<9; i++){
        if (maior < pTemp->numCartas)
        {
            maior = pTemp->numCartas;
        }
        
        pTemp = pTemp->prox;
    }

    return maior;
}
