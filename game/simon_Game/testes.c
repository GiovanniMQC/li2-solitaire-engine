#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "logicaSimon.h"

void teste_criaBaralho() {

    struct carta meu_baralho[52]; 
    cria_baralho(meu_baralho);

    CU_ASSERT_EQUAL(meu_baralho[0].valor, 13);
    CU_ASSERT_EQUAL(meu_baralho[0].naipe, 0);

    CU_ASSERT_EQUAL(meu_baralho[12].valor, 1);
    CU_ASSERT_EQUAL(meu_baralho[12].naipe, 0);

    CU_ASSERT_EQUAL(meu_baralho[13].valor, 13);
    CU_ASSERT_EQUAL(meu_baralho[13].naipe, 1);

    CU_ASSERT_EQUAL(meu_baralho[51].valor, 1);
    CU_ASSERT_EQUAL(meu_baralho[51].naipe, 3);
}

void teste_shuffle() {

    struct carta baralho_intacto[52];
    struct carta baralho_shuffled[52];
    
    cria_baralho(baralho_intacto);
    cria_baralho(baralho_shuffled);

    shuffle_baralho(baralho_shuffled);

    int cartasIguais = 0;
    
    for(int i = 0; i < 52; i++) {
        if(baralho_intacto[i].valor == baralho_shuffled[i].valor &&
           baralho_intacto[i].naipe == baralho_shuffled[i].naipe) {
            cartasIguais++;
        }
    }

    //se baralho_intacto == baralho_shuffled
    CU_ASSERT_NOT_EQUAL(cartasIguais, 52);
}

void teste_criaPilha(){

    struct carta meu_baralho[52];
    
    //gera 3 cartas para a pilha
    meu_baralho[0].valor = 10, meu_baralho[0].naipe = 0, 
    meu_baralho[1].valor = 11, meu_baralho[1].naipe = 1, 
    meu_baralho[2].valor = 12, meu_baralho[2].naipe = 2; 
    
    int contagem = 0; 

    Pilhas minha_pilha = cria_pilha(meu_baralho, 3, &contagem);

    // teste malloc
    CU_ASSERT_PTR_NOT_NULL(minha_pilha);
    
    // teste numCartas
    CU_ASSERT_EQUAL(minha_pilha->numCartas, 3);
    
    // checar quantas cartas puxou
    CU_ASSERT_EQUAL(contagem, 3);
    
    // ordem
    CU_ASSERT_EQUAL(minha_pilha->pilha[0].valor, 10);
    CU_ASSERT_EQUAL(minha_pilha->pilha[0].naipe, 0);
    
    CU_ASSERT_EQUAL(minha_pilha->pilha[2].valor, 12);
    CU_ASSERT_EQUAL(minha_pilha->pilha[2].naipe, 2);

    // limpar memoria
    if (minha_pilha != NULL) {
        free(minha_pilha->pilha),
        free(minha_pilha);        
    }
}

void teste_procuraPilha(){

    struct carta meu_baralho[52];
    cria_baralho(meu_baralho); 
    int contagem = 0;

    Pilhas pilha_A = cria_pilha(meu_baralho, 1, &contagem);
    Pilhas pilha_B = cria_pilha(meu_baralho, 2, &contagem); 

    pilha_A->prox = pilha_B;
    pilha_B->prox = NULL;
    
    Pilhas resultado = procura_pilha(pilha_A, 1);

    CU_ASSERT_PTR_NOT_NULL(resultado);
    //verifica se e a pilha_B (2 cartas)
    CU_ASSERT_EQUAL(resultado->numCartas, 2);

    // limpar
    free(pilha_A->pilha), free(pilha_A),
    free(pilha_B->pilha), free(pilha_B);
}

void teste_corrige_seq_cartas() {

    Pilhas p = malloc(sizeof(struct celula));
    p->numCartas = 3;
    p->prox = NULL;
    p->pilha = malloc(sizeof(struct carta) * 3);
    
    p->pilha[0].valor = 10;
    p->pilha[1].valor = 20;
    p->pilha[2].valor = 30;


    // apaga a carta da linha 2 (que e o indice 1)
    corrige_seq_cartas(p, 2);

    CU_ASSERT_EQUAL(p->numCartas, 2);      
    CU_ASSERT_EQUAL(p->pilha[1].valor, 30);

    // limpar
    free(p->pilha);
    free(p);
}

