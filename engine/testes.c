#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cartas.h"
#include "logica.h"

void corrige_seq_cartas(Pilhas p, int linha);

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

void teste_shuffle_baralho() {
    struct carta b1[52], b2[52];
    cria_baralho(b1);
    cria_baralho(b2);
    shuffle_baralho(b2);
    
    int iguais = 0;
    for(int i = 0; i < 52; i++) 
        if(b1[i].valor == b2[i].valor && b1[i].naipe == b2[i].naipe) iguais++;
        
    CU_ASSERT_NOT_EQUAL(iguais, 52);
}

void teste_cria_pilha() {
    struct baralho b[1];
    inicializa_baralhos(b, 1);
    int cont = 0;
    Pilhas p = cria_pilha(b, 5, &cont);
    
    CU_ASSERT_PTR_NOT_NULL(p);
    CU_ASSERT_EQUAL(p->numCartas, 5);
    CU_ASSERT_EQUAL(cont, 5);
    limpa_memoria_jogo(&p);
}

void teste_insere_cartas() {
    struct celula orig = {0}, dest = {0};
    orig.numCartas = 2; orig.pilha = malloc(2 * sizeof(struct carta));
    dest.numCartas = 1; dest.pilha = calloc(1, sizeof(struct carta));
    orig.pilha[1].valor = 5; 
    
    insere_cartas(&orig, &dest, 2, 1);
    
    CU_ASSERT_EQUAL(orig.numCartas, 1);
    CU_ASSERT_EQUAL(dest.numCartas, 2);
    CU_ASSERT_EQUAL(dest.pilha[1].valor, 5);
    
    free(orig.pilha); free(dest.pilha);
}

void teste_corrige_seq() {
    struct carta c[3] = {{0,1}, {0,2}, {0,3}};
    struct celula p = {.numCartas = 3, .pilha = c};
    
    corrige_seq_cartas(&p, 2); 
    CU_ASSERT_EQUAL(p.numCartas, 2);
    CU_ASSERT_EQUAL(p.pilha[1].valor, 3);
}

void teste_as_topo() {
    struct carta c[1] = {{0, 1}};
    struct celula p = {.numCartas = 1, .pilha = c};
    int pos[2] = {0, 0};
    
    CU_ASSERT_EQUAL(AStopo(&p, pos), 0);
    c[0].valor = 2;
    CU_ASSERT_EQUAL(AStopo(&p, pos), 1);
}

void teste_eso_uma() {
    struct celula p = {.numCartas = 3};
    int pos_topo[2] = {0, 2}, pos_meio[2] = {0, 1};
    
    CU_ASSERT_EQUAL(EsoUma(&p, pos_topo), 0);
    CU_ASSERT_EQUAL(EsoUma(&p, pos_meio), 1);
}

void teste_mesmo_naipe() {
    struct carta c[2] = {{0, 5}, {0, 6}};
    struct celula p = {.numCartas = 2, .pilha = c};
    int pos[2] = {0, 0};
    
    CU_ASSERT_EQUAL(mesmoNaipe(&p, pos), 0);
    c[1].naipe = 1;
    CU_ASSERT_EQUAL(mesmoNaipe(&p, pos), 1);
}

void teste_limpa_memoria() {
    Pilhas p = malloc(sizeof(struct celula));
    p->tipo_Pilha = strdup("TAB");
    p->flags = NULL;
    p->pilha = malloc(sizeof(struct carta));
    p->prox = NULL;
    
    limpa_memoria_jogo(&p);
    CU_ASSERT_PTR_NULL(p); // A estrutura principal foi liberta corretamente?
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry()) 
        return CU_get_error();

    CU_pSuite pSuite = CU_add_suite("Suite_Engine_Core", NULL, NULL);

    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(pSuite, "teste_char_para_flag", teste_char_para_flag),
    CU_add_test(pSuite, "teste_cria_baralho", teste_cria_baralho),
    CU_add_test(pSuite, "teste_procura_pilha", teste_procura_pilha),
    CU_add_test(pSuite, "teste_achar_limite", teste_achar_limite),
    CU_add_test(pSuite, "teste_pos_valida", teste_pos_valida),
    CU_add_test(pSuite, "teste_pilha_vazia", teste_pilha_vazia),
    CU_add_test(pSuite, "teste_libera_memoria", teste_libera_memoria),
    CU_add_test(pSuite, "teste_sequencias", teste_sequencias),
    CU_add_test(pSuite, "teste_shuffle_baralho", teste_shuffle_baralho),
    CU_add_test(pSuite, "teste_cria_pilha", teste_cria_pilha),
    CU_add_test(pSuite, "teste_insere_cartas", teste_insere_cartas),
    CU_add_test(pSuite, "teste_corrige_seq", teste_corrige_seq),
    CU_add_test(pSuite, "teste_as_topo", teste_as_topo),
    CU_add_test(pSuite, "teste_eso_uma", teste_eso_uma),
    CU_add_test(pSuite, "teste_mesmo_naipe", teste_mesmo_naipe),
    CU_add_test(pSuite, "teste_limpa_memoria", teste_limpa_memoria);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return CU_get_error();
}