#include <stdio.h>
#include <stdlib.h>
#include "logica.h"
#include "cartas.h"

int main() {
    
    char lista[50][512];
    EstadoJogo jogo;
    
    const char *pasta = "paciencias"; 

    // lista os jogos e guarda os caminhos
    int total = listarPaciencias(pasta, lista);

    // requer a escolha, retorna o caminho da paciencia escolhida
    char *escolhido = escolherPaciencia(lista, total);

    if (escolhido != NULL) {
        printf("\nVoce selecionou o arquivo: %s\n", escolhido);
        
        EstadoJogo *estado = lerPaciencia(escolhido);
        if (estado != NULL) {
            printf("[INFO] Estado do jogo inicializado com sucesso (Baralhos: %d)\n", estado->nBaralhos);
        }
    } 
    else {
        printf("\nSelecao invalida ou nenhum arquivo encontrado.\n");
    }

    printf("Nome do Jogo: %s\n", jogo.nome_paciencia);

    struct baralho b[1];
    int contagemBaralho = 0;
    int tamPilhas[14] = {0};
    int gameOver = 0;

    while (gameOver == 0) {
        // Varre e executa os movimentos automaticos antes de renderizar o tabuleiro
        processar_auto_movimentos(&jogo);

        printf("\n");
        print_pilhas(jogo.pilhas, acharLimite(jogo.pilhas));

        if (ganhou(jogo.pilhas, jogo.win_args) == 0) {
            printf("\nParabéns! Você ganhou o jogo %s!\n", jogo.nome_paciencia);
            gameOver = 1;
        } else {
            processar_jogada(&jogo, b, &contagemBaralho, tamPilhas, &gameOver);
        }
    }

    // Liberta a memoria da estrutura ligada das pilhas
    limpa_memoria_jogo(&(jogo.pilhas));
    free(jogo.win_args.tipo);
    free(jogo.win_args.numCartas);

    // Libertar array de movimentos permitidos da memória
    if (jogo.mov_perm != NULL) {
        for (int i=0; i < jogo.qts_mov_perm; i++) {
            free(jogo.mov_perm[i].tipo_origem);
            free(jogo.mov_perm[i].tipo_destino);
            free(jogo.mov_perm[i].flags);
        }
        free(jogo.mov_perm);
    }
    
    // Libertar movimentos automáticos da memória
    if (jogo.auto_movs != NULL) {
        for (int i=0; i < jogo.qts_auto_movs; i++) {
            free(jogo.auto_movs[i].tipo_origem);
            free(jogo.auto_movs[i].tipo_destino);
            free(jogo.auto_movs[i].flags);
        }
        free(jogo.auto_movs);
    }
    
    return 0;
}
