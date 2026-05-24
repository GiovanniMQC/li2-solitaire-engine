#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "logica.h"
#include "cartas.h"

int conta_pilhas_visiveis(Pilhas p);
int tamanhoS(char *s);
int avalia_regra_origem(FlagsMovimento regra, Pilhas p, int posOrig[]);
int tenta_auto_movimentos(EstadoJogo *g);

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

    struct baralho meu_baralho[1];
    
    //gera 3 cartas para a pilha
    meu_baralho[0].cartas[0].valor = 10; meu_baralho[0].cartas[0].naipe = 0; 
    meu_baralho[0].cartas[1].valor = 11; meu_baralho[0].cartas[1].naipe = 1; 
    meu_baralho[0].cartas[2].valor = 12; meu_baralho[0].cartas[2].naipe = 2; 
    
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

    struct baralho meu_baralho[1];
    cria_baralho(meu_baralho[0].cartas); 
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

    struct baralho baralho[1]; 
    Pilhas p = NULL;
    int cont, over, tam[14];

    iniciar_jogo(baralho, &p, &cont, tam, &over, 1);

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
    p->pilha[0].naipe = 0, p->pilha[0].valor = 6;
    p->pilha[1].naipe = 0, p->pilha[1].valor = 5;
    
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

void teste_AS_REI(void) {
    Pilhas p = malloc(sizeof(struct celula));
    p->numCartas = 3; p->prox = NULL; p->pilha = malloc(sizeof(struct carta) * 3);
    
    p->pilha[0].valor = 13; p->pilha[0].naipe = 0; // Fundo REI
    p->pilha[1].valor = 5;  p->pilha[1].naipe = 0; // Meio
    p->pilha[2].valor = 1;  p->pilha[2].naipe = 0; // Topo AS

    int posOrig[2] = {0, 0}; // Começa na base (REI)
    CU_ASSERT_EQUAL(REIfundo(p, posOrig), 0); // 0 = true
    CU_ASSERT_EQUAL(ASfundo(p, posOrig), 1);  // 1 = false

    int posOrigTopo[2] = {0, 2}; // Topo avalia sempre a ultima carta, indiferente da linha escolhida
    CU_ASSERT_EQUAL(AStopo(p, posOrigTopo), 0); // 0 = true
    CU_ASSERT_EQUAL(REItopo(p, posOrigTopo), 1); // 1 = false

    free(p->pilha), free(p);
}

void teste_sequencias_ordem(void) {
    Pilhas p = malloc(sizeof(struct celula));
    p->numCartas = 3; p->prox = NULL; p->pilha = malloc(sizeof(struct carta) * 3);
    
    // 5, 4, 3 (decrescente)
    p->pilha[0].valor = 5;
    p->pilha[1].valor = 4;
    p->pilha[2].valor = 3;
    
    int posOrig[2] = {0, 0};
    CU_ASSERT_EQUAL(decrescenteVerif(p, posOrig), 0); // Sucesso (0)
    CU_ASSERT_EQUAL(crescenteVerif(p, posOrig), 1);   // Falha (1)

    // 3, 4, 5 (crescente)
    p->pilha[0].valor = 3;
    p->pilha[1].valor = 4;
    p->pilha[2].valor = 5;
    CU_ASSERT_EQUAL(crescenteVerif(p, posOrig), 0); // Sucesso
    CU_ASSERT_EQUAL(decrescenteVerif(p, posOrig), 1);   // Falha

    free(p->pilha), free(p);
}

