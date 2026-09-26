#include "../includes/terminal.h"

// Funcao que envia o terminal do estado de "POSICIONANDO NAVIOS" para atualizar o cliente 
void terminal_posicionando_navios(JOGO *partida, CLIENTE *remetente){

    // Define os buffers temporarios
    char antes[150];
    char texto[TAM_BUFFER];
    char depois[TAM_BUFFER];

    // Caso o cliente seja o jogador1 da partida
    if(remetente == partida->jogador1){
        // Adiciona o texto antes, o tabuleiro e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEU TABULEIRO:", partida->jogador2->nickname);
        texto_tabuleiro(texto, antes, texto_navios_disponiveis(partida->navios_disponiveisJ1, depois), partida->tabuleiro_naviosJ1, -1, -1);
        send(partida->jogador1->socket, texto, strlen(texto), 0);
    }
    // Caso o cliente seja o jogador2 da partida
    else{
        // Adiciona o texto antes, o tabuleiro e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEU TABULEIRO:", partida->jogador1->nickname);
        texto_tabuleiro(texto, antes, texto_navios_disponiveis(partida->navios_disponiveisJ2, depois), partida->tabuleiro_naviosJ2, -1, -1);
        send(partida->jogador2->socket, texto, strlen(texto), 0);
    }

}

// Funcao que envia o terminal do estado de "AGUARDANDO ADVERSARIO POSICIONAR" para atualizar o cliente
void terminal_aguardando_adversario_posicionar(JOGO *partida, CLIENTE *remetente){

    // Define os buffers temporarios
    char antes[150];
    char texto[TAM_BUFFER];
    char depois[TAM_BUFFER];

    // Caso o cliente seja o jogador1 da partida
    if(remetente == partida->jogador1){
        // Adiciona o texto antes, o tabuleiro e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEU TABULEIRO:", partida->jogador2->nickname);
        texto_tabuleiro(texto, antes, "VOCE TERMINOU DE COLOCAR OS NAVIOS!!\nAGUARDANDO ADVERSARIO...", partida->tabuleiro_naviosJ1, -1, -1);
        send(partida->jogador1->socket, texto, strlen(texto), 0);
    }
    // Caso o cliente seja o jogador2 da partida
    else{
        // Adiciona o texto antes, o tabuleiro e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEU TABULEIRO:", partida->jogador1->nickname);
        texto_tabuleiro(texto, antes, "VOCE TERMINOU DE COLOCAR OS NAVIOS!!\nAGUARDANDO ADVERSARIO...", partida->tabuleiro_naviosJ2, -1, -1);
        send(partida->jogador2->socket, texto, strlen(texto), 0);
    }

}

// Funcao que envia o terminal do estado de "EM TURNO" para atualizar o cliente
void terminal_em_turno(JOGO *partida, CLIENTE *remetente){

    // Define os buffers temporarios
    char antes[150];
    char texto1[TAM_BUFFER];
    char texto2[TAM_BUFFER];
    char depois[TAM_BUFFER];

    // Caso o cliente seja o jogador1 da partida
    if(remetente == partida->jogador1){
        // Adiciona o texto antes, os dois tabuleiros e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador2->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ1, partida->pontos_restantesJ2);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ1, partida->ultimo_tiroJ2[0], partida->ultimo_tiroJ2[1]);
        texto_tabuleiro(texto2, texto1, "ESTA NA SUA VEZ DE JOGAR, ESCOLHA A CASA INIMIGA A SER ATACADA: \n", partida->tabuleiro_tirosJ1, partida->ultimo_tiroJ1[0], partida->ultimo_tiroJ1[1]);
        send(partida->jogador1->socket, texto2, strlen(texto2), 0);
    }
    // Caso o cliente seja o jogador2 da partida
    else{
        // Adiciona o texto antes, os dois tabuleiros e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador1->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ2, partida->pontos_restantesJ1);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ2, partida->ultimo_tiroJ1[0], partida->ultimo_tiroJ1[1]);
        texto_tabuleiro(texto2, texto1, "ESTA NA SUA VEZ DE JOGAR, ESCOLHA A CASA INIMIGA A SER ATACADA: \n", partida->tabuleiro_tirosJ2, partida->ultimo_tiroJ2[0], partida->ultimo_tiroJ2[1]);
        send(partida->jogador2->socket, texto2, strlen(texto2), 0);
    }

}

