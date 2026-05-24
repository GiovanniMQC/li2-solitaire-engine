#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include <dirent.h>
#include <string.h>
#include "cartas.h"
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

// Recebe um array de struct carta, e para cada slot (52 cartas), atribui o valor e naipe de forma consecutiva
void cria_baralho(struct carta *baralho)
{
    int val = 13;
    int np = 0;

    for(int idx=0; idx<52; idx++)
    {
        baralho[idx].valor = val;
        baralho[idx].naipe = np;

        val--;

        if(val<1){
            val = 13;
            np++;
        }
    }
}

// Pega num baralho ja preenhido e mistura a ordem das cartas de forma aleatoria
void shuffle_baralho(struct carta *baralho)
{
    srand(time(NULL)); //seed para randomizar baralho (segundos atuais)
    int idx;
    
    for(int i=0; i<52; i++)
    {
        struct carta temp;
        
        idx = rand() % 52;
        
        //guarda a carta e troca as posicoes para nao repetir cartas
        temp = baralho[idx];
        baralho[idx] = baralho[i];
        baralho[i]= temp;   
    }
    
}

// Recebe uma array que terá os baralhos do jogo e um int com quantos baralhos devem exitir.
void inicializa_baralhos (struct baralho *baralhos, int numBaralhos)
{
    for(int i = 0; i < numBaralhos; i++)
    {
        cria_baralho(baralhos[i].cartas);
        shuffle_baralho(baralhos[i].cartas);
    }
}

// Cria uma pilha a partir de um baralho e guarda o número de cartas na pilha. Coloca a nova pilha no fim da lista de células.
// Retorna o endereço da nova pilha
Pilhas cria_pilha(struct baralho *baralhos, int numCartas, int *contagemBaralho)
{
    // Um apontador para o início de uma pilha.
    Pilhas p = malloc(sizeof(struct celula));

    // FIX não está recebendo valores do DSL, inicia como NULL para não dar crash
    p->tipo_Pilha = NULL;
    p->flags = NULL;
    p->numCartas = numCartas;
    p->pilha = malloc(sizeof(struct carta) * numCartas);
    p->prox = NULL;

    // Um loop para criar uma pilha
    for(int i = 0; i < numCartas; i++, (*contagemBaralho)++)
    {
        // Escolhe automaticamente o baralho e a carta baseados na quantidade já puxada
        int numBaralho = (*contagemBaralho) / 52;
        int idxCarta = (*contagemBaralho) % 52;
        p->pilha[i].naipe = baralhos[numBaralho].cartas[idxCarta].naipe;
        p->pilha[i].valor = baralhos[numBaralho].cartas[idxCarta].valor;
    }
    return p;
}


// TODO Tem que atualizar para definir ow tipos das pilhas
// Cria uma lista de pilhas a partir de um baralho, número de pilhas e recebe uma array que guarda em sequência os tamanhos das pilhas.
Pilhas cria_pilhas(struct baralho *baralhos, int numCartas[], int numPilhas)
{
    // Se a array for nula ou a quantidade for inválida, retorna NULL
    if(numCartas == NULL || numPilhas <= 0) return NULL;
    // Variável que guarda quantas cartas foram percorridas no baralho.
    int contagemBaralho = 0;

    // Cria a primeira pilha.
    Pilhas inicioPilha = cria_pilha(baralhos, numCartas[0], &contagemBaralho);

    Pilhas tmp = inicioPilha;
    for(int i = 1; i < numPilhas; i++)
    {
        Pilhas nova = cria_pilha(baralhos, numCartas[i], &contagemBaralho);
        tmp->prox = nova;
        tmp = nova;
    }

    return inicioPilha;
}

// Recebe a pilha inicial e procura a partir de uma posição a pilha destino
// Retorna a pilha destino
Pilhas procura_pilha(Pilhas p, int pos)
{
    // Cria um apontador que será usado para procurar a pilha desejada
    Pilhas pilhaResultado = p;

    // Mudando o endereço para a coluna correta
    for(int i = 0; i < pos && pilhaResultado != NULL; i++)
    {
        pilhaResultado = pilhaResultado->prox;
    }
    return pilhaResultado;
}