void teste_naipe_cor(void) {
    Pilhas p = malloc(sizeof(struct celula));
    p->numCartas = 3; p->prox = NULL; p->pilha = malloc(sizeof(struct carta) * 3);
    
    // Mesmo naipe (Copas = 0)
    p->pilha[0].naipe = 0; p->pilha[1].naipe = 0; p->pilha[2].naipe = 0;
    int posOrig[2] = {0, 0};
    CU_ASSERT_EQUAL(mesmoNaipe(p, posOrig), 0);
    CU_ASSERT_EQUAL(mesmaCor(p, posOrig), 0);

    // Mesma cor (Copas = 0, Ouros = 2) - Vermelhos
    p->pilha[0].naipe = 0; p->pilha[1].naipe = 2; p->pilha[2].naipe = 0;
    CU_ASSERT_EQUAL(mesmoNaipe(p, posOrig), 1); // Falha naipe
    CU_ASSERT_EQUAL(mesmaCor(p, posOrig), 0);   // Passa cor

    free(p->pilha), free(p);
}

void teste_pilhaDestinoVazia(void) {
    Pilhas p = malloc(sizeof(struct celula));
    p->numCartas = 0; p->prox = NULL; p->pilha = NULL;
    int posDest[2] = {0, 0};
    
    CU_ASSERT_EQUAL(pilhaDestinoVazia(p, posDest), 0); // Vazia = Sucesso
    
    p->numCartas = 1;
    p->pilha = malloc(sizeof(struct carta));
    CU_ASSERT_EQUAL(pilhaDestinoVazia(p, posDest), 1); // Não Vazia = Falha
    
    free(p->pilha), free(p);
}

void teste_EsoUma(void) {
    Pilhas p = malloc(sizeof(struct celula));
    p->numCartas = 3; p->prox = NULL; p->pilha = malloc(sizeof(struct carta) * 3);

    int posOrigVarias[2] = {0, 0}; // Tenta arrastar a partir da base (3 cartas juntas)
    CU_ASSERT_EQUAL(EsoUma(p, posOrigVarias), 1); // Falha, porque sao varias
    
    int posOrigUma[2] = {0, 2}; // Tenta arrastar a ultima carta do topo
    CU_ASSERT_EQUAL(EsoUma(p, posOrigUma), 0); // Sucesso, porque é só 1

    free(p->pilha), free(p);
}

void teste_pos_valida() {
    int p1[2] = {0, 0}, p2[2] = {1, 0};
    CU_ASSERT_EQUAL(pos_valida(p1, p2), 0); // Valido
    
    int p3[2] = {15, 0};
    CU_ASSERT_EQUAL(pos_valida(p3, p2), 1); // Invalido (fora do limite)
    
    int p4[2] = {1, 0};
    CU_ASSERT_EQUAL(pos_valida(p2, p4), 1); // Invalido (colunas iguais)
}

void teste_cartaChegada_Relacoes() {
    Pilhas orig = malloc(sizeof(struct celula));
    Pilhas dest = malloc(sizeof(struct celula));
    orig->numCartas = 1; orig->prox = dest; orig->pilha = malloc(sizeof(struct carta));
    dest->numCartas = 1; dest->prox = NULL; dest->pilha = malloc(sizeof(struct carta));

    orig->pilha[0].valor = 5;
    dest->pilha[0].valor = 6;
    int posOrig[2] = {0,0}, posDest[2] = {1,0};
    
    CU_ASSERT_EQUAL(cartaChegadaEmaior(orig, posOrig, posDest), 0); // 6 é maior que 5
    CU_ASSERT_EQUAL(cartaChegadaEmenor(orig, posOrig, posDest), 1);
    CU_ASSERT_EQUAL(cartaMaiorOuMenor(orig, posOrig, posDest), 0);

    dest->pilha[0].valor = 4;
    CU_ASSERT_EQUAL(cartaChegadaEmaior(orig, posOrig, posDest), 1);
    CU_ASSERT_EQUAL(cartaChegadaEmenor(orig, posOrig, posDest), 0); // 4 é menor que 5
    CU_ASSERT_EQUAL(cartaMaiorOuMenor(orig, posOrig, posDest), 0);

    free(orig->pilha); free(dest->pilha); free(orig); free(dest);
}