void teste_acharLimite() {

    struct celula pilhas[10];
    
    for(int i = 0; i < 9; i++) {
        pilhas[i].numCartas = 5;
        pilhas[i].prox = &pilhas[i+1];
    }
    pilhas[9].numCartas = 5;
    pilhas[9].prox = NULL;

    pilhas[4].numCartas = 22; //maior pilha

    int maior_limite = acharLimite(&pilhas[0]);

    CU_ASSERT_EQUAL(maior_limite, 22);
}

void teste_validaJogada() {

    Pilhas p_falsa = NULL;
    
    // tentar mover da coluna 15 (n existe)
    int posOrigem[2]  = {15, 2}; 
    int posDestino[2] = {1, 3};

    int resultado = valida_jogada(p_falsa, posOrigem, posDestino);

    // a função retorna 1 se a jogada for inválida
    CU_ASSERT_EQUAL(resultado, 1);
}

void teste_liberaMem() {

    Pilhas p = malloc(sizeof(struct celula));
    p->pilha = malloc(sizeof(struct carta) * 3);
    
    p->numCartas = 0; 

    libera_memoria_cartas(p);

    CU_ASSERT_PTR_NULL(p->pilha);

    free(p);
}

void teste_insereCartas() {

    Pilhas orig = malloc(sizeof(struct celula));
    orig->pilha = malloc(sizeof(struct carta) * 3);
    orig->numCartas = 3,
    orig->pilha[0].valor = 10,
    orig->pilha[1].valor = 9,
    orig->pilha[2].valor = 8; 

    Pilhas dest = malloc(sizeof(struct celula));
    dest->pilha = malloc(sizeof(struct carta) * 1);
    dest->numCartas = 1,
    dest->pilha[0].valor = 99;

    // movemos 2 cartas, a partir da linha 2 (indice 1)
    insere_cartas(orig, dest, 2, 2);

    // origem perdeu 2 cartas, deve ficar com 1
    CU_ASSERT_EQUAL(orig->numCartas, 1);
    
    // destino ganhou 2 cartas, deve ficar com 3
    CU_ASSERT_EQUAL(dest->numCartas, 3);
    
    // as cartas foram para os lugares certos?
    CU_ASSERT_EQUAL(dest->pilha[1].valor, 9);
    CU_ASSERT_EQUAL(dest->pilha[2].valor, 8);

    free(orig->pilha), free(orig),
    free(dest->pilha), free(dest);
}

void teste_moverCartas(void) {
    
    Pilhas orig = malloc(sizeof(struct celula));
    Pilhas dest = malloc(sizeof(struct celula));
    orig->numCartas = 2; orig->pilha = malloc(sizeof(struct carta) * 2);
    dest->numCartas = 1; dest->pilha = malloc(sizeof(struct carta) * 1);
    
    orig->prox = dest;
    Pilhas cabeca = orig; // ponteiro inicial 

    int posOrig[] = {0, 1};
    int posDest[] = {1, 0};

    CU_ASSERT_EQUAL(mover_cartas(&cabeca, posOrig, posDest), 0); // 0 significa Sucesso
    CU_ASSERT_EQUAL(orig->numCartas, 1); // verificamos se de facto moveu
    CU_ASSERT_EQUAL(dest->numCartas, 2);

    free(orig->pilha), free(orig),
    free(dest->pilha), free(dest);
}

