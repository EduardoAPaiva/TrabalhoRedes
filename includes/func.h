#ifndef FUNC_H
#define FUNC_H

#include "struct_cliente.h"
#include "struct_jogo.h"

void salvar_nome(char *nome, CLIENTE *cliente);
void preparar_tabuleiro(JOGO *partida);
void resetar_tabuleiro(CLIENTE *cliente);
char* texto_tabuleiro(char buffer[], char antes[], char depois[], char tabuleiro[12][12]);
char *texto_navios_disponiveis(int navios[6], char buffer[]);
void terminal_posicionando_navios(JOGO *partida, CLIENTE *remetente);
void terminal_aguardando_adversario_posicionar(JOGO *partida, CLIENTE *remetente);
void terminal_em_turno(JOGO *partida, CLIENTE *remetente);
void terminal_esperando_turno(JOGO *partida, CLIENTE *remetente);
void terminal_vitoria(JOGO *partida, CLIENTE *remetente);
void terminal_derrota(JOGO *partida, CLIENTE *remetente);
int validar_posicionamento(char buffer[], CLIENTE *cliente);
int validar_ataque(char buffer[], CLIENTE *cliente);
int validar_busca_partida(char buffer[], CLIENTE *cliente);
void aguardar_adversario_posicionar(CLIENTE *cliente);

#endif
