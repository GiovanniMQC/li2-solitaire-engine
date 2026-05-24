#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include "cartas.h"
#include "logica.h"

void teste_char_para_flag() {
    CU_ASSERT_EQUAL(char_para_flag('*'), NAO_HA_RESTRICOES);
    CU_ASSERT_EQUAL(char_para_flag('V'), PILHA_VAZIA);
    CU_ASSERT_EQUAL(char_para_flag('a'), TOPO_AS);
    CU_ASSERT_EQUAL(char_para_flag('?'), NAO_HA_RESTRICOES);
}

void teste_cria_baralho() {
    struct carta b[52];
    cria_baralho(b);
    CU_ASSERT_EQUAL(b[0].valor, 13);
    CU_ASSERT_EQUAL(b[0].naipe, 0);
    CU_ASSERT_EQUAL(b[51].valor, 1);
    CU_ASSERT_EQUAL(b[51].naipe, 3);
}

void teste_procura_pilha() {
    struct celula p1 = {0}, p2 = {0};
    p1.prox = &p2;
    
    CU_ASSERT_PTR_EQUAL(procura_pilha(&p1, 0), &p1);
    CU_ASSERT_PTR_EQUAL(procura_pilha(&p1, 1), &p2);
    CU_ASSERT_PTR_NULL(procura_pilha(&p1, 2));
}

void teste_achar_limite() {
    struct celula p1 = {0}, p2 = {0};
    p1.numCartas = 2; p1.prox = &p2;
    p2.numCartas = 5; p2.prox = NULL;
    
    CU_ASSERT_EQUAL(acharLimite(&p1), 5);
    CU_ASSERT_EQUAL(acharLimite(&p2), 5);
}

void teste_pos_valida() {
    int orig[2] = {0, 0}, dest[2] = {1, 0};
    int inv[2] = {15, 0}, neg[2] = {-1, 0};

    CU_ASSERT_EQUAL(pos_valida(orig, dest), 0); // Válida
    CU_ASSERT_EQUAL(pos_valida(inv, dest), 1);  // Fora limite
    CU_ASSERT_EQUAL(pos_valida(orig, orig), 1); // Mesma coluna
    CU_ASSERT_EQUAL(pos_valida(neg, dest), 1);  // Limite negativo
}

void teste_pilha_vazia() {
    struct celula dest = {0};
    int p_dest[2] = {0, 0};
    
    CU_ASSERT_EQUAL(pilhaDestinoVazia(&dest, p_dest), 0);
    
    dest.numCartas = 1;
    dest.pilha = (struct carta*)1; 
    CU_ASSERT_EQUAL(pilhaDestinoVazia(&dest, p_dest), 1);
}

void teste_libera_memoria() {
    struct celula p = {0};
    p.pilha = malloc(sizeof(struct carta));
    libera_memoria_cartas(&p);
    CU_ASSERT_PTR_NULL(p.pilha);
}

void teste_sequencias() {
    struct carta c[3] = {{0, 3}, {0, 2}, {0, 1}}; // {naipe, valor}
    struct celula p = {0};
    p.numCartas = 3;
    p.pilha = c;
    
    CU_ASSERT_EQUAL(sequencias(&p), 3);
    
    c[1].naipe = 1; // quebra a sequência
    CU_ASSERT_EQUAL(sequencias(&p), 1);
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry()) 
        return CU_get_error();

    CU_pSuite pSuite = CU_add_suite("Suite_Engine_Core", NULL, NULL);

    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(pSuite, "teste_char_para_flag", teste_char_para_flag);
    CU_add_test(pSuite, "teste_cria_baralho", teste_cria_baralho);
    CU_add_test(pSuite, "teste_procura_pilha", teste_procura_pilha);
    CU_add_test(pSuite, "teste_achar_limite", teste_achar_limite);
    CU_add_test(pSuite, "teste_pos_valida", teste_pos_valida);
    CU_add_test(pSuite, "teste_pilha_vazia", teste_pilha_vazia);
    CU_add_test(pSuite, "teste_libera_memoria", teste_libera_memoria);
    CU_add_test(pSuite, "teste_sequencias", teste_sequencias);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return CU_get_error();
}