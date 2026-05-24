#include "estadoJogoPrototipo.h"

int listarPaciencias(const char *caminho_pasta, char lista_de_caminhos[][512]);
EstadoJogo* lerPaciencia(const char *caminho_ficheiro);

// Protótipos necessários para o main.c
void processar_auto_movimentos(EstadoJogo *g);
int acharLimite(Pilhas p);
int ganhou(Pilhas p, WinDef w);
void processar_jogada(EstadoJogo *g, struct baralho baralhos[], int *contagemBaralho, int tamPilhas[], int *gameOver);
void limpa_memoria_jogo(Pilhas *p);
void movimentoValido(EstadoJogo g, int posOrig[], int posDest[]);
