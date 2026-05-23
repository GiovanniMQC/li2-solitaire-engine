#ifndef LOGICA_SIMON_H
#define LOGICA_SIMON_H

#include "cartasSimon.h"
#include <stdio.h>

void cria_baralho(struct carta * baralho);
void shuffle_baralho(struct carta *baralho);
void inicializa_baralhos (struct baralho *baralhos, int numBaralhos);
Pilhas cria_pilha(struct baralho *baralhos, int numCartas, int *contagemBaralho);
Pilhas cria_pilhas(struct baralho *baralhos, int numCartas[], int numPilhas);
Pilhas procura_pilha(Pilhas p, int pos);
void libera_memoria_cartas(Pilhas p);
void insere_cartas(Pilhas origem, Pilhas destino, int linha, int numMovidas);
void corrige_seq_cartas(Pilhas p, int linha);
int mover_cartas(Pilhas *p, int posOrig[], int posDest[]);
int acharLimite(Pilhas p);
int carta_check (Pilhas pilhaOrigem, Pilhas pilhaDestino, struct carta origem, struct carta chegada, int origLin, int naipeSelecionado);
int pos_valida(int posOrig[], int posDest[]);
int valida_jogada(Pilhas p, int posOrig[], int posDest[]);
void limpa_memoria_jogo(Pilhas *p);
void iniciar_jogo(struct baralho baralhos[], Pilhas *p, int *contagemBaralho, int tamPilhas[], int *gameOver, int numBaralhos);
int valores_Check(struct carta baralho[], struct carta *ultima, int jogadaEscolhida);
void jogar_Coluna(Pilhas *p, int posOrig[], int posDest[]);
void pedir_jogada(Pilhas *p);
void processar_jogada(struct baralho baralhos[], Pilhas *p, int *contagemBaralho, int tamPilhas[], int *gameOver, int numBaralhos);
int check_gameOver(Pilhas p);
int sequencias(Pilhas p);
int verifica_colunas (Pilhas p, int coordenadaAtestar[], int colunaDest);
int existe_jogadaValida (Pilhas p);
int verifica_ganhou(Pilhas p, Pilhas testeSeq, int i);
int cartaChegadaEmenor (Pilhas *p, int posOrig[], int posDest[]);
int cartaChegadaEmaior (Pilhas *p, int posOrig[], int posDest[]);
int cartaMaiorOuMenor (Pilhas *p, int posOrig[], int posDest[]);
int mesmoNaipe (Pilhas *p, int posOrig[], int posDest[]);
int mesmaCor (Pilhas *p, int posOrig[], int posDest[]);
int pilhaDestinoVazia (Pilhas *p, int posOrig[], int posDest[]);
void salva_pilhas(FILE *save, Pilhas p);
int salvaJogo (EstadoJogo g, int contagemSaves);
void lerSaves(int numSave, EstadoJogo g);

#endif