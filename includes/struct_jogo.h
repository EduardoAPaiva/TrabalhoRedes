#ifndef STRUCT_JOGO_H
#define STRUCT_JOGO_H

#include "struct_cliente.h"

// Definindo a struct jogo com suas variaveis relevantes
struct JOGO{
    CLIENTE *jogador1;      // Salva o jogador1 da partida
    CLIENTE *jogador2;      // Salva o jogador2 da partida

    char tabuleiro_naviosJ1[12][12];        // Tabuleiro de navios posicionados do jogador1
    char tabuleiro_naviosJ2[12][12];        // Tabuleiro de navios posicionados do jogador2
    char tabuleiro_tirosJ1[12][12];         // Tabuleiro de tiros que o jogador1 ja realizou
    char tabuleiro_tirosJ2[12][12];         // Tabuleiro de tiros que o jogador2 ja realizou

    int navios_disponiveisJ1[6];        // Navios disponiveis para o jogador1 ainda posicionar
    int navios_disponiveisJ2[6];        // Navios disponiveis para o jogador2 ainda posicionar

    int pontos_restantesJ1;         // Quantos pontos de vida (casas ocupadas por navios) o jogador1 ainda possui
    int pontos_restantesJ2;         // Quantos pontos de vida (casas ocupadas por navios) o jogador2 ainda possui

    int id;         // Indice da partida para reconhecimento

    JOGO *prox;     // Variavel que indica o proximo da lista encadeada
    JOGO *ant;      // Variavel que indica o anterior da lista encadeada

};

// Funcao que aloca uma lista de jogos e retorna a lista vazia
LISTA_JOGOS* cria_lista_jogos();
// Funcao que adiciona um jogo no final de uma lista encadeada
JOGO *adiciona_jogo(CLIENTE *jogador1, CLIENTE *jogador2, LISTA_JOGOS *lista);
// Funcao que remove um jogo especificado da lista encadeada
void deleta_jogo(JOGO *jogo, LISTA_JOGOS *lista);

#endif
