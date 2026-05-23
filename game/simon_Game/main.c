#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include <dirent.h> // checar arquivos, Checar wikibooks para entender
#include <string.h> // necessario para strdup()
#include "cartasSimon.h"
#include "logicaSimon.h"

EstadoJogo criar_estado_golf_manual() {
    EstadoJogo jogo;
    jogo.nome_paciencia = strdup("Golf");
    jogo.nBaralhos = 1;
    jogo.qts_pilhas = 9; // 7(TAB) + 1(DESCARTE) + 1(STOCK)

    struct baralho b[1];
    inicializa_baralhos(b, 1);

    int contagem = 0;
    Pilhas inicio = NULL;
    Pilhas atual = NULL;
    
    // Cria 7 pilhas para o Tabuleiro (TAB) com 5 cartas inicialmente
    for (int i = 0; i < 7; i++) {
        Pilhas nova = cria_pilha(b, 5, &contagem);
        nova->tipo_Pilha = strdup("TAB="); // '=' faz com que mostre todas as cartas
        if (inicio == NULL) {
            inicio = nova;
            atual = nova;
        } else {
            atual->prox = nova;
            atual = nova;
        }
    }
    
    // Cria 1 pilha para o Descarte com 1 carta inicial
    Pilhas descarte = cria_pilha(b, 1, &contagem);
    descarte->tipo_Pilha = strdup("DESCARTE=");
    atual->prox = descarte;
    atual = descarte;
    
    // Cria 1 pilha para o Stock com 16 cartas iniciais
    Pilhas stock = cria_pilha(b, 16, &contagem);
    stock->tipo_Pilha = strdup("STOCK_"); // O '_' garante que a pilha fique invisivel no print geral
    atual->prox = stock;
    atual = stock;
    
    jogo.pilhas = inicio;
    
    // Configuracao dos Movimentos
    jogo.qts_mov_perm = 2;
    jogo.mov_perm = malloc(2 * sizeof(MovimentoDef));
    
    // MOV STOCK DESCARTE *
    jogo.mov_perm[0].tipo_origem = strdup("STOCK_");
    jogo.mov_perm[0].tipo_destino = strdup("DESCARTE=");
    jogo.mov_perm[0].qts_flags = 1;
    jogo.mov_perm[0].flags = malloc(sizeof(FlagsMovimento));
    jogo.mov_perm[0].flags[0] = NAO_HA_RESTRICOES; 
    
    // MOV TAB DESCARTE ~
    jogo.mov_perm[1].tipo_origem = strdup("TAB=");
    jogo.mov_perm[1].tipo_destino = strdup("DESCARTE=");
    jogo.mov_perm[1].qts_flags = 1;
    jogo.mov_perm[1].flags = malloc(sizeof(FlagsMovimento));
    jogo.mov_perm[1].flags[0] = OU; 
    
    // Configuracao da Condicao de Vitoria (Golf: todas as 7 pilhas TAB= devem estar vazias)
    jogo.win_args.tipo = strdup("TAB= TAB= TAB= TAB= TAB= TAB= TAB=");
    jogo.win_args.qntsWins = 7;
    jogo.win_args.numCartas = malloc(7 * sizeof(int));
    for (int i = 0; i < 7; i++) {
        jogo.win_args.numCartas[i] = 0;
    }

    jogo.auto_movs = NULL;
    jogo.qts_auto_movs = 0;
    
    return jogo;
}

int main(void)
{
    setlocale(LC_ALL, "");
    
    printf("--- Teste do Estado de Jogo Manual (Golf) ---\n\n");
    EstadoJogo jogo = criar_estado_golf_manual();
    
    printf("Nome do Jogo: %s\n", jogo.nome_paciencia);

    struct baralho b[1];
    int contagemBaralho = 0;
    int tamPilhas[14] = {0};
    int gameOver = 0;

    while (gameOver == 0) {
        printf("\n");
        print_pilhas(jogo.pilhas, acharLimite(jogo.pilhas));

        if (ganhou(jogo.pilhas, jogo.win_args) == 0) {
            printf("\nParabéns! Você ganhou o jogo %s!\n", jogo.nome_paciencia);
            gameOver = 1;
            break;
        }

        processar_jogada(&jogo, b, &contagemBaralho, tamPilhas, &gameOver);
    }

    // Liberta a memoria da estrutura ligada das pilhas
    limpa_memoria_jogo(&(jogo.pilhas));
    free(jogo.win_args.tipo);
    free(jogo.win_args.numCartas);

    // Libertar array de movimentos permitidos da memória
    for (int i=0; i < jogo.qts_mov_perm; i++) {
        free(jogo.mov_perm[i].tipo_origem);
        free(jogo.mov_perm[i].tipo_destino);
        free(jogo.mov_perm[i].flags);
    }
    free(jogo.mov_perm);
    
    return EXIT_SUCCESS;
}