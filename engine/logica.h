<<<<<<< HEAD
#include "estadoJogoPrototipo.h"

int listarPaciencias(const char *caminho_pasta, char lista_de_caminhos[][512]);
EstadoJogo* lerPaciencia(const char *caminho_ficheiro);
=======
#ifndef LOGICA_SIMON_H
#define LOGICA_SIMON_H

#include "cartasSimon.h"
#include <stdio.h>

int listarPaciencias(const char *caminho_pasta, char lista_de_caminhos[][512]);
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
void jogar_Coluna(EstadoJogo *g, int posOrig[], int posDest[]);
void pedir_jogada(EstadoJogo *g);
void processar_jogada(EstadoJogo *g, struct baralho baralhos[], int *contagemBaralho, int tamPilhas[], int *gameOver);
int check_gameOver(Pilhas p);
int sequencias(Pilhas p);
int verifica_colunas (Pilhas p, int coordenadaAtestar[], int colunaDest);
int existe_jogadaValida (Pilhas p);
int verifica_ganhou(Pilhas p, Pilhas testeSeq, int i);
int cartaChegadaEmenor (Pilhas p, int posOrig[], int posDest[]);
int cartaChegadaEmaior (Pilhas p, int posOrig[], int posDest[]);
int cartaMaiorOuMenor (Pilhas p, int posOrig[], int posDest[]);
int mesmoNaipeTopo (Pilhas p, int posOrig[], int posDest[]);
int mesmoNaipe (Pilhas p, int posOrig[]);
int mesmaCorTopo (Pilhas p, int posOrig[], int posDest[]);
int mesmaCor (Pilhas p, int posOrig[]);
int pilhaDestinoVazia (Pilhas p, int posDest[]);
void salva_valor(FILE *save, int valor);
void salva_naipe(FILE *save, int naipe);
void salva_pilhas(FILE *save, Pilhas p);
int salvaJogo (EstadoJogo g, int contagemSaves);
void lerSaves(int numSave, EstadoJogo g);
int EsoUma (Pilhas p, int posOrig[]);
int crescenteVerif (Pilhas p, int posOrig[]);
int decrescenteVerif (Pilhas p, int posOrig[]);
int AStopo (Pilhas p, int posOrig[]);
int REIfundo (Pilhas p, int posOrig[]);
int ASfundo (Pilhas p, int posOrig[]);
int REItopo (Pilhas p, int posOrig[]);
void ganhouAjuda (WinDef w, int *i, char *temp);
int tamanhoS (char *s);
int ganhou (Pilhas p, WinDef w);
int avalia_regra(FlagsMovimento regra, Pilhas p, int posOrig[], int posDest[]);
int valida_todas_regras(MovimentoDef mov, Pilhas p, int posOrig[], int posDest[]);
void movimentoValido(EstadoJogo g, int posOrig[], int posDest[]);
void processar_auto_movimentos(EstadoJogo *g);

#endif
>>>>>>> e158f55fbb9d6b2cb0efc3308ea6f16955a0b8f1
