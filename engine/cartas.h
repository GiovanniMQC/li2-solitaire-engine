#ifndef CARTAS_SIMON_H
#define CARTAS_SIMON_H

#include "estadoJogoPrototipo.h"

char* escolherPaciencia(char lista_de_caminhos[][512], int quantidade);

void print_valor(struct carta c);
void print_naipe(struct carta c);
void print_carta(struct carta c);
void print_pilhas(Pilhas p, int lim);
void print_naipes_completos(Pilhas p);
int opcao_inicio();

#endif