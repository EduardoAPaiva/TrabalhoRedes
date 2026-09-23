#ifndef STRUCT_CLIENTE_H
#define STRUCT_CLIENTE_H

#include "consts.h"

// Primeiras definicoes das structs de JOGO e CLIENTE
typedef struct JOGO JOGO;
typedef struct CLIENTE CLIENTE;

// Definindo a struct cliente com suas variaveis relevantes
struct CLIENTE{
    char nickname[50];      // Nome do cliente
    int socket;             // Socket que o servidor enxerga aquele cliente
    int cadastrado;         // Booleano que diz se o cliente ja recebeu um nome ou nao

    int estado;             // Indica em que estado da partida o cliente esta

    JOGO *partida;          // Partida em que o cliente esta conectado (pode ser NULL em caso de nao estar numa partida ainda)

    CLIENTE *prox;          // Variavel que indica o proximo da lista encadeada
    CLIENTE *ant;           // Variavel que indica o anterior da lista encadeada

};

// Definindo os tipos de dados que indicam as listas (ponteiro para o primeiro elemento da lista encadeada)
typedef JOGO* LISTA_JOGOS;
typedef CLIENTE* LISTA_CLIENTES;

// Funcao que aloca uma lista de clientes e retorna a lista vazia
LISTA_CLIENTES* cria_lista_clientes();
// Funcao que adiciona um cliente no final de uma lista encadeada
CLIENTE *adiciona_cliente(int socket, LISTA_CLIENTES *lista);
// Funcao que remove um cliente especificado da lista encadeada
void deleta_cliente(CLIENTE *cliente, LISTA_CLIENTES *lista);

#endif
