#include <stdio.h>
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

int main() {
    
    // inicializa
    if (CUE_SUCCESS != CU_initialize_registry()) 
        return CU_get_error();

    // cria a suite (uma pasta para agrupar os teus testes)
    CU_pSuite pSuite = CU_add_suite("Suite_Simon_Solitaire", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // adiciona os testes
    CU_add_test(pSuite, "teste_criaBaralho", teste_criaBaralho);
    CU_add_test(pSuite, "teste_shuffle", teste_shuffle);

    // corre tudo e limpa a memória
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return CU_get_error();
}