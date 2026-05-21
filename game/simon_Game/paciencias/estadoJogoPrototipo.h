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

typedef struct{
    Carta * cartas;
    size_t indice_topo;

}PilhaCartas;

typedef struct{
    char * tipo_origem;
    char * tipo_destino;
    enum FlagsMovimento * flags;
    size_t qts_flags;
}MovimentoDef;

enum{
    NAO_HA_RESTRICOES, // *
    PODE_SER_SEQUENCIAS // + Pode ser movida uma sequência de cartas.
    ORDENADAS_CRESCENTE, // [ As cartas a mover devem estar ordenadas de forma decrescente, por valores consecutivos.
}

typedef struct{;
    size_t pilha_origem;
    size_t pilha_destino;
    size_t qts_cartas;
} Movimento;

void adiciona_pilha(EstadoPilhas * estado_pilhas, char * tipo_pilha) {
    //estado_pilhas->qts_pilhas ++;
    //estado_pilhas[qts_pilhas++] = init_pilha();
    //estado_pilhas
}

PilhaCartas * init_pilha(void){
    //malloc struct(PilhaCartas)
    //inicializar os campos (indice = 0, cartas está vazio)
}

void push_carta(PilhaCartas * pilha, Carta c) {
    //acrescentar c a pilha->cartas
}