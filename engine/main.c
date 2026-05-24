#include <stdio.h>
#include <stdlib.h>
#include "logica.h"
#include "cartas.h"

int main() {
    
    char lista[50][512];
    
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

    return 0;
}
