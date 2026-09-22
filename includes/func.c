#include "func.h"

int numero_navios[6] = {0,0,1,2,2,1};
int qtd_pontos = 0;

void salvar_nome(char *nome, CLIENTE *cliente){
    strcpy(cliente->nickname, nome);
    cliente->cadastrado = 1;
}

void preparar_tabuleiro(JOGO *partida){

    for(int i = 0; i<12; i++){
        for(int j = 0; j<12; j++){

            if(i == 0 || i == 11){
                (partida->tabuleiro_naviosJ1)[i][j] = '=';
                (partida->tabuleiro_naviosJ2)[i][j] = '=';
                (partida->tabuleiro_tirosJ1)[i][j] = '=';
                (partida->tabuleiro_tirosJ2)[i][j] = '=';
            }
            
            else if(j == 0 || j == 11){
                (partida->tabuleiro_naviosJ1)[i][j] = '|';
                (partida->tabuleiro_naviosJ2)[i][j] = '|';
                (partida->tabuleiro_tirosJ1)[i][j] = '|';
                (partida->tabuleiro_tirosJ2)[i][j] = '|';
            }
            
            else{
                (partida->tabuleiro_naviosJ1)[i][j] = ' ';
                (partida->tabuleiro_naviosJ2)[i][j] = ' ';
                (partida->tabuleiro_tirosJ1)[i][j] = ' ';
                (partida->tabuleiro_tirosJ2)[i][j] = ' ';
            }

        }
    }

    for(int i = 0; i<TAM_MAXIMO_NAVIO; i++){
        partida->navios_disponiveisJ1[i] = numero_navios[i];
        partida->navios_disponiveisJ2[i] = numero_navios[i];
    }

}

void resetar_tabuleiro(CLIENTE *cliente){

    JOGO *partida = cliente->partida;

    for(int i = 0; i<12; i++){
        for(int j = 0; j<12; j++){

            if(i == 0 || i == 11){
                if(partida->jogador1 == cliente)
                    (partida->tabuleiro_naviosJ1)[i][j] = '=';
                else
                    (partida->tabuleiro_naviosJ2)[i][j] = '=';
            }
            
            else if(j == 0 || j == 11){
                if(partida->jogador1 == cliente)
                    (partida->tabuleiro_naviosJ1)[i][j] = '|';
                else
                    (partida->tabuleiro_naviosJ2)[i][j] = '|';
            }
            
            else{
                if(partida->jogador1 == cliente)
                    (partida->tabuleiro_naviosJ1)[i][j] = ' ';
                else
                    (partida->tabuleiro_naviosJ2)[i][j] = ' ';
            }

        }
    }

    for(int i = 0; i<TAM_MAXIMO_NAVIO; i++){
        partida->navios_disponiveisJ1[i] = numero_navios[i];
        partida->navios_disponiveisJ2[i] = numero_navios[i];
    }

}

char* texto_tabuleiro(char buffer[], char antes[], char depois[], char tabuleiro[12][12]){

    int pos = 0;

    pos += sprintf(buffer + pos, "%s\n", antes);

    pos += sprintf(buffer + pos, "      A  B  C  D  E  F  G  H  I  J   \n");
    pos += sprintf(buffer + pos, "   ==================================\n");

    for(int i = 1; i<11; i++){
        for(int j = 0; j<12; j++){
            
            if(j == 0)
                pos += sprintf(buffer + pos, "%d ", i-1);

            pos += sprintf(buffer + pos, " %c ", tabuleiro[i][j]);

        }

        pos += sprintf(buffer + pos, "\n");

    }

    pos += sprintf(buffer + pos, "   ==================================\n");

    pos += sprintf(buffer + pos, "\n%s\n", depois);

}

char *texto_navios_disponiveis(int navios[TAM_MAXIMO_NAVIO], char buffer[]){

    int flag_sem_navios = 1;
    int pos = 0;

    pos += sprintf(buffer + pos, "POSICIONE SEUS NAVIOS\nVOCE AINDA TEM:\n");

    for(int i = 0; i<TAM_MAXIMO_NAVIO; i++){

        if(navios[i] != 0){
            flag_sem_navios = 0;
            pos += sprintf(buffer + pos, "%d NAVIO DE TAMANHO %d\n", navios[i], i);
        }

    }

    pos += sprintf(buffer + pos, "\nONDE DESEJA POSICIONAR: ");

    return buffer;

}

