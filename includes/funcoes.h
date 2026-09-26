#ifndef FUNC_H
#define FUNC_H

#include "struct_cliente.h"
#include "struct_jogo.h"
#include "terminal.h"
#include "consts.h"

// Funcao que salva a string recebida no nome (nickname) do cliente
void salvar_nome(char *nome, CLIENTE *cliente);
// Funcao que coloca os caracteres iniciais corretos nos tabuleiros da partida 
void preparar_tabuleiro(JOGO *partida);
// Funcao que reseta o posicionamento de navios. Ou seja, transforma o tabuleiro no padrão vazio
void resetar_tabuleiro(CLIENTE *cliente);
// Funcao que coloca num buffer um texto, depois um tabuleiro, e depois outro texto. Esse buffer sera enviado para atualizar o terminal do cliente
void* texto_tabuleiro(char buffer[], char antes[], char depois[], char tabuleiro[12][12], int linhatiro, int colunatiro);
// Funcao que analisa quantos navios uma lista de navios tem de disponibilidade, e cria o texto pra isso
void* texto_navios_disponiveis(int navios[TAM_MAXIMO_NAVIO], char buffer[]);
// Funcao que analisa se um comando de posicionamento de navio recebido pelo cliente esta valido de acordo com o estado da partida e do tabuleiro
int validar_posicionamento(char buffer[], CLIENTE *cliente);
// Funcao que analisa se um comando de ataque recebido pelo cliente esta valido de acordo com o estado da partida e do tabuleiro
int validar_ataque(char buffer[], CLIENTE *cliente);
// Funcao que analisa se o comando recebido é exatamente a string "BUSCAR"
int validar_busca_partida(char buffer[], CLIENTE *cliente);
// Funcao que altera o estado do cliente para aguardando posicionar. Analisa se o outro jogador ja terminou de posicionar e inicia a partida
void aguardar_adversario_posicionar(CLIENTE *cliente);

#endif
