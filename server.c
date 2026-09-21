#include "includes/func.h"
#include "includes/struct_cliente.h"
#include "includes/struct_jogo.h"

#define PORTA 8080
#define MAX_CLIENTES 4

LISTA_CLIENTES *clientes;
LISTA_JOGOS *jogos;
int quantidade_clientes = 0;
int quantidade_jogos = 0;

extern int numero_navios[TAM_MAXIMO_NAVIO];
extern int qtd_pontos;

CLIENTE *cliente_esperando;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void atualizar_partida(char *texto, int bytes_recebidos, CLIENTE *remetente) {

    char buffer[TAM_BUFFER+70];

    pthread_mutex_lock(&mutex);

    if(remetente->estado == ESPERANDO_PARTIDA && (cliente_esperando == NULL || cliente_esperando == remetente)){
        cliente_esperando = remetente;
        sprintf(buffer, "AGUARDANDO ENCONTRAR UMA PARTIDA...\n");
        send(remetente->socket, buffer, strlen(buffer), 0);
    }

    else if(remetente->estado == ESPERANDO_PARTIDA && cliente_esperando != NULL && cliente_esperando != remetente){

        JOGO *partida = adiciona_jogo(cliente_esperando, remetente, jogos);
        preparar_tabuleiro(partida);
        quantidade_jogos++;
        cliente_esperando = NULL;

        terminal_posicionando_navios(partida, partida->jogador1);
        terminal_posicionando_navios(partida, partida->jogador2);

    }

    else if(remetente->estado == POSICIONANDO_NAVIOS){
        terminal_posicionando_navios(remetente->partida, remetente);
    }

    else if(remetente->estado == AGUARDANDO_ADVERSARIO_POSICIONAR){
        terminal_aguardando_adversario_posicionar(remetente->partida, remetente);
    }

    else if(remetente->estado == EM_TURNO){
        terminal_em_turno(remetente->partida, remetente);
    }

    else if(remetente->estado == ESPERANDO_TURNO){
        terminal_esperando_turno(remetente->partida, remetente);
    }

    pthread_mutex_unlock(&mutex);

}

void remover_cliente(CLIENTE *cliente) {
    pthread_mutex_lock(&mutex);

    deleta_cliente(cliente, clientes);
    quantidade_clientes--;

    pthread_mutex_unlock(&mutex);
}

void *atender_cliente(void *arg) {
    CLIENTE *cliente = (CLIENTE *)arg;

    int socket_cliente = cliente->socket;

    char buffer[TAM_BUFFER];
    int bytes_recebidos;

    printf("Cliente conectado: %d\n", socket_cliente);

    while (1) {
        bytes_recebidos = recv(socket_cliente, buffer, TAM_BUFFER - 1, 0);

        if (bytes_recebidos <= 0) {
            break;
        }

        buffer[bytes_recebidos] = '\0';

        if(cliente->cadastrado == 0){
            pthread_mutex_lock(&mutex);
            buffer[bytes_recebidos-1] = '\0';
            salvar_nome(buffer, cliente);
            buffer[bytes_recebidos-1] = '\n';
            pthread_mutex_unlock(&mutex);
        }

        if(cliente->estado == POSICIONANDO_NAVIOS){

            pthread_mutex_lock(&mutex);

            int validade = validar_posicionamento(buffer, cliente);

            printf("%d ", validade);

            if(validade == RESETAR_TABULEIRO)
                resetar_tabuleiro(cliente);

            else if(validade == NAVIOS_TODOS_POSICIONADOS)
                aguardar_adversario_posicionar(cliente);

            pthread_mutex_unlock(&mutex);

        }

        if(cliente->estado == EM_TURNO){

            pthread_mutex_lock(&mutex);

            int validade = validar_ataque(buffer, cliente);

            if(validade == SUCESSO && cliente->partida->jogador1 == cliente){
                cliente->partida->jogador2->estado = EM_TURNO;
                cliente->estado = ESPERANDO_TURNO;
                terminal_em_turno(cliente->partida, cliente->partida->jogador2);
            }
            
            else if(validade == SUCESSO && cliente->partida->jogador2 == cliente){
                cliente->partida->jogador1->estado = EM_TURNO;
                cliente->estado = ESPERANDO_TURNO;
                terminal_em_turno(cliente->partida, cliente->partida->jogador1);
            }

            pthread_mutex_unlock(&mutex);
        }

        atualizar_partida(buffer, bytes_recebidos, cliente);

        printf("Cliente %d: %s", socket_cliente, buffer);

    }

    printf("Cliente desconectado: %d\n", socket_cliente);

    remover_cliente(cliente);
    close(socket_cliente);

    return NULL;
}

int main() {
    int servidor;
    int cliente;
    CLIENTE *novo_cliente;
    struct sockaddr_in endereco;
    socklen_t tamanho_endereco = sizeof(endereco);
    int opt = 1;

    qtd_pontos = 0;
    for(int i = 0; i<TAM_MAXIMO_NAVIO; i++){
        qtd_pontos += i * numero_navios[i];
    }

    clientes = cria_lista_clientes();
    jogos = cria_lista_jogos();

    cliente_esperando = NULL;

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

        novo_cliente = adiciona_cliente(cliente, clientes);
        quantidade_clientes++;

        pthread_mutex_unlock(&mutex);

        pthread_t thread;
        pthread_create(&thread, NULL, atender_cliente, novo_cliente);
        pthread_detach(thread);

    }

    close(servidor);

    return 0;
}