void teste_cartaCheck(void) {

    Pilhas orig = malloc(sizeof(struct celula)), dest = malloc(sizeof(struct celula));
    orig->numCartas = 1; dest->numCartas = 1; 
    orig->pilha = malloc(2 * sizeof(struct carta)); 
    dest->pilha = malloc(1 * sizeof(struct carta));

    orig->pilha[0].valor = 4, orig->pilha[0].naipe = 0,
    dest->pilha[0].valor = 5, dest->pilha[0].naipe = 1;
    
    orig->pilha[1].valor = 0, orig->pilha[1].naipe = 0; 

    CU_ASSERT_EQUAL(carta_check(orig, dest, orig->pilha[0], dest->pilha[0], 0, orig->pilha[0].naipe), 0);

    free(orig->pilha), free(orig);
    free(dest->pilha), free(dest);
}

void teste_iniciarJogo(void) {

    struct carta baralho[52]; 
    Pilhas p = NULL;
    int cont, over, tam[14];

    iniciar_jogo(baralho, &p, &cont, tam, &over);

    CU_ASSERT_EQUAL(cont, 0); 
    CU_ASSERT_EQUAL(over, 0);
    CU_ASSERT_EQUAL(tam[0], 8); 
    CU_ASSERT_EQUAL(tam[9], 1);
    CU_ASSERT_PTR_NOT_NULL(p);

    while (p != NULL) {
        Pilhas temp = p;
        p = p->prox;
        free(temp->pilha), free(temp);
    }
}

void teste_sequencias(void) {

    Pilhas p = malloc(sizeof(struct celula));
    p->numCartas = 2, p->pilha = malloc(2 * sizeof(struct carta));

    //sequencia
    p->pilha[0].naipe = 0, p->pilha[0].valor = 5;
    p->pilha[1].naipe = 0, p->pilha[1].valor = 6;
    
    CU_ASSERT_EQUAL(sequencias(p), 2);
    
    free(p->pilha), free(p);
}

void teste_existe_jogadaValida(void) {

    struct celula p_unica;
    p_unica.numCartas = 0, p_unica.prox = NULL;
    
    CU_ASSERT_EQUAL(existe_jogadaValida(&p_unica), 0);
}

void teste_gameOver(void) {

    struct celula t[14];
    
    //ligar todas pilhas e atribuir zeros
    for(int i = 0; i < 13; i++) {
        
        t[i].numCartas = 0, t[i].prox = &t[i+1];
        t[13].numCartas = 0, t[13].prox = NULL;
    }
        
    // simular vitoria
    t[10].numCartas = 1, t[11].numCartas = 1, t[12].numCartas = 1, t[13].numCartas = 1;
    
    CU_ASSERT_EQUAL(check_gameOver(&t[0]), 1);
}

int main() {
    
    // inicializa
    if (CUE_SUCCESS != CU_initialize_registry()) 
        return CU_get_error();

    // cria a suite (uma pasta para agrupar os teus testes)
    CU_pSuite pSuite = CU_add_suite("Suite_Simon_Solitaire", NULL, NULL);

    //verifica a suite
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // adiciona os testes
    CU_add_test(pSuite, "teste_criaBaralho", teste_criaBaralho),
    CU_add_test(pSuite, "teste_shuffle", teste_shuffle),
    CU_add_test(pSuite, "teste_criaPilha", teste_criaPilha),
    CU_add_test(pSuite, "teste_procuraPilha", teste_procuraPilha),
    CU_add_test(pSuite, "teste_liberaMem", teste_liberaMem),
    CU_add_test(pSuite, "teste_insereCartas", teste_insereCartas),
    CU_add_test(pSuite, "teste_corrige_seq_cartas", teste_corrige_seq_cartas),
    CU_add_test(pSuite, "teste_moverCartas", teste_moverCartas),
    CU_add_test(pSuite, "teste_acharLimite", teste_acharLimite),
    CU_add_test(pSuite, "teste_cartaCheck", teste_cartaCheck),
    CU_add_test(pSuite, "teste_validaJogada", teste_validaJogada);
    CU_add_test(pSuite, "teste_iniciarJogo", teste_iniciarJogo);
    CU_add_test(pSuite, "teste_sequencias", teste_sequencias);
    CU_add_test(pSuite, "teste_existe_jogadaValida", teste_existe_jogadaValida);
    CU_add_test(pSuite, "teste_gameOver", teste_gameOver);

    // corre tudo e limpa a memoria
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return CU_get_error();
}