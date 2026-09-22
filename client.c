#include "includes/funcoes.h"

int socket_servidor;

void *receber_mensagens(void *arg) {
    char buffer[TAM_BUFFER];
    int bytes_recebidos;

    while (1) {
        bytes_recebidos = recv(socket_servidor, buffer, TAM_BUFFER - 1, 0);

        if (bytes_recebidos <= 0) {
            printf("Servidor desconectado.\n");
            exit(0);
        }

        buffer[bytes_recebidos] = '\0';

        system("clear");

        printf("%s", buffer);
        fflush(stdout);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in servidor;
    int porta = PORTA_PADRAO;
    char buffer[TAM_BUFFER];
    pthread_t thread;

    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_servidor < 0) {
        perror("Erro ao criar socket");
        return 1;
    }

    servidor.sin_family = AF_INET;

    if(argc > 1 && atoi(argv[1]) != 0)
        porta = atoi(argv[1]);

    servidor.sin_port = htons(porta);

    if(argc < 3)
        servidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    else
        servidor.sin_addr.s_addr = inet_addr(argv[2]);

    if (connect(socket_servidor, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) {
        perror("Erro ao conectar");
        close(socket_servidor);
        return 1;
    }

    system("clear");
    printf("Conectado ao servidor.\nDigite seu nome: ");

    pthread_create(&thread, NULL, receber_mensagens, NULL);

    while (1) {
        fgets(buffer, TAM_BUFFER, stdin);

        send(socket_servidor, buffer, strlen(buffer), 0);
    }

    close(socket_servidor);

    return 0;
}