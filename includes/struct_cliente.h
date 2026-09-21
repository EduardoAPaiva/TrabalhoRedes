#ifndef STRUCT_CLIENTE_H
#define STRUCT_CLIENTE_H

#define ESPERANDO_PARTIDA 1
#define POSICIONANDO_NAVIOS 2
#define AGUARDANDO_ADVERSARIO_POSICIONAR 3
#define EM_TURNO 4
#define ESPERANDO_TURNO 5

#define TAMANHO_INVALIDO 1
#define FORMATACAO_ERRADA 2
#define ORIENTACAO_INVALIDA 3
#define COLUNA_INVALIDA 4
#define LINHA_INVALIDA 5
#define RESETAR_TABULEIRO 6
#define NAVIO_FORA_DO_TABULEIRO 7
#define NAVIO_COLIDINDO 8
#define TAMANHO_DE_NAVIO_NAO_DISPONIVEL 9
#define NAVIOS_TODOS_POSICIONADOS 10
#define SUCESSO 11
#define CASA_JA_ATACADA 12

#define TAM_MAXIMO_NAVIO 6

#define TAM_BUFFER 1024*10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

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