// Realoca o espaço de memória para a quantidade atualizada de cartas.
void libera_memoria_cartas(Pilhas p)
{
    // Liberta o espaço de memória que deixou de ser usado na origem
    if (p->numCartas > 0) {
        // Redimensiona a array da pilha
        p->pilha = realloc(p->pilha, p->numCartas * sizeof(struct carta));
    } else {
        free(p->pilha);
        p->pilha = NULL;
    }
}

// Recebe duas pilhas para acessar as cartas e também a linha escolhida e o número de cartas movidas.
// Modifica os espaços de memória para acrescentar ou retirar cartas das duas pilhas e acrescenta as cartas que devem ser movidas na pilha destino.
void insere_cartas(Pilhas origem, Pilhas destino, int linha, int numMovidas)
{
    // Aumenta o espaço da memória para inserir as cartas movidas
    destino->pilha = realloc(destino->pilha, (destino->numCartas + numMovidas) * sizeof(struct carta));
    // Coloca os valores das cartas no novo espaço
    for(int i = 0; i < numMovidas; i++)
    {
        destino->pilha[destino->numCartas + i] = origem->pilha[(linha - 1) + i];
    }

    // Atualiza quantidade de cartas nas pilhas
    destino->numCartas += numMovidas;
    origem->numCartas -= numMovidas;

    // Liberta o espaço de memória que deixou de ser usado na origem
    libera_memoria_cartas(origem);
}

// Recebe uma pilha e a linha movida e passa a carta a frente da linha para trás e todas as outras também
void corrige_seq_cartas(Pilhas p, int linha)
{
    // Desloca as cartas restantes na pilha de origem para preencher o buraco deixado pela carta movida
    for (int i = linha - 1; i < p->numCartas - 1; i++)
    {
        p->pilha[i] = p->pilha[i + 1];
    }

    // Atualiza quantidade de cartas nas pilhas
    p->numCartas--;
}

// Recebe uma pilha e uma array de posição que deve ter a [coluna, linha], tanto para origem quanto o destino.
int mover_cartas(Pilhas *p, int posOrig[], int posDest[])
{
    // Extração dos dados baseados na array
    int orig_col = posOrig[0];
    int orig_linha = posOrig[1] + 1;
    int dest_col = posDest[0];
    
    // Avança o apontador para as colunas de origem e destino das cartas
    Pilhas pilhaOrigem = procura_pilha(*p, orig_col);
    Pilhas pilhaDestino = procura_pilha(*p, dest_col);
    

    // Prevenção de erros caso as colunas não existam ou quantidade seja inválida
    if(pilhaOrigem == NULL || pilhaDestino == NULL || orig_linha < 1) return 1;

    // Guarda o número de cartas que serão movidas
    int numMovidas = pilhaOrigem->numCartas - (orig_linha - 1);

    // Checa se o número de carta movidas é menor ou igual a zero
    if (numMovidas <= 0) return 1; // Nada a mover
    
    insere_cartas(pilhaOrigem, pilhaDestino, orig_linha, numMovidas);

    return 0;
}

// Acha a pilha com maior número de carta e retorna o número que há
int acharLimite(Pilhas p){
    if (p == NULL) return 0;

    Pilhas pTemp = p;
    int maior = p->numCartas;

    while (pTemp != NULL) {
        if (maior < pTemp->numCartas)
        {
            maior = pTemp->numCartas;
        }
        
        pTemp = pTemp->prox;
    }

    return maior;
}

// verifica se as posiçoes pedidas sao validas
int pos_valida(int posOrig[], int posDest[])
{
    if (posOrig[0]>=10 || posDest[0]>=10 || posDest[0]<0 || posOrig[0]<0 || posDest[1]<0 || posOrig[1]<0 || posOrig[0] == posDest[0])
        return 1;
    return 0;
}


// Liberta a memória de todas as pilhas do jogo
void limpa_memoria_jogo(Pilhas *p)
{
    if (*p != NULL) {
        Pilhas atual = *p;
        Pilhas proximo;
        while (atual != NULL) {
            proximo = atual->prox;
            if (atual->pilha != NULL) free(atual->pilha);
            if (atual->tipo_Pilha != NULL) free(atual->tipo_Pilha);
            if (atual->flags != NULL) free(atual->flags);
            free(atual);
            atual = proximo;
        }
        *p = NULL;
    }
}

