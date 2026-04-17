/*
    Struct cria uma estruturas com dados/variáveis
    
    naipe: Copas, Espadas, Ouros, Paus
    
    Com enum pode deixar explicito os valores das cartas, mas precisa 
    modificar o restante do código

*/ 

struct carta{
    int naipe;
    int valor; 
};

void print_valor(struct carta c);
void print_naipe(struct carta c);
void print_carta(struct carta c);
void print_colunas(struct carta baralho[]);
void print_game(struct carta baralho[], int stock, struct carta ult);
int pedir_jogada();

