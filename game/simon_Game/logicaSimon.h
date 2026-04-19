#include "cartasSimon.h"

void cria_baralho(struct carta * baralho);
void shuffle_baralho(struct carta *baralho);
Pilhas cria_pilha(struct carta *baralho, int numCartas, int *contagemBaralho);
Pilhas procura_pilha(Pilhas p, int pos);
void libera_memoria_cartas(Pilhas p);
void insere_cartas(Pilhas origem, Pilhas destino, int linha, int numMovidas);
void corrige_seq_cartas(Pilhas p, int linha);
int mover_cartas(Pilhas *p, int posOrig[], int posDest[]);
int acharLimite(Pilhas p);
int carta_check (Pilhas pilhaOrigem, Pilhas pilhaDestino, struct carta origem, struct carta chegada, int origLin, int naipeSelecionado);
int valida_jogada(Pilhas *p, int posOrig[], int posDest[]);

void iniciar_jogo(struct carta baralho[], Pilhas *p, int *contagemBaralho, int tamPilhas[], int *gameOver);
int valores_Check(struct carta baralho[], struct carta *ultima, int jogadaEscolhida);
void jogar_Coluna(Pilhas *p, int posOrig[], int posDest[]);
void processar_jogada(struct carta baralho[], Pilhas *p, int *contagemBaralho, int tamPilhas[], int *gameOver);