// Inicializa o jogo, atribuindo valor as variaveis iniciais, gerando e dando shuffle em um baralho
void iniciar_jogo(struct baralho baralhos[], Pilhas *p, int *contagemBaralho, int tamPilhas[], int *gameOver, int numBaralhos) 
{
    // Liberta a memória do jogo anterior caso estejamos a reiniciar
    limpa_memoria_jogo(p);
    inicializa_baralhos(baralhos, numBaralhos);
    *contagemBaralho = 0;
    // Define os tamanhos iniciais para cada uma das 10 pilhas 
    int valoresIniciais[] = {8,8,8,7,6,5,4,3,2,1,0,0,0,0};
    int tamanhoArray = sizeof(valoresIniciais) / sizeof(valoresIniciais[0]);
    for (int i = 0; i < tamanhoArray; i++)
    {
        tamPilhas[i] = valoresIniciais[i];
    }

    *p = cria_pilhas(baralhos, tamPilhas, tamanhoArray);
    *gameOver = 0;
}

// Atualiza os valores para compatíveis com array e verifica se a jogada é válida
void jogar_Coluna(EstadoJogo *g, int posOrig[], int posDest[])
{
    posOrig[0]--;
    posOrig[1]--;
    posDest[0]--;
    
    movimentoValido(*g, posOrig, posDest);
}

// Recebe a pilha e pede ao jogador as posições de jogadas.
// Chama ao jogar coluna com as informações
void pedir_jogada(EstadoJogo *g)
{
    int posOrig[2] = {0,0};
    int posDest[2] = {0,0};

    printf("Digite a coluna da carta que vai mover: ");
    scanf("%d", &posOrig[0]);
    printf("Digite a linha da carta que vai mover: ");
    scanf("%d", &posOrig[1]);

    printf("Digite a coluna destino: ");
    scanf("%d", &posDest[0]);

    jogar_Coluna(g, posOrig, posDest);
}


// A partir da jogada selecionada, processa a jogada correta para o numero dado
void processar_jogada(EstadoJogo *g, struct baralho baralhos[], int *contagemBaralho, int tamPilhas[], int *gameOver)
{
    unsigned int jogadaEscolhida = opcao_inicio();
    
    //Sair do jogo
    if(jogadaEscolhida == 3)
    {
        printf("Saindo do jogo...\n");
        *gameOver = 2;
    }

    //RESTART
    else if(jogadaEscolhida == 2)
    {
        iniciar_jogo(baralhos, &(g->pilhas), contagemBaralho, tamPilhas, gameOver, g->nBaralhos);
    }

    //Jogar
    else if(jogadaEscolhida==1)
    {
        pedir_jogada(g);
        return;
    }
}

//
// FUNCOES DA PARTE 3 PARA FICAR MAIS ORGANIZADO
// METER VERIFICAÇAO DAS COORDENADAS ESCOLHIDAS NA FUNCAO PRINCIPAL (MUITO IMPORTANTE)
//

int cartaChegadaEmenor (Pilhas p, int posOrig[], int posDest[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);
    Pilhas pilhaDestino = procura_pilha(p, posDest[0]);

    struct carta origem = (pilhaOrigem->pilha)[posOrig[1]],
    chegada = {posDest[0], 1}; // Carta placeholder caso a coluna destino esteja vazia

    if (pilhaDestino->pilha != NULL && pilhaDestino->numCartas > 0) 
    {
        chegada = (pilhaDestino->pilha)[pilhaDestino->numCartas - 1];
    }

    if (origem.valor == chegada.valor+1)
        return 0;
    return 1;
}

