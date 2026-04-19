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

// Cria uma pilha a partir de um baralho e guarda o número de cartas na pilha. Coloca a nova pilha no fim da lista de células.
// Retorna o endereço da nova pilha
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
    // Se a array for nula ou a quantidade for inválida, retorna NULL
    if(numCartas == NULL || numPilhas <= 0) return NULL;
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

// Recebe a pilha inicial e procura a partir de uma posição a pilha destino
// Retorna a pilha destino
Pilhas procura_pilha(Pilhas p, int pos)
{
    // Cria um apontador que será usado para procurar a pilha desejada
    Pilhas pilhaResultado = p;

    // Mudando o endereço para a coluna correta
    for(int i = 0; i < pos && pilhaResultado != NULL; i++)
    {
        pilhaResultado = pilhaResultado->prox;
    }
    return pilhaResultado;
}

// Realoca o espaço de memória para a quantidade atualizada de cartas.
void libera_memoria_cartas(Pilhas p)
{
    // Liberta o espaço de memória que deixou de ser usado na origem
    if (p->numCartas > 0) {
        // Redimensiona a array da pilha
        p->pilha = realloc(p->pilha, p->numCartas * sizeof(struct carta));
    } else {
        free(p->pilha);
        p->pilha = NULL;
    }
}

// Recebe duas pilhas para acessar as cartas e também a linha escolhida e o número de cartas movidas.
// Modifica os espaços de memória para acrescentar ou retirar cartas das duas pilhas e acrescenta as cartas que devem ser movidas na pilha destino.
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

// Recebe uma pilha e a linha movida e passa a carta a frente da linha para trás e todas as outras também
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
    Pilhas pilhaOrigem = procura_pilha(*p, orig_col);
    Pilhas pilhaDestino = procura_pilha(*p, dest_col);
    

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
    if (p == NULL) return 0;

    Pilhas pTemp = p;
    int maior = p->numCartas;

    while (pTemp != NULL) {
        if (maior < pTemp->numCartas)
        {
            maior = pTemp->numCartas;
        }
        
        pTemp = pTemp->prox;
    }

    return maior;
}

