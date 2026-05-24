#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logica.h"
#include "cartas.h"

// Função auxiliar para tratar interativamente do carregamento do save
EstadoJogo* carrega_save_interativo(void) {
    listar_saves();
    
    int id_save;
    printf("\nDigite o numero do save que deseja carregar: ");
    if (scanf("%d", &id_save) != 1) {
        printf("Entrada invalida.\n");
        return NULL;
    }
    
    char caminho_save[512];
    sprintf(caminho_save, "saves/save_%d.txt", id_save);
    EstadoJogo *jogo = carregar_save(caminho_save);
    if (jogo == NULL) {
        printf("Erro ao carregar o save. Verifique se o numero esta correto.\n");
        return NULL;
    }
    printf("\nSave carregado com sucesso!\n");
    return jogo;
}

// Função auxiliar para carregar um save do disco
EstadoJogo* carrega_save_ou_paciencia(const char *escolhido) {
    if (strcmp(escolhido, "LOAD_SAVE") == 0) {
        return carrega_save_interativo();
    }
    
    printf("\nVoce selecionou: %s\n", escolhido);
    EstadoJogo *jogo = lerPaciencia(escolhido);
    if (jogo == NULL) {
        printf("Erro ao carregar a paciencia.\n");
        return NULL;
    }
    return jogo;
}

// Função para liberar toda a memória da estrutura de jogo
void libera_memoria_final(EstadoJogo *jogo) {
    limpa_memoria_jogo(&(jogo->pilhas));
    free(jogo->nome_paciencia);
    free(jogo->caminho_ficheiro);
    free(jogo->win_args.tipo);
    free(jogo->win_args.numCartas);
    
    libera_movimentos_array(jogo->mov_perm, jogo->qts_mov_perm);
    libera_movimentos_array(jogo->auto_movs, jogo->qts_auto_movs);
    
    free(jogo);
}

// Loop principal do jogo
void loop_principal(EstadoJogo *jogo, struct baralho b[], int *contagemBaralho, int tamPilhas[], int *gameOver) {
    while (*gameOver == 0) {
        processar_auto_movimentos(jogo);
        printf("\n");
        print_pilhas(jogo->pilhas, acharLimite(jogo->pilhas));
        
        if (ganhou(jogo->pilhas, jogo->win_args) == 0) {
            printf("\nParabens! Voce ganhou %s!\n", jogo->nome_paciencia);
            *gameOver = 1;
        } else {
            processar_jogada(jogo, b, contagemBaralho, tamPilhas, gameOver);
        }
    }
}

// Função auxiliar para tratar do menu inicial e carregamento do jogo
EstadoJogo* inicializar_paciencia() {
    char lista[50][512];
    const char *pasta = "paciencias";

    int total = listarPaciencias(pasta, lista);
    char *escolhido = escolherPaciencia(lista, total);

    if (escolhido == NULL) {
        printf("\nSelecao invalida ou nenhum arquivo encontrado.\n");
        return NULL;
    }

    return carrega_save_ou_paciencia(escolhido);
}

int main() {
    EstadoJogo *jogo = inicializar_paciencia();
    if (jogo == NULL) return 1;

    printf("Nome do Jogo: %s\n", jogo->nome_paciencia);
    printf("Baralhos: %d | Pilhas: %d | Movimentos: %d\n",
           jogo->nBaralhos, jogo->qts_pilhas, jogo->qts_mov_perm);

    struct baralho b[jogo->nBaralhos];
    int contagemBaralho = 0;
    int tamPilhas[jogo->qts_pilhas];
    int gameOver = 0;

    loop_principal(jogo, b, &contagemBaralho, tamPilhas, &gameOver);
    
    libera_memoria_final(jogo);

    return 0;
}