#include "../includes/struct_jogo.h"

extern int qtd_pontos;

// Funcao que aloca uma lista de jogos e retorna a lista vazia
LISTA_JOGOS* cria_lista_jogos(){

    // Aloca a lista
    LISTA_JOGOS *lista = (LISTA_JOGOS *)malloc(sizeof(LISTA_JOGOS));

    // Printa um erro caso a lista alocada ainda seja NULL
    if(lista == NULL)
        printf("ERRO AO ALOCAR LISTA DE JOGOS\n");

    // Retorna a lista alocada vazia
    return lista;

}

// Funcao que adiciona um jogo no final de uma lista encadeada
JOGO *adiciona_jogo(CLIENTE *jogador1, CLIENTE *jogador2, LISTA_JOGOS *lista){
    
    // Aloca o novo jogo
    JOGO *novo_jogo = (JOGO *)malloc(sizeof(JOGO));

    // Printa um erro caso o jogo alocado ainda seja NULL e retorna NULL
    if(novo_jogo == NULL){
        printf("ERRO NA ALOCACAO DO CLIENTE\n");
        return NULL;
    }

    // Atualiza o ponteiro de prox como NULL (pois o elemento sera adicionado ao final da lista)
    novo_jogo->prox = NULL;

    // Salva os jogadores na partida
    novo_jogo->jogador1 = jogador1;
    novo_jogo->jogador2 = jogador2;

    // Nos jogadores, adiciona a partida criada
    jogador1->partida = novo_jogo;
    jogador2->partida = novo_jogo;

    // Atualiza o status dos jogadores
    jogador1->estado = POSICIONANDO_NAVIOS;
    jogador2->estado = POSICIONANDO_NAVIOS;

    // Define a qtd de pontos de vida restantes para cada jogador
    novo_jogo->pontos_restantesJ1 = qtd_pontos;
    novo_jogo->pontos_restantesJ2 = qtd_pontos;

    // Caso a lista esteja vazia, adiciona o jogo no inicio e retorna o ponteiro do novo jogo
    if(*lista == NULL){
        *lista = novo_jogo;
        novo_jogo->ant = NULL;

        return novo_jogo;
    }

    // Cria um jogo auxiliar para percorrer a lista
    JOGO *aux = *lista;

    // Loop que caminha ate o final da lista encadeada
    while(aux->prox != NULL){
        aux = aux->prox;
    }

    // Adiciona o novo jogo ao final da lista
    aux->prox = novo_jogo;
    novo_jogo->ant = aux;

    // Retorna o ponteiro do novo jogo
    return novo_jogo;

}

// Funcao que remove um jogo especificado da lista encadeada
void deleta_jogo(JOGO *jogo, LISTA_JOGOS *lista){

    // Caso o jogo seja o primeiro da lista
    if(*lista == jogo){
        // Atualiza os ponteiros para remover o jogo
        *lista = jogo->prox;
        if(*lista != NULL) (*lista)->ant = NULL;
    }
    // Caso o jogo nao seja o primeiro da lista
    else{
        // Atualiza os ponteiros para remover o jogo
        (jogo->ant)->prox = jogo->prox;
        if(jogo->prox != NULL) (jogo->prox)->ant = jogo->ant;
    }

    // Libera a memoria alocada para o jogo
    free(jogo);

}