int cartaChegadaEmaior (Pilhas p, int posOrig[], int posDest[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);
    Pilhas pilhaDestino = procura_pilha(p, posDest[0]);

    struct carta origem = (pilhaOrigem->pilha)[posOrig[1]],
    chegada = {posDest[0], 1}; // Carta placeholder caso a coluna destino esteja vazia

    if (pilhaDestino->pilha != NULL && pilhaDestino->numCartas > 0) 
    {
        chegada = (pilhaDestino->pilha)[pilhaDestino->numCartas - 1];
    }

    if (origem.valor == chegada.valor-1)
        return 0;
    return 1;
}

int cartaMaiorOuMenor (Pilhas p, int posOrig[], int posDest[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);
    Pilhas pilhaDestino = procura_pilha(p, posDest[0]);

    struct carta origem = (pilhaOrigem->pilha)[posOrig[1]],
    chegada = {posDest[0], 1}; // Carta placeholder caso a coluna destino esteja vazia

    if (pilhaDestino->pilha != NULL && pilhaDestino->numCartas > 0) 
    {
        chegada = (pilhaDestino->pilha)[pilhaDestino->numCartas - 1];
    }

    if ((origem.valor == chegada.valor-1) || (origem.valor == chegada.valor+1))
        return 0;
    return 1;
}

int mesmoNaipeTopo (Pilhas p, int posOrig[], int posDest[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);
    Pilhas pilhaDestino = procura_pilha(p, posDest[0]);

    struct carta origem = (pilhaOrigem->pilha)[posOrig[1]],
    chegada = {posDest[0], 1}; // Carta placeholder caso a coluna destino esteja vazia

    if (pilhaDestino->pilha != NULL && pilhaDestino->numCartas > 0) 
    {
        chegada = (pilhaDestino->pilha)[pilhaDestino->numCartas - 1];
    }

    if (origem.naipe == chegada.naipe)
        return 0;
    return 1;
}

int mesmoNaipe (Pilhas p, int posOrig[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);


    struct carta origem = (pilhaOrigem->pilha)[posOrig[1]];

    for (int i = posOrig[1]; i<pilhaOrigem->numCartas;i++)
        if (origem.naipe != pilhaOrigem->pilha[i].naipe)
            return 1;
    return 0;
}

int mesmaCorTopo (Pilhas p, int posOrig[], int posDest[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);
    Pilhas pilhaDestino = procura_pilha(p, posDest[0]);

    struct carta origem = (pilhaOrigem->pilha)[posOrig[1]],
    chegada = {posDest[0], 1}; // Carta placeholder caso a coluna destino esteja vazia

    if (pilhaDestino->pilha != NULL && pilhaDestino->numCartas > 0) 
    {
        chegada = (pilhaDestino->pilha)[pilhaDestino->numCartas - 1];
    }

    if ((origem.naipe == chegada.naipe + 2) || (origem.naipe == chegada.naipe -2) || (origem.naipe == chegada.naipe))
        return 0;
    return 1;
}

int mesmaCor (Pilhas p, int posOrig[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);

    struct carta origem = (pilhaOrigem->pilha)[posOrig[1]];

    for (int i = posOrig[1]; i<pilhaOrigem->numCartas;i++)
        if (!((origem.naipe == pilhaOrigem->pilha[i].naipe + 2) || (origem.naipe == pilhaOrigem->pilha[i].naipe -2) || (origem.naipe == pilhaOrigem->pilha[i].naipe)))
            return 1;
    return 0;
}

int pilhaDestinoVazia (Pilhas p, int posDest[])
{
    Pilhas pilhaDestino = procura_pilha(p, posDest[0]);

    if (pilhaDestino->pilha != NULL && pilhaDestino->numCartas > 0) 
    {
        return 1; // Falha a restrição porque a pilha NÃO está vazia
    }
    return 0; // Passa a restrição porque a pilha está vazia!
}

// Imprime o valor da carta em um arquivo de save (A, 2-10, J, Q, K)
void salva_valor(FILE *save, int valor)
{
    if (valor == 1) fprintf(save, "A");
    else if (valor == 11) fprintf(save, "J");
    else if (valor == 12) fprintf(save, "Q");
    else if (valor == 13) fprintf(save, "K");
    else fprintf(save, "%d", valor);
}

