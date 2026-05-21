struct carta{
    int naipe;
    // 0 - Copas, 1 - Espadas, 2 - Diamantes, 3 - Paus
    int valor; 
};

struct baralho{
    struct carta cartas[52];
};

typedef struct celula // guarda a lista ligada de pilhas
{
    char *tipo_Pilha;
    int numCartas;
    struct carta *pilha; //apontador para a primeira carta
    struct celula *prox; //apontador para a proxima celula (proxima pilha)
} *Pilhas;

typedef struct {
    char *tipo_pilha; // Ex: "TAB", "DESCARTE"
    int qts_pilhas;   // Quantidade de pilhas alocadas
    Pilhas pilhas;    // Apontador para a tua lista ligada atual
} EstadoPilhas;

// Em vez de bits, usamos números normais (0 para Falso, 1 para Verdadeiro)
typedef struct {
    int nao_ha_restricoes;      // *
    int pode_ser_sequencias;    // +
    int ordenadas_decrescente;  // [
    int ordenadas_crescente;    // ]
    int valor_inferior;         // <
    int valor_superior;         // >
    int ou;                     // ~
    int mesmo_naipe;            // m
    int topo_mesmo_naipe;       // M
    int naipes_alternados;      // x
    int naipe_diferente;        // X
    int mesma_cor;              // c
    int topo_mesma_cor;         // C
    int cores_alternadas;       // d
    int topo_cores_alternadas;  // D
    int pilha_vazia;            // V
    int topo_as;                // a
    int fundo_as;               // A
    int topo_rei;               // k
    int fundo_rei;              // K
} FlagsMovimento;

typedef struct {
    char *tipo_origem;    // Ex: "TAB"
    char *tipo_destino;   // Ex: "FUNDACAO"
    FlagsMovimento flags; // Estrutura com as regras (apenas 0s ou 1s)
} MovimentoDef;

// A estrutura principal que guarda toda a informação de uma Paciência lida do ficheiro
typedef struct {
    char *nome_paciencia; // Nome retirado do ficheiro
    int nBaralhos;        // Quantidade de baralhos
    
    EstadoPilhas *estado_pilhas; // Array dinâmico com os tipos de pilhas
    int qts_tipos_pilhas;
    
    MovimentoDef *mov_perm;      // Array dinâmico de movimentos permitidos
    int qts_mov_perm;
    
    MovimentoDef *auto_movs;     // Array dinâmico de movimentos automáticos
    int qts_auto_movs;
} EstadoJogo;

void print_valor(struct carta c);
void print_naipe(struct carta c);
void print_carta(struct carta c);
void print_pilhas(Pilhas p, int lim);
void print_naipes_completos(Pilhas p);
int opcao_inicio();