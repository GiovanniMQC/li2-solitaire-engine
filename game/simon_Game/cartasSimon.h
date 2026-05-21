struct carta{
    int naipe;
    // 0 - Copas, 1 - Espadas, 2 - Diamantes, 3 - Paus
    int valor; 
};

struct baralho{
    struct carta cartas[52];
};

typedef struct{
    Pilhas celula;
    MovimentoDef ;
}EstadoJogo;

typedef struct{
    char * tipo_origem;
    char * tipo_destino;
    enum FlagsMovimento * flags;
    size_t qts_flags;
} MovimentoDef;

enum{
    NAO_HA_RESTRICOES, // *
    PODE_SER_SEQUENCIAS // + Pode ser movida uma sequência de cartas.
    ORDENADAS_DECRESCENTE, // [ As cartas a mover devem estar ordenadas de forma decrescente, por valores consecutivos.
    ORDENADAS_CRESCENTE, // ] As cartas a mover devem estar ordenadas de forma crescente, por valores consecutivos.
    
}

typedef struct celula // guarda a lista ligada de pilhas
{
    char *tipo_Pilha;
    int numCartas;
    struct carta *pilha; //apontador para a primeira carta
    struct celula *prox; //apontador para a proxima celula (proxima pilha)
} *Pilhas;

void print_valor(struct carta c);
void print_naipe(struct carta c);
void print_carta(struct carta c);
void print_pilhas(Pilhas p, int lim);
void print_naipes_completos(Pilhas p);
int opcao_inicio();