// Funcao que envia o terminal do estado de "ESPERANDO TURNO" para atualizar o cliente
void terminal_esperando_turno(JOGO *partida, CLIENTE *remetente){

    // Define os buffers temporarios
    char antes[150];
    char texto1[TAM_BUFFER];
    char texto2[TAM_BUFFER];
    char depois[TAM_BUFFER];

    // Caso o cliente seja o jogador1 da partida
    if(remetente == partida->jogador1){
        // Adiciona o texto antes, os dois tabuleiros e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador2->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ1, partida->pontos_restantesJ2);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ1, partida->ultimo_tiroJ2[0], partida->ultimo_tiroJ2[1]);
        texto_tabuleiro(texto2, texto1, "ESTA NA VEZ DO ADVERSARIO JOGAR, AGUARDE... \n", partida->tabuleiro_tirosJ1, partida->ultimo_tiroJ1[0], partida->ultimo_tiroJ1[1]);
        send(partida->jogador1->socket, texto2, strlen(texto2), 0);
    }
    // Caso o cliente seja o jogador2 da partida
    else{
        // Adiciona o texto antes, os dois tabuleiros e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador1->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ2, partida->pontos_restantesJ1);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ2, partida->ultimo_tiroJ1[0], partida->ultimo_tiroJ1[1]);
        texto_tabuleiro(texto2, texto1, "ESTA NA VEZ DO ADVERSARIO JOGAR, AGUARDE... \n", partida->tabuleiro_tirosJ2, partida->ultimo_tiroJ2[0], partida->ultimo_tiroJ2[1]);
        send(partida->jogador2->socket, texto2, strlen(texto2), 0);
    }

}

// Funcao que envia o terminal do estado de "VITORIA" para atualizar o cliente
void terminal_vitoria(JOGO *partida, CLIENTE *remetente){

    // Define os buffers temporarios
    char antes[150];
    char texto1[TAM_BUFFER];
    char texto2[TAM_BUFFER];
    char depois[TAM_BUFFER];

    // Caso o cliente seja o jogador1 da partida
    if(remetente == partida->jogador1){
        // Adiciona o texto antes, os dois tabuleiros e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador2->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ1, partida->pontos_restantesJ2);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ1, partida->ultimo_tiroJ2[0], partida->ultimo_tiroJ2[1]);
        texto_tabuleiro(texto2, texto1, "PARABENS!!! VOCE VENCEU =)\nDIGITE \"BUSCAR\" PARA ENCONTRAR NOVA PARTIDA\n", partida->tabuleiro_tirosJ1, partida->ultimo_tiroJ1[0], partida->ultimo_tiroJ1[1]);
        send(partida->jogador1->socket, texto2, strlen(texto2), 0);
    }
    // Caso o cliente seja o jogador2 da partida
    else{
        // Adiciona o texto antes, os dois tabuleiros e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador1->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ2, partida->pontos_restantesJ1);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ2, partida->ultimo_tiroJ1[0], partida->ultimo_tiroJ1[1]);
        texto_tabuleiro(texto2, texto1, "PARABENS!!! VOCE VENCEU =)\nDIGITE \"BUSCAR\" PARA ENCONTRAR NOVA PARTIDA\n", partida->tabuleiro_tirosJ2, partida->ultimo_tiroJ2[0], partida->ultimo_tiroJ2[1]);
        send(partida->jogador2->socket, texto2, strlen(texto2), 0);
    }
}

// Funcao que envia o terminal do estado de "DERROTA" para atualizar o cliente
void terminal_derrota(JOGO *partida, CLIENTE *remetente){

    // Define os buffers temporarios
    char antes[150];
    char texto1[TAM_BUFFER];
    char texto2[TAM_BUFFER];
    char depois[TAM_BUFFER];

    // Caso o cliente seja o jogador1 da partida
    if(remetente == partida->jogador1){
        // Adiciona o texto antes, os dois tabuleiros e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador2->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ1, partida->pontos_restantesJ2);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ1, partida->ultimo_tiroJ2[0], partida->ultimo_tiroJ2[1]);
        texto_tabuleiro(texto2, texto1, "VOCE PERDEU =( TENTE A SORTE NA PROXIMA\nDIGITE \"BUSCAR\" PARA ENCONTRAR NOVA PARTIDA\n", partida->tabuleiro_tirosJ1, partida->ultimo_tiroJ1[0], partida->ultimo_tiroJ1[1]);
        send(partida->jogador1->socket, texto2, strlen(texto2), 0);
    }
    // Caso o cliente seja o jogador2 da partida
    else{
        // Adiciona o texto antes, os dois tabuleiros e o texto depois e envia para o cliente
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador1->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ2, partida->pontos_restantesJ1);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ2, partida->ultimo_tiroJ1[0], partida->ultimo_tiroJ1[1]);
        texto_tabuleiro(texto2, texto1, "VOCE PERDEU =( TENTE A SORTE NA PROXIMA\nDIGITE \"BUSCAR\" PARA ENCONTRAR NOVA PARTIDA\n", partida->tabuleiro_tirosJ2, partida->ultimo_tiroJ2[0], partida->ultimo_tiroJ2[1]);
        send(partida->jogador2->socket, texto2, strlen(texto2), 0);
    }
}
