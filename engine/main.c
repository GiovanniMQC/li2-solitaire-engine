#include <stdio.h>
#include <stdlib.h>

int listarPaciencias(const char *caminho_pasta, char lista_de_caminhos[][512]);
char* escolherPaciencia(char lista_de_caminhos[][512], int quantidade);

int main() {
    // Criamos o espaço para guardar os caminhos (50 caminhos de 512 letras)
    char lista[50][512];
    
    // Defina aqui o nome da pasta onde estão seus .txt
    const char *pasta = "paciencias"; 

    // 1. Lista os jogos e guarda os caminhos
    int total = listarPaciencias(pasta, lista);

    // 2. Pede para o usuário escolher
    char *escolhido = escolherPaciencia(lista, total);

    if (escolhido != NULL) {
        printf("\n[SUCESSO] Voce selecionou o arquivo: %s\n", escolhido);
    } else {
        printf("\n[ERRO] Selecao invalida ou nenhum arquivo encontrado.\n");
    }

    return 0;
}
