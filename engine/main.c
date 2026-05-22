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

    //debug foda do big g
    if (escolhido != NULL) {
        printf("\n[SUCESSO] Voce selecionou o arquivo: %s\n", escolhido);
    } else {
        printf("\n[ERRO] Selecao invalida ou nenhum arquivo encontrado.\n");
    }

    return 0;
}
