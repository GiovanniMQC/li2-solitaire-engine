struct carta{
    int naipe;
    // 0 - Copas, 1 - Espadas, 2 - Diamantes, 3 - Paus
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