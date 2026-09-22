#ifndef STRUCT_CLIENTE_H
#define STRUCT_CLIENTE_H

#include "consts.h"

typedef struct JOGO JOGO;
typedef struct CLIENTE CLIENTE;

struct CLIENTE{
    char nickname[50];
    int socket;
    int cadastrado;

    int estado;

    JOGO *partida;

    CLIENTE *prox;
    CLIENTE *ant;

};

typedef JOGO* LISTA_JOGOS;
typedef CLIENTE* LISTA_CLIENTES;

LISTA_CLIENTES* cria_lista_clientes();
CLIENTE *adiciona_cliente(int socket, LISTA_CLIENTES *lista);
void deleta_cliente(CLIENTE *cliente, LISTA_CLIENTES *lista);

#endif
