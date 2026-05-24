#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logica.h"
#include "cartas.h"

int main() {
    
    char lista[50][512];
    
    const char *pasta = "paciencias"; 

    // Lista os jogos e guarda os caminhos
    int total = listarPaciencias(pasta, lista);

    // Requer a escolha, retorna o caminho da paciencia escolhida
    char *escolhido = escolherPaciencia(lista, total);

    if (escolhido == NULL) {
        printf("\nSelecao invalida ou nenhum arquivo encontrado.\n");
        return 1;
    }

    EstadoJogo *jogo = NULL;

    if (strcmp(escolhido, "LOAD_SAVE") == 0) {
        listar_saves();
        
        int id_save;
        printf("\nDigite o numero do save que deseja carregar: ");
        if (scanf("%d", &id_save) != 1) {
            printf("Entrada invalida.\n");
            return 1;
        }
        
        char caminho_save[512];
        sprintf(caminho_save, "saves/save_%d.txt", id_save);
        jogo = carregar_save(caminho_save);
        if (jogo == NULL) {
            printf("Erro ao carregar o save. Verifique se o numero esta correto.\n");
            return 1;
        }
        printf("\nSave carregado com sucesso!\n");
    } else {
        printf("\nVoce selecionou: %s\n", escolhido);
        jogo = lerPaciencia(escolhido);
        if (jogo == NULL) {
            printf("Erro ao carregar a paciencia.\n");
            return 1;
        }
    }

    printf("Nome do Jogo: %s\n", jogo->nome_paciencia);
    printf("Baralhos: %d | Pilhas: %d | Movimentos: %d\n",
           jogo->nBaralhos, jogo->qts_pilhas, jogo->qts_mov_perm);

    struct baralho b[jogo->nBaralhos];
    int contagemBaralho = 0;
    int tamPilhas[jogo->qts_pilhas];
    int gameOver = 0;

    while (gameOver == 0) {
        // Executa movimentos automaticos antes de renderizar
        processar_auto_movimentos(jogo);

        printf("\n");
        print_pilhas(jogo->pilhas, acharLimite(jogo->pilhas));

        // ganhou() retorna 0 quando venceu, 1 caso contrario
        if (ganhou(jogo->pilhas, jogo->win_args) == 0) {
            printf("\nParabens! Voce ganhou %s!\n", jogo->nome_paciencia);
            gameOver = 1;
        } else {
            processar_jogada(jogo, b, &contagemBaralho, tamPilhas, &gameOver);
        }
    }

    // Liberta memoria das pilhas
    limpa_memoria_jogo(&(jogo->pilhas));
    free(jogo->nome_paciencia);
    free(jogo->caminho_ficheiro);
    free(jogo->win_args.tipo);
    free(jogo->win_args.numCartas);

    // Liberta movimentos permitidos
    if (jogo->mov_perm != NULL) {
        for (int i = 0; i < jogo->qts_mov_perm; i++) {
            free(jogo->mov_perm[i].tipo_origem);
            free(jogo->mov_perm[i].tipo_destino);
            free(jogo->mov_perm[i].flags);
        }
        free(jogo->mov_perm);
    }
    
    // Liberta movimentos automaticos
    if (jogo->auto_movs != NULL) {
        for (int i = 0; i < jogo->qts_auto_movs; i++) {
            free(jogo->auto_movs[i].tipo_origem);
            free(jogo->auto_movs[i].tipo_destino);
            free(jogo->auto_movs[i].flags);
        }
        free(jogo->auto_movs);
    }

    free(jogo);

    return 0;
}