void salva_naipe(FILE *save, int naipe)
{
    if (naipe == 0) fprintf(save, "H");
    else if (naipe == 1) fprintf(save, "S");
    else if (naipe == 2) fprintf(save, "D");
    else if (naipe == 3) fprintf(save, "C");
}

// Percorre as pilhas e salva as cartas presentes nelas por linha
void salva_pilhas(FILE *save, Pilhas p)
{
    Pilhas pTemp = p;

    while (pTemp != NULL) {
        for (int i = 0; i < pTemp->numCartas; i++) {
            struct carta c = pTemp->pilha[i];

            // Imprime o valor da carta (A, 2-10, J, Q, K)
            salva_valor(save, c.valor);

            // Imprime o naipe (0-Copas/H, 1-Espadas/S, 2-Diamantes/D, 3-Paus/C)
            salva_naipe(save, c.naipe);

            // Imprime um espaco entre cartas (mas não na última carta da linha)
            if (i < pTemp->numCartas - 1) fprintf(save, " ");
        }
        // Coloca o \n no final de cada pilha (inclusive deixa uma linha vazia para pilhas vazias)
        fprintf(save, "\n");
        pTemp = pTemp->prox;
    }
}

int salvaJogo (EstadoJogo g, int contagemSaves)
{
    char nomeArquivo[30];

    sprintf(nomeArquivo, "save_%d.txt", contagemSaves);
    FILE *novoSave = fopen(nomeArquivo, "w");

    if(novoSave == NULL)
    {
        printf("Erro ao criar arquivo %s", nomeArquivo);
        return 1;
    }

    fprintf(novoSave, "%s.paciencia\n", g.nome_paciencia);
    salva_pilhas(novoSave, g.pilhas);
    
    fclose(novoSave);
    return 0;
}

void lerSaves(int numSave, EstadoJogo g)
{
    DIR *dir;
    struct dirent *entrada;
    int saveEncontrado = 0;

    dir = opendir("./saves");
    if (dir == NULL) {
        printf("Erro ao abrir a pasta de saves.\n");
        return;
    }

    while((entrada = readdir(dir)) != NULL)
    {
        int numero_save;
        
        if (sscanf(entrada->d_name, "save_%d.txt", &numero_save) == 1) {
            printf("Slot [%d] -> Arquivo: %s\n", numero_save, entrada->d_name);
            saveEncontrado = 1;
        }
    }

    if (!saveEncontrado) {
        printf("Nenhum arquivo de save encontrado na pasta.\n");
    }
    closedir(dir);
}

int EsoUma (Pilhas p, int posOrig[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);

    if (posOrig[1] == pilhaOrigem->numCartas-1)
        return 0;
    return 1;
}

int crescenteVerif (Pilhas p, int posOrig[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);
    if (pilhaOrigem == NULL || posOrig[1] < 0 || posOrig[1] >= pilhaOrigem->numCartas) return 1;

    struct carta origem = (pilhaOrigem->pilha)[posOrig[1]];
    int n = 1;
    for (int i = posOrig[1] + 1; i < pilhaOrigem->numCartas; i++)
    {
        if ((origem.valor + n) != (pilhaOrigem->pilha[i]).valor)
            return 1;
        n++;
    }
    return 0;
}

int decrescenteVerif (Pilhas p, int posOrig[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);
    if (pilhaOrigem == NULL || posOrig[1] < 0 || posOrig[1] >= pilhaOrigem->numCartas) return 1;

    struct carta origem = (pilhaOrigem->pilha)[posOrig[1]];
    int n = 1;
    for (int i = posOrig[1] + 1; i < pilhaOrigem->numCartas; i++)
    {
        if ((origem.valor - n) != (pilhaOrigem->pilha[i]).valor)
            return 1;
        n++;
    }
    return 0;
}

int AStopo (Pilhas p, int posOrig[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);

    struct carta origem = (pilhaOrigem->pilha)[(pilhaOrigem->numCartas)-1];

    if (origem.valor == 1)
        return 0;
    return 1;
}

int REIfundo (Pilhas p, int posOrig[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);

    struct carta origem = (pilhaOrigem->pilha)[posOrig[1]];

    if (origem.valor == 13)
        return 0;
    return 1;
}

