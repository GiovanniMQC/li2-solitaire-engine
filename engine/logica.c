#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

// Função simples apenas para listar os nomes na tela
void listar_nomes(const char *caminho_pasta) {
    DIR *dir;
    struct dirent *entrada;
    int index = 1;

    dir = opendir(caminho_pasta);
    if (dir == NULL) return;

    printf("--- Escolha a Paciência ---\n");

    // Passa por cada arquivo na pasta
    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        }

        char caminho_arquivo[512];
        snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s/%s", caminho_pasta, entrada->d_name);

        FILE *arquivo = fopen(caminho_arquivo, "r");
        if (arquivo == NULL) continue;

        char primeira_linha[256];
        if (fgets(primeira_linha, sizeof(primeira_linha), arquivo) != NULL) {
            // Remove a quebra de linha do texto para não estragar o print
            primeira_linha[strcspn(primeira_linha, "\r\n")] = '\0';
            
            printf("%d - %s\n", index, primeira_linha);
            index++; // Aumenta o contador para a próxima linha
        }
        fclose(arquivo);
    }
    closedir(dir);
}

// Função que pede um número e devolve o caminho do arquivo correspondente àquela ordem
char* escolher_paciencia(const char *caminho_pasta) {
    int escolha = 0;
    printf("Digite o número da paciência desejada: ");
    scanf("%d", &escolha);

    DIR *dir = opendir(caminho_pasta);
    struct dirent *entrada;
    int index = 1;

    if (dir == NULL) return NULL;

    // Percorre a pasta de novo para encontrar o arquivo no índice escolhido
    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        }

        // Se o índice atual bater com o número que o usuário digitou...
        if (index == escolha) {
            char *caminho_escolhido = malloc(512);
            snprintf(caminho_escolhido, 512, "%s/%s", caminho_pasta, entrada->d_name);
            closedir(dir);
            return caminho_escolhido; // Devolve apenas aquele que encontrou
        }
        index++;
    }

    closedir(dir);
    return NULL;
}
