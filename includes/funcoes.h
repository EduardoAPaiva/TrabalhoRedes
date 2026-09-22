#ifndef FUNC_H
#define FUNC_H

#include "struct_cliente.h"
#include "struct_jogo.h"
#include "terminal.h"
#include "consts.h"

void salvar_nome(char *nome, CLIENTE *cliente);
void preparar_tabuleiro(JOGO *partida);
void resetar_tabuleiro(CLIENTE *cliente);
char* texto_tabuleiro(char buffer[], char antes[], char depois[], char tabuleiro[12][12]);
char *texto_navios_disponiveis(int navios[6], char buffer[]);
int validar_posicionamento(char buffer[], CLIENTE *cliente);
int validar_ataque(char buffer[], CLIENTE *cliente);
int validar_busca_partida(char buffer[], CLIENTE *cliente);
void aguardar_adversario_posicionar(CLIENTE *cliente);

#endif