void teste_mesmoNaipeTopo_mesmaCorTopo() {
    Pilhas orig = malloc(sizeof(struct celula));
    Pilhas dest = malloc(sizeof(struct celula));
    orig->numCartas = 1; orig->prox = dest; orig->pilha = malloc(sizeof(struct carta));
    dest->numCartas = 1; dest->prox = NULL; dest->pilha = malloc(sizeof(struct carta));

    orig->pilha[0].naipe = 0; // Copas
    dest->pilha[0].naipe = 0; // Copas
    int posOrig[2] = {0,0}, posDest[2] = {1,0};

    CU_ASSERT_EQUAL(mesmoNaipeTopo(orig, posOrig, posDest), 0);
    CU_ASSERT_EQUAL(mesmaCorTopo(orig, posOrig, posDest), 0);

    dest->pilha[0].naipe = 2; // Ouros (mesma cor, naipe dif)
    CU_ASSERT_EQUAL(mesmoNaipeTopo(orig, posOrig, posDest), 1);
    CU_ASSERT_EQUAL(mesmaCorTopo(orig, posOrig, posDest), 0);

    free(orig->pilha); free(dest->pilha); free(orig); free(dest);
}

void teste_ganhou() {
    Pilhas p = malloc(sizeof(struct celula));
    p->tipo_Pilha = strdup("TAB"); p->numCartas = 0; p->prox = NULL; p->pilha = NULL;

    WinDef w;
    w.tipo = strdup("TAB");
    w.qntsWins = 1;
    w.numCartas = malloc(sizeof(int));
    w.numCartas[0] = 0;

    CU_ASSERT_EQUAL(ganhou(p, w), 0); // 0 = sucesso (pilha está vazia como requisitado)

    p->numCartas = 1;
    CU_ASSERT_EQUAL(ganhou(p, w), 1); // 1 = falha (tem 1 carta)

    free(p->tipo_Pilha); free(p); free(w.tipo); free(w.numCartas);
}

void teste_avalia_regras() {
    Pilhas orig = malloc(sizeof(struct celula));
    Pilhas dest = malloc(sizeof(struct celula));
    orig->numCartas = 1; orig->prox = dest; orig->pilha = malloc(sizeof(struct carta));
    dest->numCartas = 0; dest->prox = NULL; dest->pilha = NULL;

    orig->pilha[0].valor = 1; // AS
    int posOrig[2] = {0,0}, posDest[2] = {1,0};

    CU_ASSERT_EQUAL(avalia_regra_origem(TOPO_AS, orig, posOrig), 0); // Sucesso
    CU_ASSERT_EQUAL(avalia_regra_origem(TOPO_REI, orig, posOrig), 1); // Falha
    CU_ASSERT_EQUAL(avalia_regra(PILHA_VAZIA, orig, posOrig, posDest), 0); // Sucesso

    free(orig->pilha); free(orig); free(dest);
}

void teste_auto_movimentos() {
    EstadoJogo g;
    g.pilhas = malloc(sizeof(struct celula));
    g.pilhas->tipo_Pilha = strdup("TAB"); g.pilhas->numCartas = 1; 
    g.pilhas->pilha = malloc(sizeof(struct carta)); g.pilhas->pilha[0].valor = 13;
    
    g.pilhas->prox = malloc(sizeof(struct celula));
    g.pilhas->prox->tipo_Pilha = strdup("FUND"); g.pilhas->prox->numCartas = 0;
    g.pilhas->prox->pilha = NULL; g.pilhas->prox->prox = NULL;

    g.qts_auto_movs = 1;
    g.auto_movs = malloc(sizeof(MovimentoDef));
    g.auto_movs[0].tipo_origem = strdup("TAB");
    g.auto_movs[0].tipo_destino = strdup("FUND");
    g.auto_movs[0].qts_flags = 1;
    g.auto_movs[0].flags = malloc(sizeof(FlagsMovimento));
    g.auto_movs[0].flags[0] = PILHA_VAZIA;

    // Testa a cadeia toda (tenta_auto, tenta_destino, avalia_regras, mover)
    CU_ASSERT_EQUAL(tenta_auto_movimentos(&g), 1);
    CU_ASSERT_EQUAL(g.pilhas->numCartas, 0); // Saiu do TAB
    CU_ASSERT_EQUAL(g.pilhas->prox->numCartas, 1); // Chegou no FUND

    free(g.pilhas->prox->tipo_Pilha); free(g.pilhas->prox->pilha); free(g.pilhas->prox);
    free(g.pilhas->tipo_Pilha); free(g.pilhas->pilha); free(g.pilhas);
    free(g.auto_movs[0].tipo_origem); free(g.auto_movs[0].tipo_destino); free(g.auto_movs[0].flags); free(g.auto_movs);
}

