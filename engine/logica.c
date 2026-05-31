#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include <dirent.h>
#include <string.h>
#include "cartas.h"
#include "logica.h"

// Função auxiliar para ler o nome do jogo e registar na lista
void registar_paciencia(const char *caminho, char lista[][512], int *q) {
    FILE *f = fopen(caminho, "r");
    char buf[256], nome[256];
    if (f && fgets(buf, sizeof(buf), f)) {
        buf[strcspn(buf, "\r\n")] = '\0';
        const char *exibir = (sscanf(buf, "JOGO %[^\n]", nome) == 1) ? nome : buf;
        printf("%d - %s\n", *q + 1, exibir);
        strcpy(lista[(*q)++], caminho);
    }
    if (f) fclose(f);
}

int listarPaciencias(const char *caminho_pasta, char lista_de_caminhos[][512]) {
    DIR *pasta = opendir(caminho_pasta);
    struct dirent *arquivo;
    int quantidade = 0;
    if (!pasta) return 0;
    printf("=== Menu de Paciencias ===\n");
    while ((arquivo = readdir(pasta))) {
        if (arquivo->d_name[0] != '.') {
            char caminho[512];
            snprintf(caminho, sizeof(caminho), "%s/%s", caminho_pasta, arquivo->d_name);
            registar_paciencia(caminho, lista_de_caminhos, &quantidade);
        }
    }
    closedir(pasta);
    return quantidade;
}

FlagsMovimento char_para_flag(char c) {
    static const struct {
        char c;
        FlagsMovimento flag;
    } FLAG_MAP[] = {
        {'*', NAO_HA_RESTRICOES}, {'+', PODE_SER_SEQUENCIAS},
        {'[', ORDENADAS_DECRESCENTE}, {']', ORDENADAS_CRESCENTE},
        {'<', VALOR_INFERIOR}, {'>', VALOR_SUPERIOR}, {'~', OU},
        {'m', MESMO_NAIPE}, {'M', TOPO_MESMO_NAIPE}, {'x', NAIPES_ALTERNADOS},
        {'X', NAIPE_DIFERENTE}, {'c', MESMA_COR}, {'C', TOPO_MESMA_COR},
        {'d', CORES_ALTERNADAS}, {'D', TOPO_CORES_ALTERNADAS},
        {'V', PILHA_VAZIA}, {'a', TOPO_AS}, {'A', FUNDO_AS},
        {'k', TOPO_REI}, {'K', FUNDO_REI}
    };
    int FLAG_MAP_SIZE = sizeof(FLAG_MAP) / sizeof(FLAG_MAP[0]);
    
    for (int i = 0; i < FLAG_MAP_SIZE; i++) {
        if (FLAG_MAP[i].c == c) return FLAG_MAP[i].flag;
    }
    return NAO_HA_RESTRICOES;
}

// Remove comentário (#...)
void remove_comentario(char *linha) {
    char *hash = strchr(linha, '#');
    if (hash) *hash = '\0';
    int len = (int)strlen(linha);
    while (len > 0 && (linha[len-1] == ' ' || linha[len-1] == '\t' ||
                       linha[len-1] == '\r' || linha[len-1] == '\n'))
        linha[--len] = '\0';
}

// Parseia comando JOGO
void parse_jogo(char *linha, EstadoJogo *e) {
    char buffer[256];
    if (sscanf(linha, "JOGO %[^\n]", buffer) == 1)
        e->nome_paciencia = strdup(buffer);
}

// Parseia comando BARALHOS
void parse_baralhos(char *linha, EstadoJogo *e, struct baralho **decks) {
    sscanf(linha, "BARALHOS %d", &e->nBaralhos);
    if (e->nBaralhos > 0) {
        *decks = malloc(sizeof(struct baralho) * e->nBaralhos);
        inicializa_baralhos(*decks, e->nBaralhos);
    }
}

// Parseia comando TIPO e armazena em arrays
void parse_tipo(char *linha, char nomes_tipo[][32], char flags_tipo[][32], int *n_tipos) {
    if (*n_tipos < 32 && sscanf(linha, "TIPO %s %s", nomes_tipo[*n_tipos], flags_tipo[*n_tipos]) == 2)
        (*n_tipos)++;
}

