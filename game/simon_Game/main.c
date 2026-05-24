#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include <dirent.h> // checar arquivos, Checar wikibooks para entender
#include <string.h> // necessario para strdup()
#include "cartasSimon.h"
#include "logicaSimon.h"

EstadoJogo criar_estado_simplesimon_manual() {
    EstadoJogo jogo;
    jogo.nome_paciencia = strdup("Simple Simon");
    jogo.nBaralhos = 1;
    jogo.qts_pilhas = 14; // 10(TAB) + 4(FUND)

    struct baralho b[1];
    inicializa_baralhos(b, 1);

    int contagem = 0;
    Pilhas inicio = NULL;
    Pilhas atual = NULL;
    
    int tab_sizes[10] = {8, 8, 8, 7, 6, 5, 4, 3, 2, 1};
    
    // Cria 10 pilhas para o Tabuleiro (TAB) com a distribuição do Simple Simon
    for (int i = 0; i < 10; i++) {
        Pilhas nova = cria_pilha(b, tab_sizes[i], &contagem);
        nova->tipo_Pilha = strdup("TAB"); 
        nova->flags = strdup("="); // '=' faz com que mostre todas as cartas
        if (inicio == NULL) {
            inicio = nova;
            atual = nova;
        } else {
            atual->prox = nova;
            atual = nova;
        }
    }
    
    // Cria 4 pilhas para a Fundação (FUND) inicialmente vazias
    for (int i = 0; i < 4; i++) {
        Pilhas nova = cria_pilha(b, 0, &contagem);
        nova->tipo_Pilha = strdup("FUND");
        nova->flags = strdup("=");
        atual->prox = nova;
        atual = nova;
    }
    
    jogo.pilhas = inicio;
    
    // Configuracao dos Movimentos
    jogo.qts_mov_perm = 4;
    jogo.mov_perm = malloc(4 * sizeof(MovimentoDef));
    
    // MOV TAB TAB <
    jogo.mov_perm[0].tipo_origem = strdup("TAB");
    jogo.mov_perm[0].tipo_destino = strdup("TAB");
    jogo.mov_perm[0].qts_flags = 1;
    jogo.mov_perm[0].flags = malloc(sizeof(FlagsMovimento));
    jogo.mov_perm[0].flags[0] = VALOR_INFERIOR; 
    
    // MOV TAB TAB +[m<
    jogo.mov_perm[1].tipo_origem = strdup("TAB");
    jogo.mov_perm[1].tipo_destino = strdup("TAB");
    jogo.mov_perm[1].qts_flags = 4;
    jogo.mov_perm[1].flags = malloc(4 * sizeof(FlagsMovimento));
    jogo.mov_perm[1].flags[0] = PODE_SER_SEQUENCIAS; 
    jogo.mov_perm[1].flags[1] = ORDENADAS_DECRESCENTE;
    jogo.mov_perm[1].flags[2] = MESMO_NAIPE;
    jogo.mov_perm[1].flags[3] = VALOR_INFERIOR; 
    
    // MOV TAB TAB V
    jogo.mov_perm[2].tipo_origem = strdup("TAB");
    jogo.mov_perm[2].tipo_destino = strdup("TAB");
    jogo.mov_perm[2].qts_flags = 1;
    jogo.mov_perm[2].flags = malloc(sizeof(FlagsMovimento));
    jogo.mov_perm[2].flags[0] = PILHA_VAZIA; 
    
    // MOV TAB TAB +[mV
    jogo.mov_perm[3].tipo_origem = strdup("TAB");
    jogo.mov_perm[3].tipo_destino = strdup("TAB");
    jogo.mov_perm[3].qts_flags = 4;
    jogo.mov_perm[3].flags = malloc(4 * sizeof(FlagsMovimento));
    jogo.mov_perm[3].flags[0] = PODE_SER_SEQUENCIAS; 
    jogo.mov_perm[3].flags[1] = ORDENADAS_DECRESCENTE;
    jogo.mov_perm[3].flags[2] = MESMO_NAIPE;
    jogo.mov_perm[3].flags[3] = PILHA_VAZIA; 
    
    // Configuracao da Condicao de Vitoria (Simple Simon: todas as 10 pilhas TAB devem estar vazias)
    jogo.win_args.tipo = strdup("TAB TAB TAB TAB TAB TAB TAB TAB TAB TAB");
    jogo.win_args.qntsWins = 10;
    jogo.win_args.numCartas = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        jogo.win_args.numCartas[i] = 0;
    }

    // Movimentos Automaticos (AUTO TAB FUND +[mKaV)
    jogo.qts_auto_movs = 1;
    jogo.auto_movs = malloc(1 * sizeof(MovimentoDef));
    jogo.auto_movs[0].tipo_origem = strdup("TAB");
    jogo.auto_movs[0].tipo_destino = strdup("FUND");
    jogo.auto_movs[0].qts_flags = 6;
    jogo.auto_movs[0].flags = malloc(6 * sizeof(FlagsMovimento));
    jogo.auto_movs[0].flags[0] = PODE_SER_SEQUENCIAS;
    jogo.auto_movs[0].flags[1] = ORDENADAS_DECRESCENTE;
    jogo.auto_movs[0].flags[2] = MESMO_NAIPE;
    jogo.auto_movs[0].flags[3] = FUNDO_REI;
    jogo.auto_movs[0].flags[4] = TOPO_AS;
    jogo.auto_movs[0].flags[5] = PILHA_VAZIA;
    
    return jogo;
}

int main(void)
{
    setlocale(LC_ALL, "");
    
    printf("--- Teste do Estado de Jogo Manual (Simple Simon) ---\n\n");
    EstadoJogo jogo = criar_estado_simplesimon_manual();
    
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
            // FIX não deve ser entregue assim, não é permitido BREAK
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
    
    // Libertar movimentos automáticos da memória
    if (jogo.auto_movs != NULL) {
        for (int i=0; i < jogo.qts_auto_movs; i++) {
            free(jogo.auto_movs[i].tipo_origem);
            free(jogo.auto_movs[i].tipo_destino);
            free(jogo.auto_movs[i].flags);
        }
        free(jogo.auto_movs);
    }
    
    return EXIT_SUCCESS;
}