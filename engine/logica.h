#ifndef LOGICA_H
#define LOGICA_H

#include <stdio.h>
#include "cartas.h"

int listarPaciencias(const char *caminho_pasta, char lista_de_caminhos[][512]);
EstadoJogo* lerPaciencia(const char *caminho_ficheiro);
int salvar_jogo(EstadoJogo *g, const char *caminho_save);
EstadoJogo* carregar_save(const char *caminho_save);
void listar_saves(void);
FlagsMovimento char_para_flag(char c);
int sequencias(Pilhas p);
void processar_auto_movimentos(EstadoJogo *g);
int acharLimite(Pilhas p);
int ganhou(Pilhas p, WinDef w);
void processar_jogada(EstadoJogo *g, struct baralho baralhos[], int *contagemBaralho, int tamPilhas[], int *gameOver);
void limpa_memoria_jogo(Pilhas *p);
void movimentoValido(EstadoJogo g, int posOrig[], int posDest[]);
void cria_baralho(struct carta * baralho);
void shuffle_baralho(struct carta *baralho);
void inicializa_baralhos (struct baralho *baralhos, int numBaralhos);
Pilhas cria_pilha(struct baralho *baralhos, int numCartas, int *contagemBaralho);
Pilhas cria_pilhas(struct baralho *baralhos, int numCartas[], int numPilhas);
Pilhas procura_pilha(Pilhas p, int pos);
void libera_memoria_cartas(Pilhas p);
void insere_cartas(Pilhas origem, Pilhas destino, int linha, int numMovidas);
int mover_cartas(Pilhas *p, int posOrig[], int posDest[]);
int pos_valida(int posOrig[], int posDest[]);
void iniciar_jogo(struct baralho baralhos[], Pilhas *p, int *contagemBaralho, int tamPilhas[], int *gameOver, int numBaralhos);
void jogar_Coluna(EstadoJogo *g, int posOrig[], int posDest[]);
void pedir_jogada(EstadoJogo *g);
int cartaChegadaEmenor (Pilhas p, int posOrig[], int posDest[]);
int cartaChegadaEmaior (Pilhas p, int posOrig[], int posDest[]);
int cartaMaiorOuMenor (Pilhas p, int posOrig[], int posDest[]);
int mesmoNaipeTopo (Pilhas p, int posOrig[], int posDest[]);
int mesmoNaipe (Pilhas p, int posOrig[]);
int mesmaCorTopo (Pilhas p, int posOrig[], int posDest[]);
int mesmaCor (Pilhas p, int posOrig[]);
int pilhaDestinoVazia (Pilhas p, int posDest[]);
int EsoUma (Pilhas p, int posOrig[]);
int crescenteVerif (Pilhas p, int posOrig[]);
int decrescenteVerif (Pilhas p, int posOrig[]);
int AStopo (Pilhas p, int posOrig[]);
int REIfundo (Pilhas p, int posOrig[]);
int ASfundo (Pilhas p, int posOrig[]);
int REItopo (Pilhas p, int posOrig[]);
int avalia_regra(FlagsMovimento regra, Pilhas p, int posOrig[], int posDest[]);
int valida_todas_regras(MovimentoDef mov, Pilhas p, int posOrig[], int posDest[]);

#endif