void terminal_posicionando_navios(JOGO *partida, CLIENTE *remetente){

    char antes[150];
    char texto[TAM_BUFFER];
    char depois[TAM_BUFFER];

    if(remetente == partida->jogador1){
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEU TABULEIRO:", partida->jogador2->nickname);
        texto_tabuleiro(texto, antes, texto_navios_disponiveis(partida->navios_disponiveisJ1, depois), partida->tabuleiro_naviosJ1);
        send(partida->jogador1->socket, texto, strlen(texto), 0);
    }

    else{
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEU TABULEIRO:", partida->jogador1->nickname);
        texto_tabuleiro(texto, antes, texto_navios_disponiveis(partida->navios_disponiveisJ2, depois), partida->tabuleiro_naviosJ2);
        send(partida->jogador2->socket, texto, strlen(texto), 0);
    }

}

void terminal_aguardando_adversario_posicionar(JOGO *partida, CLIENTE *remetente){

    char antes[150];
    char texto[TAM_BUFFER];
    char depois[TAM_BUFFER];

    if(remetente == partida->jogador1){
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEU TABULEIRO:", partida->jogador2->nickname);
        texto_tabuleiro(texto, antes, "VOCE TERMINOU DE COLOCAR OS NAVIOS!!\nAGUARDANDO ADVERSARIO...", partida->tabuleiro_naviosJ1);
        send(partida->jogador1->socket, texto, strlen(texto), 0);
    }

    else{
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEU TABULEIRO:", partida->jogador1->nickname);
        texto_tabuleiro(texto, antes, "VOCE TERMINOU DE COLOCAR OS NAVIOS!!\nAGUARDANDO ADVERSARIO...", partida->tabuleiro_naviosJ2);
        send(partida->jogador2->socket, texto, strlen(texto), 0);
    }

}

void terminal_em_turno(JOGO *partida, CLIENTE *remetente){

    char antes[150];
    char texto1[TAM_BUFFER];
    char texto2[TAM_BUFFER];
    char depois[TAM_BUFFER];

    if(remetente == partida->jogador1){
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador2->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ1, partida->pontos_restantesJ2);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ1);
        texto_tabuleiro(texto2, texto1, "ESTA NA SUA VEZ DE JOGAR, ESCOLHA A CASA INIMIGA A SER ATACADA: \n", partida->tabuleiro_tirosJ1);
        send(partida->jogador1->socket, texto2, strlen(texto2), 0);
    }

    else{
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador1->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ2, partida->pontos_restantesJ1);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ2);
        texto_tabuleiro(texto2, texto1, "ESTA NA SUA VEZ DE JOGAR, ESCOLHA A CASA INIMIGA A SER ATACADA: \n", partida->tabuleiro_tirosJ2);
        send(partida->jogador2->socket, texto2, strlen(texto2), 0);
    }

}

void terminal_esperando_turno(JOGO *partida, CLIENTE *remetente){

    char antes[150];
    char texto1[TAM_BUFFER];
    char texto2[TAM_BUFFER];
    char depois[TAM_BUFFER];

    if(remetente == partida->jogador1){
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador2->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ1, partida->pontos_restantesJ2);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ1);
        texto_tabuleiro(texto2, texto1, "ESTA NA VEZ DO ADVERSARIO JOGAR, AGUARDE... \n", partida->tabuleiro_tirosJ1);
        send(partida->jogador1->socket, texto2, strlen(texto2), 0);
    }

    else{
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador1->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ2, partida->pontos_restantesJ1);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ2);
        texto_tabuleiro(texto2, texto1, "ESTA NA VEZ DO ADVERSARIO JOGAR, AGUARDE... \n", partida->tabuleiro_tirosJ2);
        send(partida->jogador2->socket, texto2, strlen(texto2), 0);
    }

}

void terminal_vitoria(JOGO *partida, CLIENTE *remetente){
    char antes[150];
    char texto1[TAM_BUFFER];
    char texto2[TAM_BUFFER];
    char depois[TAM_BUFFER];

    if(remetente == partida->jogador1){
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador2->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ1, partida->pontos_restantesJ2);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ1);
        texto_tabuleiro(texto2, texto1, "PARABENS!!! VOCE VENCEU =)\nDIGITE \"BUSCAR\" PARA ENCONTRAR NOVA PARTIDA\n", partida->tabuleiro_tirosJ1);
        send(partida->jogador1->socket, texto2, strlen(texto2), 0);
    }

    else{
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador1->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ2, partida->pontos_restantesJ1);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ2);
        texto_tabuleiro(texto2, texto1, "PARABENS!!! VOCE VENCEU =)\nDIGITE \"BUSCAR\" PARA ENCONTRAR NOVA PARTIDA\n", partida->tabuleiro_tirosJ2);
        send(partida->jogador2->socket, texto2, strlen(texto2), 0);
    }
}

