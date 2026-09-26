#ifndef CONSTS_H
#define CONSTS_H

// Definicoes de TRUE e FALSE para melhor entendimento do codigo
#define TRUE 1
#define FALSE 0

// Definicoes de estados para os jogadores
#define ESPERANDO_PARTIDA 1
#define POSICIONANDO_NAVIOS 2
#define AGUARDANDO_ADVERSARIO_POSICIONAR 3
#define EM_TURNO 4
#define ESPERANDO_TURNO 5
#define DERROTA 6
#define VITORIA 7

// Definicoes de codigos de retorno para funcoes de validacao
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
#define PARTIDA_FINALIZADA 13

// Definicao do tamanho maximo de navios permitidos
#define TAM_MAXIMO_NAVIO 6
#define NUMERO_NAVIOS_DISPONIVEIS {0,0,1,2,2,1}

// Definicao de cores
#define COR_VERMELHO "\033[31m"
#define COR_RESET "\033[0m"
// EM CASO DE ERRO NA EXIBICAO DAS CORES (COMO APARECER OS CODIGOS DA COR AO INVES DE TROCAR DE COR)
// POR FAVOR, COMENTAR OS DOIS DEFINES ACIMA E DESCOMENTAR OS DOIS DEFINES ABAIXO
// #define COR_VERMELHO ""
// #define COR_RESET ""

// Definicao de tamanho do buffer, porta padrao e maximo de clientes por servidor
#define TAM_BUFFER 1024*10
#define PORTA_PADRAO 8080
#define MAX_CLIENTES 4

// Bibliotecas necessarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#endif