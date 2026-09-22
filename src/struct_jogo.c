#include "../includes/struct_jogo.h"

extern int qtd_pontos;

LISTA_JOGOS* cria_lista_jogos(){

    LISTA_JOGOS *lista = (LISTA_JOGOS *)malloc(sizeof(LISTA_JOGOS));

    if(lista == NULL)
        printf("ERRO AO ALOCAR LISTA DE JOGOS\n");

    return lista;

}

JOGO *adiciona_jogo(CLIENTE *jogador1, CLIENTE *jogador2, LISTA_JOGOS *lista){
    
    JOGO *novo_jogo = (JOGO *)malloc(sizeof(JOGO));

    novo_jogo->prox = NULL;
    novo_jogo->jogador1 = jogador1;
    novo_jogo->jogador2 = jogador2;

    jogador1->partida = novo_jogo;
    jogador2->partida = novo_jogo;

    jogador1->estado = POSICIONANDO_NAVIOS;
    jogador2->estado = POSICIONANDO_NAVIOS;

    novo_jogo->pontos_restantesJ1 = qtd_pontos;
    novo_jogo->pontos_restantesJ2 = qtd_pontos;

    if(*lista == NULL){
        *lista = novo_jogo;
        novo_jogo->ant = NULL;

        return novo_jogo;
    }

    JOGO *aux = *lista;

    while(aux->prox != NULL){
        aux = aux->prox;
    }

    aux->prox = novo_jogo;
    novo_jogo->ant = aux;

    return novo_jogo;

}

void deleta_jogo(JOGO *jogo, LISTA_JOGOS *lista){

    if(*lista == jogo){
        *lista = jogo->prox;
        if(*lista != NULL) (*lista)->ant = NULL;
    }

    else{
        (jogo->ant)->prox = jogo->prox;
        if(jogo->prox != NULL) (jogo->prox)->ant = jogo->ant;
    }

    free(jogo);

}