// Carrega cartas na pilha INIT durante parsing
void parse_init_carrega_cartas(Pilhas nova, int q_cartas, struct baralho *decks, int *contagem_cartas) {
    if (q_cartas <= 0 || decks == NULL) return;
    
    nova->pilha = malloc(sizeof(struct carta) * q_cartas);
    for (int i = 0; i < q_cartas; i++) {
        nova->pilha[i] = decks[*contagem_cartas / 52].cartas[*contagem_cartas % 52];
        (*contagem_cartas)++;
    }
}

// Encontra tipo na lista e copia flags
void parse_init_copia_flags(Pilhas nova, const char *t_nome, char nomes_tipo[][32], 
                                   char flags_tipo[][32], int n_tipos) {
    int encontrado = 0;
    for (int i = 0; i < n_tipos && !encontrado; i++) {
        if (strcmp(nomes_tipo[i], t_nome) == 0) {
            nova->flags = strdup(flags_tipo[i]);
            encontrado = 1;
        }
    }
}

// Função auxiliar para adicionar pilha ao estado do jogo
void insere_pilha_estado(EstadoJogo *e, Pilhas nova) {
    if (!e->pilhas) e->pilhas = nova;
    else {
        Pilhas aux = e->pilhas;
        while (aux->prox) aux = aux->prox;
        aux->prox = nova;
    }
    e->qts_pilhas++;
}

// Parseia comando INIT
void parse_init(char *linha, EstadoJogo *e, char nomes_tipo[][32], 
                       char flags_tipo[][32], int n_tipos, struct baralho *decks, int *contagem_cartas) {
    char t_nome[32];
    int q_cartas;
    
    if (sscanf(linha, "INIT %s %d", t_nome, &q_cartas) != 2) return;
    
    Pilhas nova = calloc(1, sizeof(struct celula));
    nova->tipo_Pilha = strdup(t_nome);
    nova->numCartas = q_cartas;
    parse_init_copia_flags(nova, t_nome, nomes_tipo, flags_tipo, n_tipos);
    parse_init_carrega_cartas(nova, q_cartas, decks, contagem_cartas);
    
    insere_pilha_estado(e, nova);
}

// Função auxiliar para alocar regras de movimento
void aloca_movimento(MovimentoDef **arr, int *q, char *o, char *d, char *fl) {
    *arr = realloc(*arr, (*q + 1) * sizeof(MovimentoDef));
    MovimentoDef *m = &(*arr)[*q];
    m->tipo_origem = strdup(o);
    m->tipo_destino = strdup(d);
    m->qts_flags = strlen(fl);
    m->flags = malloc(sizeof(FlagsMovimento) * m->qts_flags);
    for (int i = 0; i < m->qts_flags; i++)
        m->flags[i] = char_para_flag(fl[i]);
    (*q)++;
}

// Parseia comando MOV ou AUTO
void parse_movimento(char *linha, EstadoJogo *e, int is_auto) {
    char o[32], d[32], fl[64];
    if (sscanf(linha, "%*s %s %s %s", o, d, fl) != 3) return;
    
    if (is_auto) aloca_movimento(&e->auto_movs, &e->qts_auto_movs, o, d, fl);
    else aloca_movimento(&e->mov_perm, &e->qts_mov_perm, o, d, fl);
}

// Parseia comando WIN
void parse_win(char *linha, EstadoJogo *e) {
    char t[32];
    int q;
    
    if (sscanf(linha, "WIN %s %d", t, &q) != 2) return;
    
    if (!e->win_args.tipo) e->win_args.tipo = strdup(t);
    else {
        char *tmp = malloc(strlen(e->win_args.tipo) + strlen(t) + 2);
        sprintf(tmp, "%s %s", e->win_args.tipo, t);
        free(e->win_args.tipo);
        e->win_args.tipo = tmp;
    }
    e->win_args.numCartas = realloc(e->win_args.numCartas, 
                                      (e->win_args.qntsWins + 1) * sizeof(int));
    e->win_args.numCartas[e->win_args.qntsWins++] = q;
}

