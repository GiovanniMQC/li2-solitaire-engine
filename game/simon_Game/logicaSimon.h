#include "cartasSimon.h"

void cria_baralho(struct carta * baralho);
void shuffle_baralho(struct carta *baralho);
int achar_base_coluna(struct carta baralho[], int coluna);
void iniciar_jogo(struct carta baralho[], int *stock, struct carta *ultima, int *gameOver);
void puxar_Stock(struct carta baralho[], int *stock, struct carta *ultima);
int valores_Check(struct carta baralho[], struct carta *ultima, int jogadaEscolhida);
void jogada_Coluna(struct carta baralho[], int *stock, struct carta *ultima, int jogadaEscolhida);
void processar_jogada(struct carta baralho[], int *stock, struct carta *ultima, int *gameOver);