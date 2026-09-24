#include "includes/consts.h"

// Variavel global que mantera o socket do servidor a qual o cliente esta conectado
int socket_servidor;

// Funcao que tera a thread de receber mensagens
void *receber_mensagens(void *arg) {
    // Define o buffer recebido e a qtd de bytes
    char buffer[TAM_BUFFER];
    int bytes_recebidos;

    // Loop infinito
    while (1) {
        // Espera receber uma mensagem do servidor
        bytes_recebidos = recv(socket_servidor, buffer, TAM_BUFFER - 1, 0);

        // Caso retorne valor menor que zero, significa que o servidor foi encerrado. Printa isso no terminal e encerra o programa
        if (bytes_recebidos <= 0) {
            printf("Servidor desconectado.\n");
            exit(0);
        }

        // Garante que a mensagem tenha o '\0' no final
        buffer[bytes_recebidos] = '\0';

        // Limpa o terminal com as mensagens antigas
        system("clear");

        // Printa no terminal a mensagem nova recebida, dando um flush depois
        printf("%s", buffer);
        fflush(stdout);
    }

    // Encerra a thread caso saia do loop
    // Igual ao main, nunca chega aqui pois nao existe uma rotina de encerramento
    return NULL;
}

// Funcao main, principal do codigo
int main(int argc, char *argv[]) {

    // Define algumas variaveis importantes para o funcionamento do cliente
    struct sockaddr_in servidor;
    int porta = PORTA_PADRAO;
    char buffer[TAM_BUFFER];
    pthread_t thread;

    // Define o socket no proprio dispositivo do cliente
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);

    // Caso retorne menor que zero, retorna erro no socket e encerra o programa
    if (socket_servidor < 0) {
        perror("Erro ao criar socket");
        return 1;
    }

    // Define que o servidor sera do tipo ipv4
    servidor.sin_family = AF_INET;

    // Caso receba algum argumento e o primeiro argumento nao seja zero, define a porta como esse valor
    if(argc > 1 && atoi(argv[1]) != 0)
        porta = atoi(argv[1]);

    // Define a porta do servidor
    servidor.sin_port = htons(porta);

    // Caso receba menos de 2 argumentos, utiliza o IP local
    if(argc < 3)
        servidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Caso contrario, utiliza o segundo argumento como o endereco de IP para conectar no servidor
    else
        servidor.sin_addr.s_addr = inet_addr(argv[2]);

    // Tenta realizar o connect. Caso retorne menor que zero, deu erro no connect, fecha o socket e encerra o programa.
    if (connect(socket_servidor, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) {
        perror("Erro ao conectar");
        close(socket_servidor);
        return 1;
    }

    // Limpa o terminal do cliente e printa a mensagem de que foi conectado no servidor, pedindo o nome do cliente
    system("clear");
    printf("Conectado ao servidor.\nDigite seu nome: ");

    // Cria a thread que ficara recebendo mensagens, enquanto a thread principal fica responsavel por enviar as mensagens
    pthread_create(&thread, NULL, receber_mensagens, NULL);

    // Loop infinito da thread principal
    while (1) {
        // Espera o cliente digitar algo no terminal
        fgets(buffer, TAM_BUFFER, stdin);

        // Apos o cliente digitar, envia o texto digitado para o servidor
        send(socket_servidor, buffer, strlen(buffer), 0);
    }

    // O cliente nao possui uma rotina de encerramente, ficando ativo indefinidamente
    // Portanto esse close(socket_servidor) nunca é de fato alcançado
    // Está aqui apenas por motivos didaticos (caso em algum momento seja implementado uma rotina de encerramento)
    close(socket_servidor);

    return 0;
}