// verifica se as as cartas são compatíveis
int carta_check (Pilhas pilhaOrigem, Pilhas pilhaDestino, struct carta origem, struct carta chegada, int origLin, int naipeSelecionado)
{
    
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

// verifica se as posiçoes pedidas sao validas
int pos_valida(int posOrig[], int posDest[])
{
    if (posOrig[0]>=10 || posDest[0]>=10 || posDest[0]<0 || posOrig[0]<0 || posDest[1]<0 || posOrig[1]<0 || posOrig[0] == posDest[0])
        return 1;
    return 0;
}

// verifica se a jogada pedida é possivel
int valida_jogada(Pilhas p, int posOrig[], int posDest[])
{
    
    int origCol = posOrig[0],
    origLin = posOrig[1],
    destCol = posDest[0],
    destLin = posDest[1];
    
    Pilhas pilhaOrigem = procura_pilha(p, origCol),
    pilhaDestino = procura_pilha(p, destCol);
        
    if (pos_valida(posOrig, posDest) || pilhaOrigem == NULL || pilhaDestino == NULL)
        return 1;

    destLin = pilhaDestino->numCartas - 1;

    if (pilhaOrigem->pilha == NULL)
    {
        return 1;
    }

    struct carta origem = (pilhaOrigem->pilha)[origLin],
    chegada = {destCol, 1}; // Carta placeholder caso a coluna destino esteja vazia

    if (pilhaDestino->pilha != NULL && pilhaDestino->numCartas > 0) {
        chegada = (pilhaDestino->pilha)[destLin];
    }
    
    if (carta_check(pilhaOrigem, pilhaDestino, origem, chegada, origLin, origem.naipe) == 1)
        return 1;

    return 0;
}

// Inicializa o jogo, atribuindo valor as variaveis iniciais, gerando e dando shuffle em um baralho
void iniciar_jogo(struct carta baralho[], Pilhas *p, int *contagemBaralho, int tamPilhas[], int *gameOver) 
{
    cria_baralho(baralho);
    // shuffle_baralho(baralho);
    *contagemBaralho = 0;
    // Define os tamanhos iniciais para cada uma das 10 pilhas 
    int valoresIniciais[] = {8,8,8,7,6,5,4,3,2,1,0,0,0,0};
    int tamanhoArray = sizeof(valoresIniciais) / sizeof(valoresIniciais[0]);
    for (int i = 0; i < tamanhoArray; i++)
    {
        tamPilhas[i] = valoresIniciais[i];
    }

    *p = cria_pilhas(baralho, tamPilhas, tamanhoArray);
    *gameOver = 0;
}

// Atualiza os valores para compatíveis com array e verifica se a jogada é válida
void jogar_Coluna(Pilhas *p, int posOrig[], int posDest[])
{
    posOrig[0]--;
    posOrig[1]--;
    posDest[0]--;
    if(valida_jogada(*p, posOrig, posDest))
    {
        printf("Jogada Inválida\n");
    }
    else
    {
        mover_cartas(p, posOrig, posDest);
    }
}

// Recebe a pilha e pede ao jogador as posições de jogadas.
// Chama ao jogar coluna com as informações
void pedir_jogada(Pilhas *p)
{
    int posOrig[2] = {0,0};
    int posDest[2] = {0,0};

    printf("Digite a coluna da carta que vai mover:");
    scanf("%d", &posOrig[0]);
    printf("Digite a linha da carta que vai mover:");
    scanf("%d", &posOrig[1]);

    printf("Digite a coluna destino:");
    scanf("%d", &posDest[0]);

    jogar_Coluna(p, posOrig, posDest);
}


// A partir da jogada selecionada, processa a jogada correta para o numero dado
void processar_jogada(struct carta baralho[], Pilhas *p, int *contagemBaralho, int tamPilhas[], int *gameOver)
{
    unsigned int jogadaEscolhida = opcao_inicio();
    
    //Sair do jogo
    if(jogadaEscolhida == 3)
    {
        printf("Saindo do jogo...\n");
        *gameOver = 2;
    }

    //RESTART
    else if(jogadaEscolhida == 2)
    {
        iniciar_jogo(baralho, p, contagemBaralho, tamPilhas, gameOver);
    }

    //Jogar
    else if(jogadaEscolhida==1)
    {
        pedir_jogada(p);
        return;
    }
}

// devolve o numero de cartas da mesma pinta seguidas
int sequencias(Pilhas p)
{
    int seq = 1;
    for (int i = p->numCartas-1; i>=1; i--)
    {
        if(!(((p->pilha)[i].naipe == (p->pilha)[i-1].naipe) && ((p->pilha)[i].valor == (p->pilha)[i-1].valor-1)))
            return seq;
        seq++;
    }
    return seq;
}

// verifica se existe uma jogada valida entre duas colunas
int verifica_colunas (Pilhas p, int coordenadaAtestar[], int colunaDest)
{
    if (colunaDest>=9 && coordenadaAtestar[0]>=9)
        return 1;
    if (colunaDest == coordenadaAtestar[0])
        colunaDest++;

    int coordenadasChegada[2] = {colunaDest, ((p->numCartas)-1)};

    if (valida_jogada(p, coordenadaAtestar, coordenadasChegada) == 0)
        return 0;
    
    return 1;
}

//subfunção do check gameover para ver se já não existem jogadas validas
int existe_jogadaValida (Pilhas p)
{
    Pilhas p2 = p, p3 = p;
    
    for (int colunaOrig = 0; colunaOrig<10; colunaOrig++)
    {
        if (p3->numCartas == 0)
            return 0;

        int coordenadaAtestar[2] = {colunaOrig, ((p3->numCartas)-(sequencias(p)))};

        for (int colunaDest = 0; colunaDest<10; colunaDest++)
        {
            if (verifica_colunas(p2, coordenadaAtestar, colunaDest) == 0)
                return 0;

            p2 = p2->prox;
        }
        p3 = p3->prox;
    }
    return 1;
}

// subfunção do check gameover para ver se ganhou (as pilhas de pintas estão todas feitas)
int verifica_ganhou(Pilhas p, Pilhas testeSeq, int i)
{
    if (testeSeq->numCartas != 0 && sequencias(testeSeq)==13)
    {
        int origem[2] = {i, (testeSeq->numCartas)-13};
        int destino[2] = {10+(testeSeq->pilha[testeSeq->numCartas-1].naipe),0};
        mover_cartas(&p, origem, destino);
    }

    Pilhas copas = procura_pilha(p, 10), espadas = copas->prox, ouros = espadas->prox, paus = ouros->prox;
    
    if (copas->numCartas != 0 && espadas->numCartas != 0 && ouros->numCartas != 0 && paus->numCartas != 0)
    {
        return 1;
    }
    return 0;
}

//verifica se o jogo acabou
int check_gameOver(Pilhas p)
{
    Pilhas testeSeq = p;

    for (int i = 0; i<10; i++)
    {
        if(verifica_ganhou(p, testeSeq, i))
        {
            printf("Ganhaste!\n");
            return 1;
        }
            
        testeSeq = testeSeq->prox;
    }

    if (existe_jogadaValida(p) == 1)
    {
        return 2;
    }

    return 0;
}
