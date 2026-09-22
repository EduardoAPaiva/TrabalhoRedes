#ifndef STRUCT_JOGO_H
#define STRUCT_JOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include "struct_cliente.h"

struct JOGO{
    CLIENTE *jogador1;
    CLIENTE *jogador2;

    char tabuleiro_naviosJ1[12][12];
    char tabuleiro_naviosJ2[12][12];
    char tabuleiro_tirosJ1[12][12];
    char tabuleiro_tirosJ2[12][12];

    int navios_disponiveisJ1[6];
    int navios_disponiveisJ2[6];

    int pontos_restantesJ1;
    int pontos_restantesJ2;

    int id;

    JOGO *prox;
    JOGO *ant;

};

LISTA_JOGOS* cria_lista_jogos();
JOGO *adiciona_jogo(CLIENTE *jogador1, CLIENTE *jogador2, LISTA_JOGOS *lista);
void deleta_jogo(JOGO *jogo, LISTA_JOGOS *lista);

#endif
