#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include "cartasSimon.h"

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

// Cria uma pilha a partir de um baralho, um número de cartas na pilha e a quantidade de pilhas. Coloca a nova pilha no fim da lista de células.
// Retorna o endereço da primeira pilha
Pilhas cria_pilha(struct carta *baralho, int tamPilha, int *j)
{
    // Um apontador para o início de uma pilha.
    Pilhas p = malloc(sizeof(struct celula));
    p->numCartas = tamPilha;
    p->pilha = malloc(sizeof(struct carta) * tamPilha);
    p->prox = NULL;

    // Um loop para criar uma pilha
    for(int i = 0; i < tamPilha; i++, (*j)++)
    {
        p->pilha[i].naipe = baralho[(*j)].naipe;
        p->pilha[i].valor = baralho[(*j)].valor;
    }
    return p;
}

// Cria uma lista de pilhas a partir de um baralho, número de pilhas e recebe uma array que guarda em sequência os tamanhos das pilhas.
Pilhas cria_pilhas(struct carta *baralho, int tamPilha[], int numPilhas)
{
    if(numPilhas <= 0) return NULL;
    // Variável que guarda quantas cartas foram percorridas no baralho.
    int contagemBaralho = 0;

    // Cria a primeira pilha.
    Pilhas inicioPilha = cria_pilha(baralho, tamPilha[0], &contagemBaralho);

    Pilhas tmp = inicioPilha;
    for(int i = 1; i < numPilhas; i++)
    {
        Pilhas nova = cria_pilha(baralho, tamPilha[i], &contagemBaralho);
        tmp->prox = nova;
        tmp = nova;
    }

    return inicioPilha;
}

// Recebe uma pilha e uma array de posição que deve ter a [coluna, linha, quantidade], tanto para origem quanto o destino.
Pilhas mover_cartas(Pilhas *p, int cartOrig[], int cartDest[])
{
    if (p == NULL || *p == NULL) return NULL;

    // Extração dos dados baseados na array (assumindo índice 0 para as colunas)
    int orig_col = cartOrig[0];
    int orig_linha = cartOrig[1];
    int qtd = cartOrig[2];
    
    
    int dest_col = cartDest[0];

    if (qtd <= 0) return *p; // Nada a mover

    Pilhas orig_node = *p;
    Pilhas dest_node = *p;

    // 1. Encontrar as colunas (nós) na lista ligada
    for (int i = 0; i < orig_col && orig_node != NULL; i++) orig_node = orig_node->prox;
    for (int i = 0; i < dest_col && dest_node != NULL; i++) dest_node = dest_node->prox;

    // Validar se as colunas existem e se a linha e quantidade fazem sentido na origem
    if (orig_node == NULL || dest_node == NULL || orig_node == dest_node) return *p;
    if (orig_linha < 0 || orig_linha + qtd > orig_node->numCartas) return *p;

    // 2. Aumentar o tamanho da pilha de destino usando realloc
    int novo_tam_dest = dest_node->numCartas + qtd;
    struct carta *nova_pilha_dest = realloc(dest_node->pilha, novo_tam_dest * sizeof(struct carta));
    if (nova_pilha_dest == NULL && novo_tam_dest > 0) return *p; // Falha de alocação de memória
    dest_node->pilha = nova_pilha_dest;

    // 3. Copiar as cartas da origem para o final da pilha de destino
    for (int i = 0; i < qtd; i++) {
        dest_node->pilha[dest_node->numCartas + i] = orig_node->pilha[orig_linha + i];
    }
    dest_node->numCartas = novo_tam_dest;

    // 4. Reduzir a pilha de origem
    int novo_tam_orig = orig_node->numCartas - qtd;
    // Em vez de realocar para menor (o que às vezes não é estritamente necessário em C),
    // nós simplesmente ajustamos o contador. A memória extra é irrelevante e economiza processamento.
    orig_node->numCartas = novo_tam_orig;

    return *p;
}

//LOGICA: Recebe um baralho e uma coluna, retorna o index da ultima carta de cima pra baixo da coluna
int achar_base_coluna(struct carta baralho[], int coluna) 
{
    int i = coluna-1;

    //parte da primeira carta da coluna e desce (+7 no indice) pra ver se tem valor ou foi removida com valor 0
    while(baralho[i].valor != 0)
    {
        if(baralho[i+7].valor != 0 && i+7 < 35)
            i+=7;
        else
            return i;
    }

    return -1;
}

//LOGICA: Inicializa o jogo, atribuindo valor as variaveis iniciais, gerando e dando shuffle em um baralho
void iniciar_jogo(struct carta baralho[], int *stock, struct carta *ultima, int *gameOver) 
{
    cria_baralho(baralho);
    shuffle_baralho(baralho);
    
    *stock = 16;
    *gameOver = 0;
    *ultima = baralho[51]; 
}

//LOGICA: Puxa uma carta do stock para a mao atual, as ultimas 16 cartas correspondem ao stock, sao selecionados consecutivamente ate 35
void puxar_Stock(struct carta baralho[], int *stock, struct carta *ultima)
{
    (*stock)--;
    *ultima = baralho[35 + *stock];
}

//LOGICA: Pega na carta atual da mao e na carta correspondente a base da coluna escolhida e     
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

//LOGICA: Recebe o n de uma coluna (1 a 7) e verifica se os valores da carta na mao e da carta na base da coluna 
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

// LOGICA: Recebe o baralho, quantidade de cartas do stock e jogadaEscolhida para definir o estado do gameOver dependendo
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

//LOGICA: A partir da jogada selecionada, processa a jogada correta para o numero dado
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