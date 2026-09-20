#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define EM_JOGO -1
#define VITORIA_1 1
#define VITORIA_2 2
#define EMPATE 0

#define PORTA 8080
#define MAX_CLIENTES 2
#define TAM_BUFFER 1024

typedef struct{
    int jogador1;
    int jogador2;

    char tabuleiro[3][3];

    int turno;

    int ESTADO;

} JOGO;

int clientes[MAX_CLIENTES];
int quantidade_clientes = 0;

JOGO partida;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char *texto_tabuleiro(char *msg){
    char *texto = (char *)malloc(1024);

    sprintf(texto, " %c | %c | %c \n---+---+---\n %c | %c | %c \n---+---+---\n %c | %c | %c \n\n%s\n",
            partida.tabuleiro[0][0], partida.tabuleiro[0][1], partida.tabuleiro[0][2],
            partida.tabuleiro[1][0], partida.tabuleiro[1][1], partida.tabuleiro[1][2],
            partida.tabuleiro[2][0], partida.tabuleiro[2][1], partida.tabuleiro[2][2], msg);

    return texto;
}

void enviar_todos() {

    pthread_mutex_lock(&mutex);

    if(partida.ESTADO == EM_JOGO){

        char *texto_seuturno = texto_tabuleiro("Seu turno");
        char *texto_turnoadv = texto_tabuleiro("Turno do adversario");

        for (int i = 0; i < quantidade_clientes; i++) {
            if (clientes[i] == partida.turno) {
                send(clientes[i], texto_seuturno, strlen(texto_seuturno), 0);
            }

            else{
                send(clientes[i], texto_turnoadv, strlen(texto_turnoadv), 0);
            }

        }

        free(texto_seuturno);
        free(texto_turnoadv);
    }

    else if(partida.ESTADO == VITORIA_1){
        char *texto_vitoria = texto_tabuleiro("Voce venceu!! PARABENS");
        char *texto_derrota = texto_tabuleiro("Voce perdeu! Tente a sorte na proxima");

        for (int i = 0; i < quantidade_clientes; i++) {
            if (clientes[i] == partida.jogador1) {
                send(clientes[i], texto_vitoria, strlen(texto_vitoria), 0);
            }

            else{
                send(clientes[i], texto_derrota, strlen(texto_derrota), 0);
            }

        }

        free(texto_vitoria);
        free(texto_derrota);

    }

    else if(partida.ESTADO == VITORIA_2){
        char *texto_vitoria = texto_tabuleiro("Voce venceu!! PARABENS");
        char *texto_derrota = texto_tabuleiro("Voce perdeu! Tente a sorte na proxima");

        for (int i = 0; i < quantidade_clientes; i++) {
            if (clientes[i] == partida.jogador2) {
                send(clientes[i], texto_vitoria, strlen(texto_vitoria), 0);
            }

            else{
                send(clientes[i], texto_derrota, strlen(texto_derrota), 0);
            }

        }

        free(texto_vitoria);
        free(texto_derrota);

    }

    else if(partida.ESTADO == EMPATE){
        char *texto_empate = texto_tabuleiro("A partida empatou!! Tenta outra vez");

        for (int i = 0; i < quantidade_clientes; i++) {
            send(clientes[i], texto_empate, strlen(texto_empate), 0);
        }

        free(texto_empate);

    }

    pthread_mutex_unlock(&mutex);
}

int verificar_vitoria(){

    for(int i = 0; i<3; i++){
        if(partida.tabuleiro[i][0] == ' ') continue;
        if(partida.tabuleiro[i][0] == 'X' && partida.tabuleiro[i][1] == 'X' && partida.tabuleiro[i][2] == 'X') return 1;
        if(partida.tabuleiro[i][0] == 'O' && partida.tabuleiro[i][1] == 'O' && partida.tabuleiro[i][2] == 'O') return 2;
    }

    for(int i = 0; i<3; i++){
        if(partida.tabuleiro[0][i] == ' ') continue;
        if(partida.tabuleiro[0][i] == 'X' && partida.tabuleiro[1][i] == 'X' && partida.tabuleiro[2][i] == 'X') return 1;
        if(partida.tabuleiro[0][i] == 'O' && partida.tabuleiro[1][i] == 'O' && partida.tabuleiro[2][i] == 'O') return 2;
    }

    if(partida.tabuleiro[0][0] != ' '){
        if(partida.tabuleiro[0][0] == 'X' && partida.tabuleiro[1][1] == 'X' && partida.tabuleiro[2][2] == 'X') return 1;
        if(partida.tabuleiro[0][0] == 'O' && partida.tabuleiro[1][1] == 'O' && partida.tabuleiro[2][2] == 'O') return 2;
    }

    if(partida.tabuleiro[0][2] != ' '){
        if(partida.tabuleiro[0][2] == 'X' && partida.tabuleiro[1][1] == 'X' && partida.tabuleiro[2][0] == 'X') return 1;
        if(partida.tabuleiro[0][2] == 'O' && partida.tabuleiro[1][1] == 'O' && partida.tabuleiro[2][0] == 'O') return 2;
    }

    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            if(partida.tabuleiro[i][j] == ' ') return -1;
        }
    }

    return 0;

}