// Função auxiliar para rotear comandos da paciência
void processa_comando_paciencia(char *cmd, char *linha, EstadoJogo *e, struct baralho **decks,
                                       char nomes_tipo[][32], char flags_tipo[][32],
                                       int *n_tipos, int *contagem_cartas) {
    if (strcmp(cmd, "JOGO") == 0) parse_jogo(linha, e);
    else if (strcmp(cmd, "BARALHOS") == 0) parse_baralhos(linha, e, decks);
    else if (strcmp(cmd, "TIPO") == 0) parse_tipo(linha, nomes_tipo, flags_tipo, n_tipos);
    else if (strcmp(cmd, "INIT") == 0) parse_init(linha, e, nomes_tipo, flags_tipo, *n_tipos, *decks, contagem_cartas);
    else if (strcmp(cmd, "MOV") == 0) parse_movimento(linha, e, 0);
    else if (strcmp(cmd, "AUTO") == 0) parse_movimento(linha, e, 1);
    else if (strcmp(cmd, "WIN") == 0) parse_win(linha, e);
}

// Função auxiliar para processar as linhas do ficheiro de configuração da paciência
void ler_linhas_paciencia(FILE *f, EstadoJogo *e, struct baralho **decks,
    char nomes_tipo[][32], char flags_tipo[][32],
    int *n_tipos, int *contagem_cartas) {
    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {
        remove_comentario(linha);
        if (strlen(linha) > 0) {
            char cmd[32];
            if (sscanf(linha, "%s", cmd) == 1 && linha[0] != '#') {
                processa_comando_paciencia(cmd, linha, e, decks, nomes_tipo, flags_tipo, n_tipos, contagem_cartas);
            }
        }
    }
}

EstadoJogo* lerPaciencia(const char *caminho_ficheiro) {
    FILE *f = fopen(caminho_ficheiro, "r");
    if (!f) return NULL;

    EstadoJogo *e = calloc(1, sizeof(EstadoJogo));
    e->caminho_ficheiro = strdup(caminho_ficheiro);

    char nomes_tipo[32][32], flags_tipo[32][32];
    int n_tipos = 0, contagem_cartas = 0;
    struct baralho *decks = NULL;

    ler_linhas_paciencia(f, e, &decks, nomes_tipo, flags_tipo, &n_tipos, &contagem_cartas);
    
    if (decks) free(decks);
    fclose(f);
    return e;
}

// Função auxiliar para escrever uma carta num ficheiro de save
void escrever_carta_save(FILE *f, struct carta c) {
    if (c.valor == 1) fprintf(f, "A");
    else if (c.valor == 11) fprintf(f, "J");
    else if (c.valor == 12) fprintf(f, "Q");
    else if (c.valor == 13) fprintf(f, "K");
    else fprintf(f, "%d", c.valor);
    
    const char naipes[] = {'H', 'S', 'D', 'C'};
    if (c.naipe >= 0 && c.naipe <= 3) fprintf(f, "%c", naipes[c.naipe]);
}

// Função auxiliar para escrever o estado das pilhas num ficheiro de save
void escrever_pilhas_save(FILE *f, Pilhas p) {
    while (p != NULL) {
        for (int i = 0; i < p->numCartas; i++) {
            escrever_carta_save(f, p->pilha[i]);
            if (i < p->numCartas - 1) fprintf(f, " ");
        }
        fprintf(f, "\n"); // Quebra de linha define o fim da pilha (pilhas vazias ficam em branco)
        p = p->prox;
    }
}

// Salva o estado atual do jogo num arquivo .txt dentro da pasta 'saves'
int salvar_jogo(EstadoJogo *g, const char *caminho_save) {
    FILE *f = fopen(caminho_save, "w");
    if (!f) return 0;
    
    // Salva apenas o nome do ficheiro, ignorando a pasta
    char *nome = strrchr(g->caminho_ficheiro, '/');
    // verifica se é nulo e avança um char depois do /
    fprintf(f, "%s\n", nome ? nome + 1 : g->caminho_ficheiro);
    
    // salva o conteúdo de cada pilha
    escrever_pilhas_save(f, g->pilhas);

    fclose(f);
    return 1;
}

// Transforma char em número de naipe
int char_para_naipe(char naipe_char) {
    if (naipe_char == 'H') return 0;
    if (naipe_char == 'S') return 1;
    if (naipe_char == 'D') return 2;
    if (naipe_char == 'C') return 3;
    return 0;
}

