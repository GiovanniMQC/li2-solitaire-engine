struct carta{
    int naipe;
    // 0 - Copas, 1 - Espadas, 2 - Diamantes, 3 - Paus
    int valor; 
};

typedef struct{
    //Pilhas actuais: tipo da pilha e que cartas
    //qnts cartas
    EstadoPilhas estado_pilhas;
    // n de baralhos
    int nBaralhos;
    //que movimentos sao validos
    MovimentoDef * mov_perm;
    // descricao dos movimentos automáticos
    MovimentoDef * auto_movs;
}EstadoJogo;

typedef struct{
    char * tipo_pilha; //TAB, DESCARTE
    size_t qts_pilhas; // tamanho de pilhas
    PilhaCartas * pilhas; // array de pilhas
} EstadoPilhas;

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
    NAO_HA_RESTRICOES, // * Não há restrições, i.e. é sempre possível mover cartas entre os dois tipos de pilhas.
    PODE_SER_SEQUENCIAS, // + Pode ser movida uma sequência de cartas.
    ORDENADAS_DECRESCENTE, // [ As cartas a mover devem estar ordenadas de forma decrescente, por valores consecutivos.
    ORDENADAS_CRESCENTE, // ] As cartas a mover devem estar ordenadas de forma crescente, por valores consecutivos.
    VALOR_INFERIOR, // < A carta de topo na sequência (eventualmente singular) a mover deve ter um valor imediatamente inferior à carta de topo no destino.
    VALOR_SUPERIOR, // > A carta de topo na sequência (eventualmente singular) a mover deve ter um valor imediatamente superior à carta de topo no destino.
    OU, // ~ Atalho para “< ou > ”“, i.e. o valor deve ser imediatamente superior ou imediatamente inferior.
    MESMO_NAIPE, // m As cartas a mover devem pertencer ao mesmo naipe
    TOPO_MESMO_NAIPE, // M A carta de topo na sequência (eventualmente singular) a mover deve pertencer ao mesmo naipe da carta no topo do destino
    NAIPES_ALTERNADOS, // x As cartas a mover devem ser de naipes alternados, i.e. cada par de cartas consecutivas são de naipes diferentes.
    NAIPE_DIFERENTE, // X A carta de topo na sequência (eventualmente singular) a mover não deve pertencer ao mesmo naipe da carta no topo do destino.
    MESMA_COR, // c As cartas a mover devem ter a mesma cor.
    TOPO_MESMA_COR, // C A carta de topo na sequência (eventualmente singular) a mover deve ter a mesma cor da carta no topo do destino.
    CORES_ALTERNADAS, // d As cartas a mover devem ter cores alternadas, i.e. cada par de cartas consecutivas têm cores diferentes.
    TOPO_CORES_ALTERNADAS, // D A carta de topo na sequência (eventualmente singular) a mover não deve ter a mesma cor da carta no topo do destino.
    PILHA_VAZIA, // V A pilha de destino deve estar vazia.
    TOPO_AS, // a A carta de topo na sequência (eventualmente singular) a mover deve ser um Ás.
    FUNDO_AS, // A A carta de fundo na sequência (eventualmente singular) a mover deve ser um Ás.
    TOPO_REI, // k A carta de topo na sequência (eventualmente singular) a mover deve ser um Rei.
    FUNDO_REI // K A carta de fundo na sequência (eventualmente singular) a mover deve ser um Rei.
};

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