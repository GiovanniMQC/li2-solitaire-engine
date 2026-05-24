#include "cartas.h"
#include "logica.h"
#include "estadoJogoPrototipo.h" 

int valida_jogada(Pilha p_falsa, int posOrigem, int posDestino);
void corrige_seq_cartas(Pilha *p, int valor);
int check_gameOver(Tabuleiro *t);
void cria_baralho(struct carta * baralho);
void shuffle_baralho(struct carta *baralho);
int achar_base_coluna(struct carta baralho[], int coluna);
void iniciar_jogo(struct carta baralho[], int *stock, struct carta *ultima, int *gameOver);
void puxar_Stock(struct carta baralho[], int *stock, struct carta *ultima);
int valores_Check(struct carta baralho[], struct carta *ultima, int jogadaEscolhida);
void jogada_Coluna(struct carta baralho[], int *stock, struct carta *ultima, int jogadaEscolhida);
void processar_jogada(struct carta baralho[], int *stock, struct carta *ultima, int *gameOver);
int check_gameOver(Tabuleiro *t);
int carta_check(Pilha *orig, Pilha *dest, struct carta c1, struct carta c2, int valor, int naipe); // Ajusta os tipos se forem diferentes no teu logica.c
int sequencias(Pilha p);
int existe_jogadaValida(Pilha *p_unica);
