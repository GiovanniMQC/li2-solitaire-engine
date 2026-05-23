#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include "cartasSimon.h"
#include "logicaSimon.h"

// Recebe uma carta e le o seu valor, printa o valor correspondente no terminal
void print_valor(struct carta c) 
{
    if (c.valor == 1) 
        printf(" A   ");
    else if (c.valor == 10) 
        printf(" 10  ");
    else if (c.valor == 11) 
        printf(" J   ");
    else if (c.valor == 12) 
        printf(" Q   ");
    else if (c.valor == 13) 
        printf(" K   ");
    else 
        printf(" %d   ", c.valor);  
}

// Recebe uma carta e le o seu naipe, printa o naipe correspondente no terminal
void print_naipe(struct carta c)
{
    if (c.naipe == 0) 
        printf("  ♥  ");
    else if (c.naipe == 1) 
        printf("  ♠  ");
    else if (c.naipe == 2) 
        printf("  ♦  ");
    else if (c.naipe == 3) 
        printf("  ♣  ");
}

// Recebe uma carta e da print a ela com formato apropriado (valor e naipe com fundo branco)
void print_carta(struct carta c) 
{
    printf("\033[30;47m"); //texto preto (30) fundo branco (47)
    print_valor(c);
    print_naipe(c);  
    printf("\033[0m"); //reseta pro normal
}

int conta_pilhas_visiveis(Pilhas p)
{
    int contagem = 0;
    while(p != NULL)
    {
        if(p->tipo_Pilha != NULL && (strchr(p->tipo_Pilha, '=') || strchr(p->tipo_Pilha, '^')))
        {
            contagem++;
        }
        p = p->prox;
    }
    return contagem;
}

void print_tipos(Pilhas p)
{
    char nomePilha[30] = "";
    Pilhas pTemp = p;
    while (pTemp != NULL){
        if (pTemp->tipo_Pilha != NULL && (strchr(pTemp->tipo_Pilha, '=') || strchr(pTemp->tipo_Pilha, '^')))
        {
            if(strcmp(nomePilha, pTemp->tipo_Pilha) != 0)
            {
                printf("%-10s ", pTemp->tipo_Pilha);
                sprintf(nomePilha, "%s", pTemp->tipo_Pilha);
            }
            else
            {
                printf("           ");
            }
        }
        pTemp = pTemp->prox;
    }
    printf("\n");
}

// Percorre as pilhas e dá print de todas as cartas presentes nelas
void print_pilhas(Pilhas p, int lim){
    int linha = 0;
    
    print_tipos(p);
    while (linha<(lim+1)){
        Pilhas pTemp = p;
        while (pTemp != NULL){
            // Renderiza a coluna apenas se ela for uma pilha visível (regras = ou ^)
            if (pTemp->tipo_Pilha != NULL && (strchr(pTemp->tipo_Pilha, '=') || strchr(pTemp->tipo_Pilha, '^'))) {
                if(pTemp->numCartas <= linha)
                    printf("          "); // espaco vazio
                else if (strchr(pTemp->tipo_Pilha, '^') && linha < pTemp->numCartas - 1)
                    printf("\033[30;47m  [****]  \033[0m"); // Desenha a carta oculta se for regra '^' e não for o topo
                else
                    print_carta(pTemp->pilha[linha]); // Desenha a carta se for regra '=' ou for o topo do '^'
                    
                printf(" ");
            }
            pTemp = pTemp->prox;
        }
        linha++;
        printf("\n");
    }
}

// Mostra os naipes que o usuário já completou ou não
void print_naipes_completos(Pilhas p)
{
    Pilhas copas = procura_pilha(p, 10), espadas = copas->prox, ouros = espadas->prox, paus = ouros->prox;
    if(copas->numCartas > 0)
    {
        printf("✓♥  ");
    } else printf(" ♥   ");
    if(espadas->numCartas > 0)
    {
        printf(" ✓♠  ");
    } else printf("  ♠   ");
    if(ouros->numCartas > 0)
    {
        printf(" ✓♦  ");
    } else printf("  ♦   ");
    if(paus->numCartas > 0)
    {
        printf(" ✓♣  ");
    } else printf("  ♣  ");
    putchar('\n');
}
// Pede o numero da jogada no terminal para o player, nao para ate conseguir um numero valido de jogada
//1 -> fazer a jogada
//2 -> restart
//3 -> sair
int opcao_inicio()
{
    unsigned int jogada_escolhida = 0;
    
    printf("1 - Jogar, 2 - Reiniciar, 3 - Sair: ");
    scanf("%d", &jogada_escolhida);
    
    while(jogada_escolhida > 3 || jogada_escolhida < 1)
    {
        printf("Opção inválida\n");
        printf("1 - Jogar, 2 - Reiniciar, 3 - Sair: ");
        scanf("%d", &jogada_escolhida);
    }

    return jogada_escolhida;
}