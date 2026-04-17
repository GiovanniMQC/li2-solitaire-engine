/*
    Struct cria uma estruturas com dados/variáveis
    
    naipe: Copas, Espadas, Ouros, Paus
    
    Com enum pode deixar explicito os valores das cartas, mas precisa 
    modificar o restante do código

*/ 

enum naipe
{
    COPAS,
    ESPADAS,
    DIAMANTES,
    PAUS
};

struct carta{
    enum naipe naipe;
    int valor; 
};

typedef struct celula
{
    int numCartas;
    struct carta *pilha;
    struct celula *prox;
} *Pilhas;

void print_valor(struct carta c);
void print_naipe(struct carta c);
void print_carta(struct carta c);
void print_colunas(struct carta baralho[]);
void print_game(struct carta baralho[], int stock, struct carta ult);
int pedir_jogada();