void terminal_derrota(JOGO *partida, CLIENTE *remetente){
    char antes[150];
    char texto1[TAM_BUFFER];
    char texto2[TAM_BUFFER];
    char depois[TAM_BUFFER];

    if(remetente == partida->jogador1){
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador2->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ1, partida->pontos_restantesJ2);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ1);
        texto_tabuleiro(texto2, texto1, "VOCE PERDEU =( TENTE A SORTE NA PROXIMA\nDIGITE \"BUSCAR\" PARA ENCONTRAR NOVA PARTIDA\n", partida->tabuleiro_tirosJ1);
        send(partida->jogador1->socket, texto2, strlen(texto2), 0);
    }

    else{
        sprintf(antes, "PARTIDA ENCONTRADA!\nSEU ADVERSARIO: %s\n\n\nSEUS NAVIOS:\n", partida->jogador1->nickname);
        sprintf(depois, "VIDAS RESTANTES: %d\nVIDAS RESTANTES DO ADVERSARIO: %d\n\n\nSEUS TIROS: ", partida->pontos_restantesJ2, partida->pontos_restantesJ1);
        texto_tabuleiro(texto1, antes, depois, partida->tabuleiro_naviosJ2);
        texto_tabuleiro(texto2, texto1, "VOCE PERDEU =( TENTE A SORTE NA PROXIMA\nDIGITE \"BUSCAR\" PARA ENCONTRAR NOVA PARTIDA\n", partida->tabuleiro_tirosJ2);
        send(partida->jogador2->socket, texto2, strlen(texto2), 0);
    }
}

int validar_posicionamento(char buffer[], CLIENTE *cliente){

    if(buffer[0] == 'R' && (buffer[1] == '\n' || buffer[1] == '\0'))
        return RESETAR_TABULEIRO;

    if(buffer[0] != '2' && buffer[0] != '3' && buffer[0] != '4' && buffer[0] != '5')
        return TAMANHO_INVALIDO;

    if(buffer[1] != ' ')
        return FORMATACAO_ERRADA;

    if(buffer[2] != 'H' && buffer[2] !=  'V')
        return ORIENTACAO_INVALIDA;

    if(buffer[3] != ' ')
        return FORMATACAO_ERRADA;

    if(buffer[4] < 'A' || buffer[4] > 'J')
        return COLUNA_INVALIDA;

    if(buffer[5] < '0' || buffer[5] > '9')
        return LINHA_INVALIDA;

    if(buffer[6] != '\0' && buffer[6] != '\n')
        return FORMATACAO_ERRADA;

    int linha = buffer[5]-'0'+1;
    int coluna = buffer[4] - 'A'+1;
    int tamanho = buffer[0] - '0';

    JOGO *partida = cliente->partida;

    if(cliente == partida->jogador1 && partida->navios_disponiveisJ1[tamanho] == 0)
        return TAMANHO_DE_NAVIO_NAO_DISPONIVEL;

    if(cliente == partida->jogador2 && partida->navios_disponiveisJ2[tamanho] == 0)
        return TAMANHO_DE_NAVIO_NAO_DISPONIVEL;

    if(buffer[2] == 'H' && coluna + tamanho > 11)
        return NAVIO_FORA_DO_TABULEIRO;
    if(buffer[2] == 'V' && linha + tamanho > 11)
        return NAVIO_FORA_DO_TABULEIRO;

    int flag_valido = 1;
    for(int i = 0; i<tamanho; i++){

        if(cliente == partida->jogador1 && buffer[2] == 'H' && partida->tabuleiro_naviosJ1[linha][coluna+i] != ' ')
            flag_valido = 0;

        if(cliente == partida->jogador2 && buffer[2] == 'H' && partida->tabuleiro_naviosJ2[linha][coluna+i] != ' ')
            flag_valido = 0;

            
        if(cliente == partida->jogador1 && buffer[2] == 'V' && partida->tabuleiro_naviosJ1[linha+i][coluna] != ' ')
            flag_valido = 0;

        if(cliente == partida->jogador2 && buffer[2] == 'V' && partida->tabuleiro_naviosJ2[linha+i][coluna] != ' ')
            flag_valido = 0;

    }

    if(flag_valido == 0)
        return NAVIO_COLIDINDO;

    for(int i = 0; i<tamanho; i++){

        if(cliente == partida->jogador1 && buffer[2] == 'H')
            partida->tabuleiro_naviosJ1[linha][coluna+i] = 'O';

        if(cliente == partida->jogador2 && buffer[2] == 'H')
            partida->tabuleiro_naviosJ2[linha][coluna+i] = 'O';

            
        if(cliente == partida->jogador1 && buffer[2] == 'V')
            partida->tabuleiro_naviosJ1[linha+i][coluna] = 'O';

        if(cliente == partida->jogador2 && buffer[2] == 'V')
            partida->tabuleiro_naviosJ2[linha+i][coluna] = 'O';

    }

    if(cliente == partida->jogador1)
        partida->navios_disponiveisJ1[tamanho]--;

    if(cliente == partida->jogador2)
        partida->navios_disponiveisJ2[tamanho]--;


    int flag_navios_todos_posicionados = 1;
    for(int i = 0; i<TAM_MAXIMO_NAVIO; i++){
        
        if(cliente == partida->jogador1 && partida->navios_disponiveisJ1[i] != 0)
            flag_navios_todos_posicionados = 0;

        if(cliente == partida->jogador2 && partida->navios_disponiveisJ2[i] != 0)
            flag_navios_todos_posicionados = 0;

    }

    if(flag_navios_todos_posicionados)
        return NAVIOS_TODOS_POSICIONADOS;


    return SUCESSO;

}