int ASfundo (Pilhas p, int posOrig[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);

    struct carta origem = (pilhaOrigem->pilha)[posOrig[1]];

    if (origem.valor == 1)
        return 0;
    return 1;
}

int REItopo (Pilhas p, int posOrig[])
{
    Pilhas pilhaOrigem = procura_pilha(p, posOrig[0]);

    struct carta origem = (pilhaOrigem->pilha)[(pilhaOrigem->numCartas)-1];

    if (origem.valor == 13)
        return 0;
    return 1;
}

void ganhouAjuda (WinDef w, int *i, char *temp)
{
    int num=0;
    while (w.tipo[(*i)] != ' ' || w.tipo[(*i)] != '\0')
    {
        temp[num] = w.tipo[(*i)];
        (*i)++;
        num++;
    }
}
int tamanhoS (char *s)
{
    int i=0;
    while (s[i] != '\0')
    {
        i++;
    }
    return i;
}

int ganhou (Pilhas p, WinDef w)
{
    int id = 0;
    
    // Pega numa copia da string para usar strtok sem alterar a original
    char *copia = strdup(w.tipo), *token = strtok(copia, " ");
    
    Pilhas pTemp = p;
    
    // Compara cada palavra token (nome da pilha) com as pilhas do jogo
    while (token != NULL && pTemp != NULL) {
        if (pTemp->tipo_Pilha != NULL && strcmp(pTemp->tipo_Pilha, token) == 0) {
            if (pTemp->numCartas != w.numCartas[id]) {
                free(copia);
                return 1; // Falhou a condicao de cartas
            }
            id++;
            token = strtok(NULL, " ");
        }
        pTemp = pTemp->prox;
    }
    
    free(copia);
    
    if (id == w.qntsWins) return 0; // Ganhou!
    return 1;
}

// avalia as regras que dependem unicamente da pilha de origem
int avalia_regra_origem(FlagsMovimento regra, Pilhas p, int posOrig[])
{
    switch (regra) {
        case TOPO_REI:             return REItopo(p, posOrig);
        case FUNDO_REI:            return REIfundo(p, posOrig);
        case TOPO_AS:              return AStopo(p, posOrig);
        case FUNDO_AS:             return ASfundo(p, posOrig);
        case MESMO_NAIPE:          return mesmoNaipe(p, posOrig);
        case MESMA_COR:            return mesmaCor(p, posOrig);
        case ORDENADAS_DECRESCENTE:return decrescenteVerif(p, posOrig);
        case ORDENADAS_CRESCENTE:  return crescenteVerif(p, posOrig);
        default:                   return -1; // Regra não é deste grupo
    }
}

int avalia_regra(FlagsMovimento regra, Pilhas p, int posOrig[], int posDest[])
{
    int resultado = avalia_regra_origem(regra, p, posOrig);
    if (resultado != -1) return resultado;

    switch (regra) {
        case OU:                   return cartaMaiorOuMenor(p, posOrig, posDest);
        case VALOR_INFERIOR:       return cartaChegadaEmaior(p, posOrig, posDest);
        case VALOR_SUPERIOR:       return cartaChegadaEmenor(p, posOrig, posDest);
        case TOPO_MESMO_NAIPE:     return mesmoNaipeTopo(p, posOrig, posDest);
        case TOPO_MESMA_COR:       return mesmaCorTopo(p, posOrig, posDest);
        case PILHA_VAZIA:          return pilhaDestinoVazia(p, posDest);
        default:                   return 0; // NAO_HA_RESTRICOES passa sempre
    }
}

// Retorna 1 se todas as flags do movimento passarem, e 0 caso alguma falhe
int valida_todas_regras(MovimentoDef mov, Pilhas p, int posOrig[], int posDest[])
{
    int restricoes_atendidas = 1;
    int permite_seq = 0;

    for (int f = 0; f < mov.qts_flags && restricoes_atendidas == 1; f++) {
        if (mov.flags[f] == PODE_SER_SEQUENCIAS) {
            permite_seq = 1;
        } else if (avalia_regra(mov.flags[f], p, posOrig, posDest) != 0) {
            restricoes_atendidas = 0;
        }
    }

    // Se não tiver a flag '+' (PODE_SER_SEQUENCIAS), só pode mover 1 carta
    if (permite_seq == 0 && EsoUma(p, posOrig) != 0) {
        restricoes_atendidas = 0;
    }

    return restricoes_atendidas;
}

