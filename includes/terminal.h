#ifndef TERMINAL_H
#define TERMINAL_H

#include "struct_cliente.h"
#include "struct_jogo.h"
#include "funcoes.h"

void terminal_posicionando_navios(JOGO *partida, CLIENTE *remetente);
void terminal_aguardando_adversario_posicionar(JOGO *partida, CLIENTE *remetente);
void terminal_em_turno(JOGO *partida, CLIENTE *remetente);
void terminal_esperando_turno(JOGO *partida, CLIENTE *remetente);
void terminal_vitoria(JOGO *partida, CLIENTE *remetente);
void terminal_derrota(JOGO *partida, CLIENTE *remetente);

#endif