int validar_ataque(char buffer[], CLIENTE *cliente){

    if(buffer[0] < 'A' || buffer[0] > 'J')
        return COLUNA_INVALIDA;

    if(buffer[1] < '0' || buffer[1] > '9')
        return LINHA_INVALIDA;

    if(buffer[2] != '\0' && buffer[2] != '\n')
        return FORMATACAO_ERRADA;

    JOGO *partida = cliente->partida;

    int linha = buffer[1]-'0'+1;
    int coluna = buffer[0] - 'A'+1;

    if(cliente == partida->jogador1 && partida->tabuleiro_tirosJ1[linha][coluna] == 'X')
        return CASA_JA_ATACADA;

    else if(cliente == partida->jogador2 && partida->tabuleiro_tirosJ2[linha][coluna] == 'X')
        return CASA_JA_ATACADA;


    if(cliente == partida->jogador1){

        if(partida->tabuleiro_naviosJ2[linha][coluna] == 'O'){
            partida->pontos_restantesJ2--;
            partida->tabuleiro_tirosJ1[linha][coluna] = 'O';

            if(partida->pontos_restantesJ2 == 0){
                partida->jogador1->estado = VITORIA;
                terminal_vitoria(partida, partida->jogador1);

                partida->jogador2->estado = DERROTA;
                terminal_derrota(partida, partida->jogador2);
                return PARTIDA_FINALIZADA;
            }

        }
        else{
            partida->tabuleiro_tirosJ1[linha][coluna] = 'X';
        }

        partida->tabuleiro_naviosJ2[linha][coluna] = 'X';
    }

    if(cliente == partida->jogador2){

        if(partida->tabuleiro_naviosJ1[linha][coluna] == 'O'){
            partida->pontos_restantesJ1--;
            partida->tabuleiro_tirosJ2[linha][coluna] = 'O';

            if(partida->pontos_restantesJ1 == 0){
                partida->jogador2->estado = VITORIA;
                terminal_vitoria(partida, partida->jogador2);

                partida->jogador1->estado = DERROTA;
                terminal_derrota(partida, partida->jogador1);
                return PARTIDA_FINALIZADA;
            }

        }
        else{

            partida->tabuleiro_tirosJ2[linha][coluna] = 'X';
        }

        partida->tabuleiro_naviosJ1[linha][coluna] = 'X';
    }

    return SUCESSO;

}

int validar_busca_partida(char buffer[], CLIENTE *cliente){

    if(strcmp(buffer, "BUSCAR") == 0 || strcmp(buffer, "BUSCAR\n") == 0)
        return SUCESSO;
    
    return FORMATACAO_ERRADA;

}

void aguardar_adversario_posicionar(CLIENTE *cliente){

    JOGO *partida = cliente->partida;

    if(cliente == partida->jogador1 && partida->jogador2->estado == POSICIONANDO_NAVIOS)
        cliente->estado = AGUARDANDO_ADVERSARIO_POSICIONAR;

    else if(cliente == partida->jogador2 && partida->jogador1->estado == POSICIONANDO_NAVIOS)
        cliente->estado = AGUARDANDO_ADVERSARIO_POSICIONAR;

    else if(cliente == partida->jogador1 && partida->jogador2->estado == AGUARDANDO_ADVERSARIO_POSICIONAR){
        partida->jogador1->estado = EM_TURNO;
        partida->jogador2->estado = ESPERANDO_TURNO;
        terminal_em_turno(partida, partida->jogador1);
        terminal_esperando_turno(partida, partida->jogador2);
    }

    else if(cliente == partida->jogador2 && partida->jogador1->estado == AGUARDANDO_ADVERSARIO_POSICIONAR){
        partida->jogador2->estado = EM_TURNO;
        partida->jogador1->estado = ESPERANDO_TURNO;
        terminal_esperando_turno(partida, partida->jogador1);
        terminal_em_turno(partida, partida->jogador2);
    }

}