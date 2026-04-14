#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include "cartas.h"
#include "logica.h"

int init_suite(void) 
{
    return 0;
}

int clean_suite(void) 
{
    return 0;
}

 // verifica se as cartas estão todas por ordem
 
void test_cria_baralho(void) 
{
    struct carta baralho[52];
    cria_baralho(baralho);
    
    // primeira carta deve ser o ás
    CU_ASSERT_EQUAL(baralho[0].valor, 1);
    CU_ASSERT_EQUAL(baralho[0].naipe, COPAS);

    // a 13 carta deve ser o rei de copas
    CU_ASSERT_EQUAL(baralho[12].valor, 13);
    CU_ASSERT_EQUAL(baralho[12].naipe, COPAS);

    // a 14 carta deve ser o as de espadas
    CU_ASSERT_EQUAL(baralho[13].valor, 1);
    CU_ASSERT_EQUAL(baralho[13].naipe, ESPADAS);

    // a ultima carta deve ser o rei de paus
    CU_ASSERT_EQUAL(baralho[51].valor, 13);
    CU_ASSERT_EQUAL(baralho[51].naipe, PAUS);
}

// testa se a quantidade do stock diminui stock e a atribuição da carta.
void test_puxar_Stock(void) 
{
    struct carta baralho[52];
    int stock = 16;
    struct carta ultima;
    
    cria_baralho(baralho); 
    puxar_Stock(baralho, &stock, &ultima);
    
    // o stock deve diminuir em 1
    CU_ASSERT_EQUAL(stock, 15);
    
    // a carta "ultima" deve ser igual à carta no índice 35 + 15 (ou seja, baralho[50])
    CU_ASSERT_EQUAL(ultima.valor, baralho[50].valor);
    CU_ASSERT_EQUAL(ultima.naipe, baralho[50].naipe);
}

//  checa se os valores aceites são só +/- 1
void test_valores_Check(void) 
{
    struct carta baralho[52];
    struct carta ultima;
    
    for(int i = 0; i < 52; i++) 
    {
        baralho[i].valor = 0; 
    }
    
    // faz com que a primeira carta do baralho seja 5 de valor para testar o +/- 1
    baralho[0].valor = 5;
    
    // +1
    ultima.valor = 6;
    CU_ASSERT_EQUAL(valores_Check(baralho, &ultima, 1), 1);
    
    // -1
    ultima.valor = 4;
    CU_ASSERT_EQUAL(valores_Check(baralho, &ultima, 1), 1);
    
    // +2
    ultima.valor = 7;
    CU_ASSERT_EQUAL(valores_Check(baralho, &ultima, 1), 0);
    
    // =
    ultima.valor = 5;
    CU_ASSERT_EQUAL(valores_Check(baralho, &ultima, 1), 0);
}

//  main do CUnit

int main() 
{
    CU_pSuite pSuite = NULL;

    // começa a registar os testes todos
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // adiciona a suite ao registo
    pSuite = CU_add_suite("Suite de Testes do Jogo de Cartas", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // adiciona os testes individuais a suite
    if ((NULL == CU_add_test(pSuite, "Testar criacao do baralho", test_cria_baralho)) ||
        (NULL == CU_add_test(pSuite, "Testar funcionalidade de puxar stock", test_puxar_Stock)) ||
        (NULL == CU_add_test(pSuite, "Testar check de valores validos (+1 e -1)", test_valores_Check))) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // descrição dos testes, passo a passo para ver exatamente onde falha
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    // limpa o registo e sai
    CU_cleanup_registry();
    return CU_get_error();
}