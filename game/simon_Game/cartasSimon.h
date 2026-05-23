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
    char *tipo_Pilha; // Nome definido (ex. TAB)
    char *flags; // Regras para pilha (ex. =)
    int numCartas;
    struct carta *pilha; //apontador para a primeira carta
    struct celula *prox; //apontador para a proxima celula (proxima pilha)
} *Pilhas;

// feito com * significa que a funçao que chama estas verificaçoes tem que checkar para a pilha que está a ser pega
typedef enum {
    NAO_HA_RESTRICOES,      // * 0      // "feito", so verificar na função principal e fazer o mover cartas 
    PODE_SER_SEQUENCIAS,    // +        // feito
    ORDENADAS_DECRESCENTE,  // [        // feito
    ORDENADAS_CRESCENTE,    // ]        // feito
    VALOR_INFERIOR,         // <        // feito*
    VALOR_SUPERIOR,         // > 5      // feito*
    OU,                     // ~        // feito*
    MESMO_NAIPE,            // m        // feito*
    TOPO_MESMO_NAIPE,       // M        // feito
    NAIPES_ALTERNADOS,      // x        // feito* Not(mesmoNaipe) mas para a pilha de cartas toda
    NAIPE_DIFERENTE,        // X 10     // feito* Not(mesmoNaipe)
    MESMA_COR,              // c        // feito*
    TOPO_MESMA_COR,         // C        // feito
    CORES_ALTERNADAS,       // d        // feito* Not(mesmaCor)
    TOPO_CORES_ALTERNADAS,  // D        // feito
    PILHA_VAZIA,            // V 15     // feito
    TOPO_AS,                // a        // feito
    FUNDO_AS,               // A        // feito
    TOPO_REI,               // k        // feito
    FUNDO_REI               // K        // feito
} FlagsMovimento;

typedef struct {
    char *tipo_origem;     // Ex: "TAB"
    char *tipo_destino;    // Ex: "FUNDACAO"
    FlagsMovimento *flags; // Array dinâmico de enums (guarda as múltiplas regras do movimento)
    int qts_flags;         // Tamanho do array
} MovimentoDef;

typedef struct wins{
    char *tipo;       // Nomes dos tipos de pilhas separado por espaço
    int qntsWins;     // Num de condições para ganhar
    int *numCartas;   // Array alocado dinamicamente (ex: malloc(qntsWins * sizeof(int)))
} WinDef;

// A estrutura principal que guarda toda a informação de uma Paciência lida do ficheiro
typedef struct {
    char *nome_paciencia; // Nome retirado do ficheiro
    int nBaralhos;        // Quantidade de baralhos
    
    Pilhas pilhas;        // Lista ligada com todas as pilhas do jogo
    int qts_pilhas;       // Quantidade total de pilhas instanciadas
    
    MovimentoDef *mov_perm;      // Array dinâmico de movimentos permitidos
    int qts_mov_perm;
    
    WinDef win_args; // Condições para usuário ganhar

    MovimentoDef *auto_movs;     // Array dinâmico de movimentos automáticos
    int qts_auto_movs;
} EstadoJogo;

void print_valor(struct carta c);
void print_naipe(struct carta c);
void print_carta(struct carta c);
void print_pilhas(Pilhas p, int lim);
void print_naipes_completos(Pilhas p);
int opcao_inicio();