#include "../includes/recv_send.h"

// Funcao que garante que o recv recebeu toda a mensagem
int enviar_tudo(int socket, const char buf[]){
    // Cria as variaveis que guarda o total ja enviado e o tamanho total
    int total = 0;
    int tamanho = strlen(buf) + 1; // inclui o '\0'

    // Enquanto a mensagem nao for enviada por inteiro
    while (total < tamanho){
        // Tenta enviar toda a mensagem que ainda nao foi enviada
        int n = send(socket, buf + total, tamanho - total, 0);

        // Caso retorne menor que 1, significa que houve erro
        if (n <= 0)
            return -1;

        // Adiciona ao total o tanto de bytes enviados
        total += n;
    }

    // Retorna o total de bytes enviados
    return total;
}

// Funcao que garante que o send enviou toda a mensagem
int receber_tudo(int socket, char buf[]){
    // Cria a variavel que guarda o total ja recebido
    int total = 0;

    // Enquanto a mensagem for menor que o tamanho maximo do buffer
    while (total < TAM_BUFFER){
        // Recebe uma quantidade n de bytes
        int n = recv(socket, buf + total, TAM_BUFFER - total, 0);

        // Caso retorne um valor negativo, significa que houve erro
        if (n <= 0)
            return -1;

        // Procura o '\0' nos bytes recebidos
        for (int i = 0; i < n; i++){
            // Caso encontre o '\0', retorna a quantidade de bytes que deu total de mensagem
            if (buf[total + i] == '\0')
                return total + i + 1;
        }

        // Adiciona o valor recebido ao total
        total += n;
    }

    // Retorna negativo caso a mensagem ultrapasse o tamanho maximo do buffer
    return -1;
}