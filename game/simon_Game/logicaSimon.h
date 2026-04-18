#include "cartasSimon.h"

void cria_baralho(struct carta * baralho);
void shuffle_baralho(struct carta *baralho);
void iniciar_jogo(struct carta baralho[], Pilhas *p, int *contagemBaralho, int tamPilhas[], int *gameOver);
void jogar_Coluna(Pilhas *p, int posOrig[], int posDest[]);
void processar_jogada(struct carta baralho[], Pilhas *p, int *contagemBaralho, int tamPilhas[], int *gameOver);
int acharLimite(Pilhas p);