void teste_secundarias_tamanho_visiveis() {
    CU_ASSERT_EQUAL(tamanhoS("teste"), 5);

    Pilhas p1 = malloc(sizeof(struct celula));
    Pilhas p2 = malloc(sizeof(struct celula));
    p1->flags = strdup("="); p1->prox = p2;
    p2->flags = strdup("_"); p2->prox = NULL;

    CU_ASSERT_EQUAL(conta_pilhas_visiveis(p1), 1); // Apenas a '=' é contada

    free(p1->flags); free(p1);
    free(p2->flags); free(p2);
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
    CU_add_test(pSuite, "teste_criaBaralho", teste_criaBaralho);
    CU_add_test(pSuite, "teste_shuffle", teste_shuffle);
    CU_add_test(pSuite, "teste_criaPilha", teste_criaPilha);
    CU_add_test(pSuite, "teste_procuraPilha", teste_procuraPilha);
    CU_add_test(pSuite, "teste_liberaMem", teste_liberaMem);
    CU_add_test(pSuite, "teste_insereCartas", teste_insereCartas);
    CU_add_test(pSuite, "teste_corrige_seq_cartas", teste_corrige_seq_cartas);
    CU_add_test(pSuite, "teste_moverCartas", teste_moverCartas);
    CU_add_test(pSuite, "teste_acharLimite", teste_acharLimite);
    CU_add_test(pSuite, "teste_cartaCheck", teste_cartaCheck);
    CU_add_test(pSuite, "teste_validaJogada", teste_validaJogada);
    CU_add_test(pSuite, "teste_iniciarJogo", teste_iniciarJogo);
    CU_add_test(pSuite, "teste_sequencias", teste_sequencias);
    CU_add_test(pSuite, "teste_existe_jogadaValida", teste_existe_jogadaValida);
    CU_add_test(pSuite, "teste_gameOver", teste_gameOver);
    CU_add_test(pSuite, "teste_AS_REI", teste_AS_REI);
    CU_add_test(pSuite, "teste_sequencias_ordem", teste_sequencias_ordem);
    CU_add_test(pSuite, "teste_naipe_cor", teste_naipe_cor);
    CU_add_test(pSuite, "teste_pilhaDestinoVazia", teste_pilhaDestinoVazia);
    CU_add_test(pSuite, "teste_EsoUma", teste_EsoUma);
    CU_add_test(pSuite, "teste_pos_valida", teste_pos_valida);
    CU_add_test(pSuite, "teste_cartaChegada_Relacoes", teste_cartaChegada_Relacoes);
    CU_add_test(pSuite, "teste_mesmoNaipeTopo_mesmaCorTopo", teste_mesmoNaipeTopo_mesmaCorTopo);
    CU_add_test(pSuite, "teste_ganhou", teste_ganhou);
    CU_add_test(pSuite, "teste_avalia_regras", teste_avalia_regras);
    CU_add_test(pSuite, "teste_auto_movimentos", teste_auto_movimentos);
    CU_add_test(pSuite, "teste_secundarias_tamanho_visiveis", teste_secundarias_tamanho_visiveis);

    // corre tudo e limpa a memoria
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return CU_get_error();
}