void avaliar_jogada(char buffer[TAM_BUFFER], int socket_cliente){

    if((buffer[0] != '1' && buffer[0] != '2' && buffer[0] != '3') || 
        (buffer[2] != '1' && buffer[2] != '2' && buffer[2] != '3') ||
        (buffer[1] != ' ') ||
        (buffer[4] != '\0' && buffer[4] != '\n')){
        char *texto = texto_tabuleiro("Jogada invalida! A jogada dever ser no formato \"N N\"");
        send(socket_cliente, texto, strlen(texto), 0);
        free(texto);
        return;
    }

    int lin = (int)buffer[0] - 49;
    int col = (int)buffer[2] - 49;

    if(partida.tabuleiro[lin][col] != ' '){
        char *texto = texto_tabuleiro("Jogada invalida! A casa ja esta ocupada");
        send(socket_cliente, texto, strlen(texto), 0);
        free(texto);
        return;
    }

    if(socket_cliente == partida.jogador1){
        partida.tabuleiro[lin][col] = 'X';
        partida.turno = partida.jogador2;
        
    }
    else if(socket_cliente == partida.jogador2){
        partida.tabuleiro[lin][col] = 'O';
        partida.turno = partida.jogador1;
    }

    partida.ESTADO = verificar_vitoria();

    enviar_todos();
    return;

}

void remover_cliente(int socket) {
    pthread_mutex_lock(&mutex);

    for (int i = 0; i < quantidade_clientes; i++) {
        if (clientes[i] == socket) {
            clientes[i] = clientes[quantidade_clientes - 1];
            quantidade_clientes--;
            break;
        }
    }

    pthread_mutex_unlock(&mutex);
}

void *atender_cliente(void *arg) {
    int socket_cliente = *(int *)arg;
    free(arg);

    char buffer[TAM_BUFFER];
    int bytes_recebidos;

    printf("Cliente conectado: %d\n", socket_cliente);

    while (1) {
        bytes_recebidos = recv(socket_cliente, buffer, TAM_BUFFER - 1, 0);

        if (bytes_recebidos <= 0) {
            break;
        }

        buffer[bytes_recebidos] = '\0';

        printf("Cliente %d: %s", socket_cliente, buffer);

        if(partida.ESTADO != EM_JOGO){
            enviar_todos();
        }

        else if(partida.turno != socket_cliente){
            char *texto = texto_tabuleiro("Turno do adversario");
            send(socket_cliente, texto, strlen(texto), 0);
            free(texto);
        }

        else{
            avaliar_jogada(buffer, socket_cliente);
        }

    }

    printf("Cliente desconectado: %d\n", socket_cliente);

    remover_cliente(socket_cliente);
    close(socket_cliente);

    return NULL;
}

void atualizar_partida(){

    if(quantidade_clientes == 1){
        char texto[30] = "Aguardando jogador...\n";
        send(clientes[0], texto, strlen(texto), 0);
        return;
    }

    else{
        char texto[30] = "Partida encontrada!\n";
        send(clientes[0], texto, strlen(texto), 0);
        send(clientes[1], texto, strlen(texto), 0);

        for(int i = 0; i<3; i++){
            for(int j = 0; j<3; j++){
                partida.tabuleiro[i][j] = ' ';
            }
        }

        partida.jogador1 = clientes[0];
        partida.jogador2 = clientes[1];

        srand(time(NULL));
        int vez = rand()%2;
        
        if(vez == 0) partida.turno = partida.jogador1;
        else partida.turno = partida.jogador2;

        enviar_todos();

    }

}

int main() {
    int servidor;
    int cliente;
    int *novo_cliente;
    struct sockaddr_in endereco;
    socklen_t tamanho_endereco = sizeof(endereco);
    int opt = 1;

    partida.ESTADO = EM_JOGO;

    servidor = socket(AF_INET, SOCK_STREAM, 0);

    if (servidor < 0) {
        perror("Erro ao criar socket");
        return 1;
    }

    if (setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Erro no setsockopt");
        close(servidor);
        return 1;
    }

    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(PORTA);

    if (bind(servidor, (struct sockaddr *)&endereco, sizeof(endereco)) < 0) {
        perror("Erro no bind");
        close(servidor);
        return 1;
    }

    if (listen(servidor, MAX_CLIENTES) < 0) {
        perror("Erro no listen");
        close(servidor);
        return 1;
    }

    printf("Servidor iniciado na porta %d...\n", PORTA);

    while (1) {
        cliente = accept(servidor, (struct sockaddr *)&endereco, &tamanho_endereco);

        if (cliente < 0) {
            perror("Erro no accept");
            continue;
        }

        pthread_mutex_lock(&mutex);

        if (quantidade_clientes >= MAX_CLIENTES) {
            pthread_mutex_unlock(&mutex);
            send(cliente, "Servidor cheio.\n", 16, 0);
            close(cliente);
            continue;
        }

        clientes[quantidade_clientes] = cliente;
        quantidade_clientes++;

        pthread_mutex_unlock(&mutex);

        novo_cliente = malloc(sizeof(int));
        *novo_cliente = cliente;

        pthread_t thread;
        pthread_create(&thread, NULL, atender_cliente, novo_cliente);
        pthread_detach(thread);

        atualizar_partida();

    }

    close(servidor);

    return 0;
}