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

typedef enum {
    NAO_HA_RESTRICOES,      // *
    PODE_SER_SEQUENCIAS,    // +
    ORDENADAS_DECRESCENTE,  // [
    ORDENADAS_CRESCENTE,    // ]
    VALOR_INFERIOR,         // <
    VALOR_SUPERIOR,         // >
    OU,                     // ~
    MESMO_NAIPE,            // m
    TOPO_MESMO_NAIPE,       // M
    NAIPES_ALTERNADOS,      // x
    NAIPE_DIFERENTE,        // X
    MESMA_COR,              // c
    TOPO_MESMA_COR,         // C
    CORES_ALTERNADAS,       // d
    TOPO_CORES_ALTERNADAS,  // D
    PILHA_VAZIA,            // V
    TOPO_AS,                // a
    FUNDO_AS,               // A
    TOPO_REI,               // k
    FUNDO_REI               // K
} FlagsMovimento;

typedef struct {
    char *tipo_origem;     // Ex: "TAB"
    char *tipo_destino;    // Ex: "FUNDACAO"
    FlagsMovimento *flags; // Array dinâmico de enums (guarda as múltiplas regras do movimento)
    int qts_flags;         // Tamanho do array
} MovimentoDef;

// A estrutura principal que guarda toda a informação de uma Paciência lida do ficheiro
typedef struct {
    char *nome_paciencia; // Nome retirado do ficheiro
    int nBaralhos;        // Quantidade de baralhos
    
    Pilhas pilhas;        // Lista ligada com todas as pilhas do jogo
    int qts_pilhas;       // Quantidade total de pilhas instanciadas
    
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