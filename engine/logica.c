#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "logica.h"

// funcao para listar os arquivos e guardar os caminhos num array
int listarPaciencias(const char *caminho_pasta, char lista_de_caminhos[][512]) {
    DIR *pasta = opendir(caminho_pasta);
    struct dirent *arquivo;
    int quantidade = 0;

    printf("=== Menu de Paciencias ===\n");

    while ((arquivo = readdir(pasta)) != NULL) {
        // ignora as pastas ocultas do sistema
        if (strcmp(arquivo->d_name, ".") != 0 && strcmp(arquivo->d_name, "..") != 0) {        

            // monta o caminho completo
            char caminho_completo[512];
            snprintf(caminho_completo, sizeof(caminho_completo), "%s/%s", caminho_pasta, arquivo->d_name);

            // abre o arquivo em leitura
            FILE *texto = fopen(caminho_completo, "r");
            if (texto != NULL) {
                char nome_do_jogo[256];
                if (fgets(nome_do_jogo, sizeof(nome_do_jogo), texto) != NULL) {

                    nome_do_jogo[strcspn(nome_do_jogo, "\r\n")] = '\0'; //remove o enter
                
                    printf("%d - %s\n", quantidade + 1, nome_do_jogo);
                    strcpy(lista_de_caminhos[quantidade], caminho_completo); 
                    quantidade++;
                }
                fclose(texto);
            }
        }
    }
    closedir(pasta);
    return quantidade; 
}

EstadoJogo* lerPaciencia(const char *caminho_ficheiro) {
    FILE *ficheiro = fopen(caminho_ficheiro, "r");
    if (ficheiro == NULL) {
        printf("Nao foi possivel abrir o arquivo da paciencia");
        return NULL;
    }

    EstadoJogo *estado = (EstadoJogo*)malloc(sizeof(EstadoJogo));
    if (estado == NULL) {
        printf("Falha de alocacao de memoria \n");
        fclose(ficheiro);
        return NULL;
    }

    char linha[512];

    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        if (strncmp(linha, "BARALHOS:", 9) == 0) {
            sscanf(linha, "BARALHOS: %d", &estado->nBaralhos);
        }
    }

    fclose(ficheiro);
    return estado;
}