// Transforma string em número de carta
int str_para_valor(char *tok) {
    if (strcmp(tok, "A") == 0) return 1;
    if (strcmp(tok, "J") == 0) return 11;
    if (strcmp(tok, "Q") == 0) return 12;
    if (strcmp(tok, "K") == 0) return 13;
    return atoi(tok);
}

// Função auxiliar para extrair uma carta de uma string como "AH" ou "10S"
struct carta parse_carta_string(char *tok) {
    struct carta c = {0, 0};
    int len = strlen(tok);
    if (len == 0) return c;
    
    c.naipe = char_para_naipe(tok[len - 1]);
    tok[len - 1] = '\0';
    c.valor = str_para_valor(tok);
    
    return c;
}

// Conta número de tokens de espaço na linha
int conta_tokens_espaco(const char *linha) {
    int n = 0;
    char *copia = strdup(linha), *tok = strtok(copia, " ");
    while (tok) { n++; tok = strtok(NULL, " "); }
    free(copia);
    return n;
}

// Parse uma string de cartas no formato "AH 2D 10S..." e carrega em pilha
void carrega_cartas_pilha(char *linha, Pilhas p) {
    p->numCartas = conta_tokens_espaco(linha);
    if (p->numCartas == 0) { free(p->pilha); p->pilha = NULL; return; } // Esvazia corretamente
    
    p->pilha = realloc(p->pilha, p->numCartas * sizeof(struct carta));
    char *tok = strtok(linha, " ");
    int i = 0;
    while (tok && i < p->numCartas) {
        p->pilha[i++] = parse_carta_string(tok);
        tok = strtok(NULL, " ");
    }
}

// Função auxiliar para ler as cartas do save e substituir nas pilhas
void processar_pilhas_save(FILE *f, Pilhas p) {
    char linha[512];
    while (p != NULL && fgets(linha, sizeof(linha), f) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        carrega_cartas_pilha(linha, p);
        p = p->prox;
    }
    
    // Se o ficheiro terminar, esvazia as pilhas restantes
    while (p != NULL) {
        p->numCartas = 0;
        free(p->pilha);
        p->pilha = NULL;
        p = p->prox;
    }
}

// Função auxiliar para ler a primeira linha do save e inicializar o estado base
EstadoJogo* inicializa_estado_save(FILE *f) {
    char linha[512], caminho[1024];
    if (!fgets(linha, sizeof(linha), f)) return NULL;
    linha[strcspn(linha, "\r\n")] = '\0';
    
    snprintf(caminho, sizeof(caminho), "paciencias/%s", linha);
    return lerPaciencia(caminho);
}

