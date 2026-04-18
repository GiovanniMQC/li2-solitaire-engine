#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include "cartasSimon.h"

// Recebe um array de struct carta, e para cada slot (52 cartas), atribui o valor e naipe de forma consecutiva
void cria_baralho(struct carta *baralho)
{
    int val = 13;
    int np = 0;

    for(int idx=0; idx<52; idx++)
    {
        baralho[idx].valor = val;
        baralho[idx].naipe = np;

        val--;

        if(val<1){
            val = 13;
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

Pilhas procura_pilha(Pilhas *p, int pos)
{
    // Cria um apontador que será usado para procurar a pilha desejada
    Pilhas pilhaResultado = *p;

    // Mudando o endereço para a coluna correta
    for(int i = 0; i < pos && pilhaResultado != NULL; i++)
    {
        pilhaResultado = pilhaResultado->prox;
    }
    return pilhaResultado;
}

void libera_memoria_cartas(Pilhas p)
{
    // Liberta o espaço de memória que deixou de ser usado na origem
    if (p->numCartas > 0) {
        p->pilha = realloc(p->pilha, p->numCartas * sizeof(struct carta));
    } else {
        free(p->pilha);
        p->pilha = NULL;
    }
}

void insere_carta(struct carta origem, Pilhas destino)
{
    // Aumenta o espaço da memória para inserir as cartas movidas
        destino->pilha = realloc(destino->pilha, (destino->numCartas + 1) * sizeof(struct carta));
    
    // Adiciona a carta na pilha destino
    destino->pilha[destino->numCartas] = origem;

    // Atualiza quantidade de cartas nas pilhas
    destino->numCartas++;
}

void insere_cartas(Pilhas origem, Pilhas destino, int linha, int numMovidas)
{
    // Aumenta o espaço da memória para inserir as cartas movidas
    destino->pilha = realloc(destino->pilha, (destino->numCartas + numMovidas) * sizeof(struct carta));
    // Coloca os valores das cartas no novo espaço
    for(int i = 0; i < numMovidas; i++)
    {
        destino->pilha[destino->numCartas + i] = origem->pilha[(linha - 1) + i];
    }

    // Atualiza quantidade de cartas nas pilhas
    destino->numCartas += numMovidas;
    origem->numCartas -= numMovidas;

    // Liberta o espaço de memória que deixou de ser usado na origem
    libera_memoria_cartas(origem);
}

void corrige_seq_cartas(Pilhas p, int linha)
{
    // Desloca as cartas restantes na pilha de origem para preencher o buraco deixado pela carta movida
    for (int i = linha - 1; i < p->numCartas - 1; i++)
    {
        p->pilha[i] = p->pilha[i + 1];
    }

    // Atualiza quantidade de cartas nas pilhas
    p->numCartas--;
}

// Recebe uma pilha e uma array de posição que deve ter a [coluna, linha], tanto para origem quanto o destino.
int mover_cartas(Pilhas *p, int posOrig[], int posDest[])
{
    // Extração dos dados baseados na array
    int orig_col = posOrig[0];
    int orig_linha = posOrig[1] + 1;
    int dest_col = posDest[0];
    
    // Avança o apontador para as colunas de origem e destino das cartas
    Pilhas pilhaOrigem = procura_pilha(p, orig_col);
    Pilhas pilhaDestino = procura_pilha(p, dest_col);
    

    // Prevenção de erros caso as colunas não existam ou quantidade seja inválida
    if(pilhaOrigem == NULL || pilhaDestino == NULL || orig_linha < 1) return 1;

    // Guarda o número de cartas que serão movidas
    int numMovidas = pilhaOrigem->numCartas - (orig_linha - 1);

    // Checa se o número de carta movidas é menor ou igual a zero
    if (numMovidas <= 0) return 1; // Nada a mover
    
    insere_cartas(pilhaOrigem, pilhaDestino, orig_linha, numMovidas);

    return 0;
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

int carta_check (Pilhas *p, int posOrig[], int posDest[])
{
    int origCol = posOrig[0];
    int origLin = posOrig[1];
    int destCol = posDest[0];
    int destLin = posDest[1];

    Pilhas pilhaOrigem = procura_pilha(p, origCol);
    Pilhas pilhaDestino = procura_pilha(p, destCol);
    
    struct carta origem = (pilhaOrigem->pilha)[origLin];
    struct carta chegada = (pilhaDestino->pilha)[destLin];

    int naipeSelecionado = origem.naipe;

    if ((pilhaOrigem->numCartas)<(origLin) || (!(origem.valor == (chegada.valor-1) || pilhaDestino->numCartas == 0)))
        return 1;
    
    for (int i = origLin; i < pilhaOrigem->numCartas; i++)
    {
        struct carta cartaAverificar = (pilhaOrigem->pilha)[i];
        if (!(cartaAverificar.naipe == naipeSelecionado) || (cartaAverificar.valor == (pilhaOrigem->pilha)[i+1].valor-1))
            return 1;
    }
    return 0;
}

int valida_jogada(Pilhas *p, int posOrig[], int posDest[])
{
    if (posOrig[0]>=10 || posDest[0]>=10 || posDest[0]<0 || posOrig[0]<0 || posDest[1]<0 || posOrig[1]<0)
        return 1;
    
    if (carta_check(p, posOrig, posDest) == 1)
        return 1;

    mover_cartas(p, posOrig, posDest);
    return 0;
}