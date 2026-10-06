#ifndef RECV_SEND_H
#define RECV_SEND_H

#include "consts.h"

// Funcao que garante que o recv recebeu toda a mensagem
int enviar_tudo(int socket, const char buf[]);
// Funcao que garante que o send enviou toda a mensagem
int receber_tudo(int socket, char buf[]);

#endif