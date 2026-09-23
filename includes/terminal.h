#ifndef TERMINAL_H
#define TERMINAL_H

#include "struct_cliente.h"
#include "struct_jogo.h"
#include "funcoes.h"

// Funcao que envia o terminal do estado de "POSICIONANDO NAVIOS" para atualizar o cliente 
void terminal_posicionando_navios(JOGO *partida, CLIENTE *remetente);
// Funcao que envia o terminal do estado de "AGUARDANDO ADVERSARIO POSICIONAR" para atualizar o cliente
void terminal_aguardando_adversario_posicionar(JOGO *partida, CLIENTE *remetente);
// Funcao que envia o terminal do estado de "EM TURNO" para atualizar o cliente
void terminal_em_turno(JOGO *partida, CLIENTE *remetente);
// Funcao que envia o terminal do estado de "ESPERANDO TURNO" para atualizar o cliente
void terminal_esperando_turno(JOGO *partida, CLIENTE *remetente);
// Funcao que envia o terminal do estado de "VITORIA" para atualizar o cliente
void terminal_vitoria(JOGO *partida, CLIENTE *remetente);
// Funcao que envia o terminal do estado de "DERROTA" para atualizar o cliente
void terminal_derrota(JOGO *partida, CLIENTE *remetente);

#endif