void movimentoValido(EstadoJogo g, int posOrig[], int posDest[])
{
    Pilhas pilhaOrigem = procura_pilha(g.pilhas, posOrig[0]);
    Pilhas pilhaDestino = procura_pilha(g.pilhas, posDest[0]);

    // Checa se existe algo NULL
    if (pilhaOrigem == NULL || pilhaDestino == NULL || 
        pilhaOrigem->tipo_Pilha == NULL || pilhaDestino->tipo_Pilha == NULL) {
        printf("Posição inválida\n");
        return;
    }

    // Percorre as regras de movimento e compara com tipo da pilha
    for (int i = 0; i < g.qts_mov_perm; i++) {
        if (strcmp(g.mov_perm[i].tipo_origem, pilhaOrigem->tipo_Pilha) == 0 &&
            strcmp(g.mov_perm[i].tipo_destino, pilhaDestino->tipo_Pilha) == 0) {
            
            if (valida_todas_regras(g.mov_perm[i], g.pilhas, posOrig, posDest) == 1) {
                Pilhas p = g.pilhas;
                mover_cartas(&p, posOrig, posDest);
                return; // Movimento feito com sucesso, encerra a função
            }
        }
    }

    printf("Movimento Inválido\n");
}

// Testa todas as regras automáticas para um par específico de pilhas de origem e destino
int avalia_regras_auto_movimento(EstadoJogo *g, Pilhas pOrig, int colOrig, int linha, Pilhas pDest, int colDest) {
    for (int i = 0; i < g->qts_auto_movs; i++) {
        if (strcmp(g->auto_movs[i].tipo_origem, pOrig->tipo_Pilha) == 0 &&
            strcmp(g->auto_movs[i].tipo_destino, pDest->tipo_Pilha) == 0) {

            int posOrig[2] = {colOrig, linha};
            int posDest[2] = {colDest, 0}; // A linha de destino nao importa

            if (valida_todas_regras(g->auto_movs[i], g->pilhas, posOrig, posDest) == 1) {
                Pilhas p = g->pilhas;
                mover_cartas(&p, posOrig, posDest);
                return 1; // Movimento bem sucedido!
            }
        }
    }
    return 0;
}

// Avalia todas as pilhas de destino possíveis para uma carta num movimento automático
int tenta_destino_auto_movimento(EstadoJogo *g, Pilhas pOrig, int colOrig, int linha) {
    Pilhas pDest = g->pilhas;
    int colDest = 0;

    while (pDest != NULL) {
        if (colOrig != colDest && pDest->tipo_Pilha != NULL) {
            if (avalia_regras_auto_movimento(g, pOrig, colOrig, linha, pDest, colDest) == 1) {
                return 1;
            }
        }
        pDest = pDest->prox;
        colDest++;
    }
    return 0;
}

// Helper: Tenta aplicar as regras de auto_movs uma única vez no tabuleiro.
// Retorna 1 se fez algum movimento, ou 0 se nada aconteceu.
int tenta_auto_movimentos(EstadoJogo *g) {
    Pilhas pOrig = g->pilhas;
    int colOrig = 0;

    while (pOrig != NULL) {
        if (pOrig->tipo_Pilha != NULL && pOrig->numCartas > 0) {
            // Testa todas as linhas da pilha origem
            for (int linha = 0; linha < pOrig->numCartas; linha++) {
                if (tenta_destino_auto_movimento(g, pOrig, colOrig, linha) == 1) {
                    return 1; // Encontrou e executou um auto movimento
                }
            }
        }
        pOrig = pOrig->prox;
        colOrig++;
    }
    return 0; // Nenhum movimento automático encontrado
}

void processar_auto_movimentos(EstadoJogo *g) {
    // Fica a executar movimentos consecutivamente até que o tabuleiro não mude mais
    while (tenta_auto_movimentos(g) == 1) { }
}