// Carrega um jogo salvo utilizando as funções de importação do estado
EstadoJogo* carregar_save(const char *caminho_save) {
    FILE *f = fopen(caminho_save, "r");
    if (!f) return NULL;
    
    // Inicializa o jogo base a partir da indicacao do ficheiro de save
    EstadoJogo *g = inicializa_estado_save(f);
    if (!g) { fclose(f); return NULL; }
    
    // Substitui as cartas aleatórias iniciais pelas cartas registadas no save
    processar_pilhas_save(f, g->pilhas);

    fclose(f); 
    return g;
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

// verifica se as as cartas são compatíveis
int carta_check (Pilhas pilhaOrigem, Pilhas pilhaDestino, struct carta origem, struct carta chegada, int origLin, int naipeSelecionado)
{
    
    if ((pilhaOrigem->numCartas)<(origLin) || (!(origem.valor == (chegada.valor-1) || pilhaDestino->numCartas == 0)))
        return 1;
    
    for (int i = origLin; i < pilhaOrigem->numCartas; i++)
    {
        struct carta cartaAverificar = (pilhaOrigem->pilha)[i];
        if (!(cartaAverificar.naipe == naipeSelecionado) || (cartaAverificar.valor == (pilhaOrigem->pilha)[i+1].valor-1))
            return 1;
    }
    return 0;
}

// Valida se posição está fora dos limites
int pos_fora_limites(int col, int lin) {
    return col >= 10 || col < 0 || lin < 0;
}

// verifica se as posiçoes pedidas sao validas
int pos_valida(int posOrig[], int posDest[])
{
    if (pos_fora_limites(posOrig[0], posOrig[1])) return 1;
    if (pos_fora_limites(posDest[0], posDest[1])) return 1;
    if (posOrig[0] == posDest[0]) return 1;
    return 0;
}

// verifica se a jogada pedida é possivel
int valida_jogada(Pilhas p, int posOrig[], int posDest[])
{
    int origCol = posOrig[0], origLin = posOrig[1];
    int destCol = posDest[0];
    
    Pilhas pilhaOrigem = procura_pilha(p, origCol);
    Pilhas pilhaDestino = procura_pilha(p, destCol);
        
    if (pos_valida(posOrig, posDest) || pilhaOrigem == NULL || pilhaDestino == NULL)
        return 1;

    if (pilhaOrigem->pilha == NULL)
        return 1;

    struct carta origem = pilhaOrigem->pilha[origLin];
    struct carta chegada = {0, 1};
    
    if (pilhaDestino->pilha != NULL && pilhaDestino->numCartas > 0)
        chegada = pilhaDestino->pilha[pilhaDestino->numCartas - 1];
    
    if (carta_check(pilhaOrigem, pilhaDestino, origem, chegada, origLin, origem.naipe) == 1)
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

// Retorna o próximo ID numérico disponível na pasta saves
int obter_proximo_save_id() {
    DIR *dir = opendir("saves");
    if (!dir) return 1; // Se a pasta não existir, começa no 1
    
    struct dirent *entrada;
    int max_id = 0;
    
    while ((entrada = readdir(dir)) != NULL) {
        int id;
        if (sscanf(entrada->d_name, "save_%d.txt", &id) == 1) {
            if (id > max_id) max_id = id;
        }
    }
    closedir(dir);
    return max_id + 1;
}

// Função auxiliar para processar comando SAIR
void comando_sair(int *gameOver) {
    printf("Saindo do jogo...\n");
    *gameOver = 2;
}

// Função auxiliar para processar comando SALVAR
void comando_salvar(EstadoJogo *g) {
    int prox_id = obter_proximo_save_id();
    char nome_save[128];
    sprintf(nome_save, "saves/save_%d.txt", prox_id);
    
    if (salvar_jogo(g, nome_save)) {
        printf("Jogo salvo com sucesso no slot [%d]!\n", prox_id);
    } else {
        printf("Erro ao salvar o jogo.\n");
    }
}

// Função auxiliar para liberar os arrays dinâmicos de movimentos
void libera_movimentos_array(MovimentoDef *movs, int qts) {
    if (movs != NULL) {
        for (int i = 0; i < qts; i++) {
            free(movs[i].tipo_origem);
            free(movs[i].tipo_destino);
            free(movs[i].flags);
        }
        free(movs);
    }
}

// Função auxiliar para liberar memória do estado
void libera_estado_jogo(EstadoJogo *g) {
    limpa_memoria_jogo(&(g->pilhas));
    free(g->nome_paciencia);
    free(g->caminho_ficheiro);
    free(g->win_args.tipo);
    free(g->win_args.numCartas);
    
    libera_movimentos_array(g->mov_perm, g->qts_mov_perm);
    libera_movimentos_array(g->auto_movs, g->qts_auto_movs);
}

// Função auxiliar para processar comando RESTART
void comando_restart(EstadoJogo *g, int *gameOver) {
    char *caminho = strdup(g->caminho_ficheiro);
    libera_estado_jogo(g);
    
    EstadoJogo *novo = lerPaciencia(caminho);
    free(caminho);
    
    if (novo != NULL) {
        *g = *novo;
        free(novo);
    }
    *gameOver = 0;
}

// A partir da jogada selecionada, processa a jogada correta para o numero dado
void processar_jogada(EstadoJogo *g, struct baralho baralhos[], int *contagemBaralho, int tamPilhas[], int *gameOver)
{
    unsigned int jogadaEscolhida = opcao_inicio();
    
    if (jogadaEscolhida == 4) {
        comando_sair(gameOver);
    } else if (jogadaEscolhida == 3) {
        comando_salvar(g);
    } else if (jogadaEscolhida == 2) {
        comando_restart(g, gameOver);
    } else if (jogadaEscolhida == 1) {
        pedir_jogada(g);
    }
}

// devolve o numero de cartas da mesma pinta seguidas
int sequencias(Pilhas p)
{
    int seq = 1;
    for (int i = p->numCartas-1; i>=1; i--)
    {
        if(!(((p->pilha)[i].naipe == (p->pilha)[i-1].naipe) && ((p->pilha)[i].valor == (p->pilha)[i-1].valor-1)))
            return seq;
        seq++;
    }
    return seq;
}

// verifica se existe uma jogada valida entre duas colunas
int verifica_colunas (Pilhas p, int coordenadaAtestar[], int colunaDest)
{
    if (colunaDest>=9 && coordenadaAtestar[0]>=9)
        return 1;
    if (colunaDest == coordenadaAtestar[0])
        colunaDest++;

    int coordenadasChegada[2] = {colunaDest, 0};

    if (valida_jogada(p, coordenadaAtestar, coordenadasChegada) == 0)
        return 0;
    
    return 1;
}

//subfunção do check gameover para ver se já não existem jogadas validas
int existe_jogadaValida (Pilhas p)
{
    Pilhas p3 = p;
    
    for (int colunaOrig = 0; colunaOrig<10; colunaOrig++)
    {
        if (p3->numCartas > 0)
        {
            int coordenadaAtestar[2] = {colunaOrig, ((p3->numCartas)-(sequencias(p3)))};

            for (int colunaDest = 0; colunaDest<10; colunaDest++)
            {
                if (verifica_colunas(p, coordenadaAtestar, colunaDest) == 0)
                    return 0;
            }
        }
        p3 = p3->prox;
    }
    return 1;
}

// subfunção do check gameover para ver se ganhou (as pilhas de pintas estão todas feitas)
int verifica_ganhou(Pilhas p, Pilhas testeSeq, int i)
{
    if (testeSeq->numCartas != 0 && sequencias(testeSeq)==13)
    {
        int origem[2] = {i, (testeSeq->numCartas)-13};
        int destino[2] = {10+(testeSeq->pilha[testeSeq->numCartas-1].naipe),0};
        mover_cartas(&p, origem, destino);
    }

    Pilhas copas = procura_pilha(p, 10), espadas = copas->prox, ouros = espadas->prox, paus = ouros->prox;
    
    if (copas->numCartas != 0 && espadas->numCartas != 0 && ouros->numCartas != 0 && paus->numCartas != 0)
    {
        return 1;
    }
    return 0;
}

//verifica se o jogo acabou
int check_gameOver(Pilhas p)
{
    Pilhas testeSeq = p;

    for (int i = 0; i<10; i++)
    {
        if(verifica_ganhou(p, testeSeq, i))
        {
            printf("Ganhaste!\n");
            return 1;
        }
            
        testeSeq = testeSeq->prox;
    }

    if (existe_jogadaValida(p) == 1)
    {
        return 2;
    }

    return 0;
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
    if (valor >= 2 && valor <= 9) fprintf(save, "%d", valor);
    else if (valor == 10) fprintf(save, "10");
    else if (valor == 1) fprintf(save, "A");
    else if (valor == 11) fprintf(save, "J");
    else if (valor == 12) fprintf(save, "Q");
    else if (valor == 13) fprintf(save, "K");
}

void salva_naipe(FILE *save, int naipe)
{
    const char NAIPES[] = {'H', 'S', 'D', 'C'};
    if (naipe >= 0 && naipe <= 3) fprintf(save, "%c", NAIPES[naipe]);
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

// Função auxiliar para processar as entradas da pasta de saves
int processar_entrada_save(struct dirent *entrada) {
    int numero_save;
    if (sscanf(entrada->d_name, "save_%d.txt", &numero_save) == 1) {
        printf(" [%d] -> %s\n", numero_save, entrada->d_name);
        return 1;
    }
    return 0;
}

void listar_saves(void)
{
    struct dirent *entrada;
    int saveEncontrado = 0;

    DIR *dir = opendir("saves");
    if (dir == NULL) {
        printf("Nenhum save encontrado ou pasta 'saves' inexistente.\n");
        return;
    }

    printf("\n=== Saves Disponiveis ===\n");
    while((entrada = readdir(dir)) != NULL)
    {
        if (processar_entrada_save(entrada)) {
            saveEncontrado = 1;
        }
    }

    if (!saveEncontrado) {
        printf(" Nenhum arquivo de save encontrado na pasta.\n");
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

int tamanhoS (char *s)
{
    int i=0;
    while (s[i] != '\0')
    {
        i++;
    }
    i++;
    return i;
}

/*
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
*/

int numeroDePilhasParaGanhar (Pilhas p, WinDef w, int tamanhoString)
{
    int j=0, contador = 0;
    
    for (int i=0; i<tamanhoString; i++)
    {
        if (p->tipo_Pilha[j] == '\0'&& (w.tipo[i] == ' ' || w.tipo[i] == '\0'))
        {
            contador++;
            if (p->prox == NULL)
                return contador;
            
            i=-1;
            j=0;
            p=p->prox;
        }
        else if (p->tipo_Pilha[j] == w.tipo[i])
        {
            j++;
        }
        else
        {
            j=0;
        }
    }
    return contador;
} 

void contadorReset (Pilhas *p, int *i, int *j)
{
    (*p)=(*p)->prox;
    (*i)=-1;
    (*j)=0;
}

int checkIgual (Pilhas p, WinDef w, int i, int j)
{
    if (p->tipo_Pilha[j] == '\0' && (w.tipo[i] == ' ' || w.tipo[i] == '\0'))
        return 0;
    return 1;
}

int winCheck (Pilhas p, WinDef w, int id, int *win, int numeroDeWins)
{
    if (p->numCartas != w.numCartas[id])
        return 1;
        
    if ((*win) >= numeroDeWins)
    return 0;
    (*win)++;
        
    return 2;
}

void naoWinCheck (Pilhas p, WinDef w, int *i, int *j, int *id)
{
    if (w.tipo[(*i)]== ' ')
    {
        (*id)++;
    }
    else if (p->tipo_Pilha[(*j)] == w.tipo[(*i)])
    {
        (*j)++;
    }
    else
    {
        (*j)=0;
        (*i)=-1;
    }
}

int ganhou (Pilhas p, WinDef w)
{
    //p->tipo_Pilha
    //w.tipo

    if (w.tipo == NULL)
        return 1;
    
    int j=0, id=0, win=0, tamanhoString = tamanhoS(w.tipo), numeroDeWins = numeroDePilhasParaGanhar (p, w, tamanhoString);
        
    for (int i=0; i<tamanhoString; i++)
    {
        if (checkIgual(p,w,i,j) == 0)
        {
            if (winCheck(p, w, id, &win, numeroDeWins)==2)
                contadorReset(&p, &i, &j);
            else
            return (winCheck(p, w, id, &win, numeroDeWins));
        }
        else
        {
            naoWinCheck (p, w, &i, &j, &id);
        }

        if (i+1 == tamanhoString && p->prox != NULL)
        {
            contadorReset(&p, &i, &j);
        }
    }
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
        default:                   return -1;
    }
}

// avalia regras de destino e bidirecionais
int avalia_regra_destino(FlagsMovimento regra, Pilhas p, int posOrig[], int posDest[])
{
    switch (regra) {
        case OU:                   return cartaMaiorOuMenor(p, posOrig, posDest);
        case VALOR_INFERIOR:       return cartaChegadaEmaior(p, posOrig, posDest);
        case VALOR_SUPERIOR:       return cartaChegadaEmenor(p, posOrig, posDest);
        case TOPO_MESMO_NAIPE:     return mesmoNaipeTopo(p, posOrig, posDest);
        case TOPO_MESMA_COR:       return mesmaCorTopo(p, posOrig, posDest);
        case PILHA_VAZIA:          return pilhaDestinoVazia(p, posDest);
        default:                   return 0;
    }
}

int avalia_regra(FlagsMovimento regra, Pilhas p, int posOrig[], int posDest[])
{
    int resultado = avalia_regra_origem(regra, p, posOrig);
    if (resultado != -1) return resultado;
    return avalia_regra_destino(regra, p, posOrig, posDest);
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
                return 1; // Movimento bem sucedido
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

// Tenta aplicar as regras de auto_movs uma única vez no tabuleiro
// Retorna 1 se fez algum movimento, ou 0 se nada aconteceu
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
    return 0; 
}

void processar_auto_movimentos(EstadoJogo *g) {
    // Fica a executar movimentos consecutivamente até que o tabuleiro não mude mais
    while (tenta_auto_movimentos(g) == 1) { }
}