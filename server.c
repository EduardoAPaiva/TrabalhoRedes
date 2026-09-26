#include "includes/funcoes.h"
#include "includes/struct_cliente.h"
#include "includes/struct_jogo.h"
#include "includes/terminal.h"
#include "includes/consts.h"

// Lista encadeada com os clientes conectados
LISTA_CLIENTES *clientes;
int quantidade_clientes = 0;

// Cliente aguardando para entrar numa partida
CLIENTE *cliente_esperando;

// Lista encadeada com os jogos existentes no momento
LISTA_JOGOS *jogos;
int quantidade_jogos = 0;
int id_atual_partida = 1;

// Constante com as opcoes de tamanhos de navios existentes
extern int numero_navios[TAM_MAXIMO_NAVIO];
extern int qtd_pontos;

// Cria o mutex para evitar acessos simultaneos das threads
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Funcao que atualiza o status de uma partida a partir de uma requisicao de um cliente
void atualizar_partida(char *texto, int bytes_recebidos, CLIENTE *remetente) {

    //Cria um buffer intermediario
    char buffer[TAM_BUFFER];

    // Bloqueia o mutex
    pthread_mutex_lock(&mutex);

    // Caso o remetente esteja esperando uma partida e nao tenha ninguem esperando ou seja ele mesmo na lista
    if(remetente->estado == ESPERANDO_PARTIDA && (cliente_esperando == NULL || cliente_esperando == remetente)){
        cliente_esperando = remetente;                                  // Adiciona o remetente da mensagem na fila de cliente esperando
        sprintf(buffer, "AGUARDANDO ENCONTRAR UMA PARTIDA...\n");       // Cria a mensagem de texto
        send(remetente->socket, buffer, strlen(buffer), 0);             // Atualiza o terminal do cliente
    }

    // Caso o remetente esteja esperando uma partida e ja tenha outro esperando
    else if(remetente->estado == ESPERANDO_PARTIDA && cliente_esperando != NULL && cliente_esperando != remetente){

        // Cria uma nova partida e add na lista de jogos. Atualiza os dados relevantes da partida, como id e incremente o id_atual_partida
        JOGO *partida = adiciona_jogo(cliente_esperando, remetente, jogos);
        partida->id = id_atual_partida;
        id_atual_partida++;
        quantidade_jogos++;

        // Printa no terminal do servidor que uma partida foi criada
        printf("PARTIDA DE ID = %d CRIADA COM OS JOGADORES [%s] e [%s]\n", partida->id, remetente->nickname, cliente_esperando->nickname);

        // Prepara o tabuleiro da partida, preenchendo com os devidos caracteres
        preparar_tabuleiro(partida);

        // Remove o cliente que estava esperando da lista
        cliente_esperando = NULL;

        // Atualiza o terminal de ambos os jogadores para o estado de  posicionando navios
        terminal_posicionando_navios(partida, partida->jogador1);
        terminal_posicionando_navios(partida, partida->jogador2);

    }

    // Caso o remetente esteja no estado de posicionando navios, atualiza o terminal
    else if(remetente->estado == POSICIONANDO_NAVIOS){
        terminal_posicionando_navios(remetente->partida, remetente);
    }
    // Caso o remetente esteja no estado de aguardando adversario terminar de posicionar, atualiza terminal
    else if(remetente->estado == AGUARDANDO_ADVERSARIO_POSICIONAR){
        terminal_aguardando_adversario_posicionar(remetente->partida, remetente);
    }
    // Caso o remetente esteja no turno pra jogar, atualiza o terminal
    else if(remetente->estado == EM_TURNO){
        terminal_em_turno(remetente->partida, remetente);
    }
    // Caso o remetente esteja esperando o turno do adversario, atualiza o terminal
    else if(remetente->estado == ESPERANDO_TURNO){
        terminal_esperando_turno(remetente->partida, remetente);
    }
    // Caso o remetente esteja no estado de vitoria, atualiza o terminal
    else if(remetente->estado == VITORIA){
        terminal_vitoria(remetente->partida, remetente);
    }
    // Caso o remetente esteja no estado de derrota, atualiza o terminal
    else if(remetente->estado == DERROTA){
        terminal_derrota(remetente->partida, remetente);
    }

    // Libera o mutex
    pthread_mutex_unlock(&mutex);

}

// Funcao que remove um cliente da lista de clientes
void remover_cliente(CLIENTE *cliente) {

    // Bloqueia o mutex
    pthread_mutex_lock(&mutex);

    // Obtem a partida do cliente, e tambem define um jogador, inicializado como NULL
    JOGO *partida = cliente->partida;
    CLIENTE *jogador = NULL;

    // Caso o cliente esteja numa partida que nao tenha sido finalizada ainda
    if(partida != NULL && cliente->estado != VITORIA && cliente->estado != DERROTA){

        // Caso o cliente seja o jogador 1 e o jogador2 ainda estiver conectado
        if(cliente == partida->jogador1 && partida->jogador2 != NULL){
            // Remove o jogador2 da partida e define a variavel jogador sendo o jogador2
            partida->jogador2->estado = ESPERANDO_PARTIDA;
            partida->jogador2->partida = NULL;
            jogador = partida->jogador2;
        }
        // Caso o cliente seja o jogador 2 e o jogador 1 ainda estiver conectado
        if(cliente == partida->jogador2 && partida->jogador1 != NULL){
            // Remove o jogador1 da partida e define a variavel jogador sendo o jogador1
            partida->jogador1->estado = ESPERANDO_PARTIDA;
            partida->jogador1->partida = NULL;
            jogador = partida->jogador1;
        }

        // Printa no terminal do servidor que a partida que o cliente que desconectou estava foi encerrada e excluida
        printf("PARTIDA DE ID = %d FINALIZADA E EXCLUIDA POIS UM DOS JOGADORES DESCONECTOU\n", partida->id);
        // Deleta de fato a partida e decrementa o contador de quantidade de jogos existentes
        deleta_jogo(partida, jogos);
        quantidade_jogos--;

    }

    // Caso o cliente que desconectou estivesse em uma partida ja finalizada
    else if(partida != NULL){

        // Caso cliente seja o jogador1 e o jogador2 ainda esteja conectado
        if(cliente == partida->jogador1 && partida->jogador2 != NULL)
            // Apenas remove o jogador1 (cliente) da partida, mantendo a partida existindo ainda
            partida->jogador1 = NULL;
        // Caso o cliente seja o jogador2 e o jogador1 ainda esteja conectado
        else if(cliente == partida->jogador2 && partida->jogador2 != NULL)
            // Apenas remove o jogador2 (cliente) da partida, mantendo a partida existindo ainda
            partida->jogador2 = NULL;
        // Caso o cliente esteja numa partida onde apenas ele estava conectado
        else{
            //Printa no terminal, encerrando e excluindo a partida do cliente e decrementando a quantidade de partidas existentes
            printf("PARTIDA DE ID = %d FINALIZADA E EXCLUIDA POIS AMBOS JOGADORES DESCONECTARAM\n", partida->id);
            deleta_jogo(partida, jogos);
            quantidade_jogos--;
        }

    }

    // Deleta o cliente da lista de clientes, decrementando o contador
    deleta_cliente(cliente, clientes);
    quantidade_clientes--;

    // Libera o mutex
    pthread_mutex_unlock(&mutex);

    // Caso a variavel jogador tenha sido atualizado para algum jogador existente
    if(jogador != NULL)
        // Atualiza a partida para que o terminal do jogador afetado deja atualizado
        atualizar_partida(" ", 2, jogador);

}

// Funcao que tera a thread de atender cada cliente
void *atender_cliente(void *arg) {
    // Faz o cast para transformar o ponteiro de void em um ponteiro para CLIENTE
    CLIENTE *cliente = (CLIENTE *)arg;

    // Obtem o socket do cliente a partir da struct
    int socket_cliente = cliente->socket;

    // Define o buffer que recebera as mensagens e o tanto de bytes recebidos
    char buffer[TAM_BUFFER];
    int bytes_recebidos;

    // Printa no terminal do servidor que o cliente foi conectado
    printf("Cliente conectado: %d\n", socket_cliente);

    // Loop que espera uma mensagem do cliente (jogador) 
    while (1) {
        // Espera o cliente enviar uma mensagem
        bytes_recebidos = recv(socket_cliente, buffer, TAM_BUFFER - 1, 0);

        // Caso nao receba bytes ou retorne um valor negativo, significa que o cliente desconectou, saindo do loop
        if (bytes_recebidos <= 0) {
            break;
        }

        // Garante que a mensagem termine com um \0
        buffer[bytes_recebidos] = '\0';

        // Caso o cliente ainda nao tenha sido cadastrado (colocou o nome)
        if(cliente->cadastrado == 0){
            // Bloqueia o mutex, substitui o \n por um \0, salva o nome no cliente, retorna o \n para o buffer e libera o mutex novamente
            pthread_mutex_lock(&mutex);
            buffer[bytes_recebidos-1] = '\0';
            salvar_nome(buffer, cliente);
            buffer[bytes_recebidos-1] = '\n';
            pthread_mutex_unlock(&mutex);
        }

        // Caso o cliente esteja no estado de posicionando navios
        if(cliente->estado == POSICIONANDO_NAVIOS){

            // Bloqueia o mutex
            pthread_mutex_lock(&mutex);

            // Analisa se o comando recebido é valido para o estado que a partida e o tabuleiro se encontram
            int validade = validar_posicionamento(buffer, cliente);

            // Caso receba que o comando enviado foi para resetar o tabuleiro, chama a funcao que faz isso
            if(validade == RESETAR_TABULEIRO)
                resetar_tabuleiro(cliente);
            // Caso perceba que o comando posicionou o ultimo navio, atualiza para o estado de aguardando adversario posicionar
            else if(validade == NAVIOS_TODOS_POSICIONADOS)
                aguardar_adversario_posicionar(cliente);

            // Libera o mutex
            pthread_mutex_unlock(&mutex);

        }
        // Caso o cliente esteja no turno para jogar
        if(cliente->estado == EM_TURNO){

            // Bloqueia o mutex
            pthread_mutex_lock(&mutex);

            // Analisa se o comando recebido para ataque é valido para o estado que a partida e o tabuleiro se encontram
            int validade = validar_ataque(buffer, cliente);

            // Caso o ataque seja valido e o cliente seja o jogador1
            if(validade == SUCESSO && cliente->partida->jogador1 == cliente){
                // Inverte os estados de turno para ambos jogadores, e atualiza o terminal do adversario
                cliente->partida->jogador2->estado = EM_TURNO;
                cliente->estado = ESPERANDO_TURNO;
                terminal_em_turno(cliente->partida, cliente->partida->jogador2);
            }
            // Caso o ataque seja valido e o cliente seja o jogador2
            else if(validade == SUCESSO && cliente->partida->jogador2 == cliente){
                // Inverte os estados de turno para ambos jogadores, e atualiza o terminal do adversario
                cliente->partida->jogador1->estado = EM_TURNO;
                cliente->estado = ESPERANDO_TURNO;
                terminal_em_turno(cliente->partida, cliente->partida->jogador1);
            }

            // Libera o mutex
            pthread_mutex_unlock(&mutex);
        }
        // Caso o cliente esteja no estado de vitoria ou derrota (partida finalizada)
        if(cliente->estado == VITORIA || cliente->estado == DERROTA){

            // Bloqueia o mutex
            pthread_mutex_lock(&mutex);

            // Valida se o comando recebido foi valido para buscar nova partida. Comando correto seria "BUSCAR"
            int validade = validar_busca_partida(buffer, cliente);

            // Caso o comando seja valido
            if(validade == SUCESSO){

                // Salva a partida que o cliente estava
                JOGO *partida = cliente->partida;

                //Caso ele fosse o jogador1, remove da partida. Realiza o mesmo caso seja o jogador2
                if(cliente == cliente->partida->jogador1)
                    cliente->partida->jogador1 = NULL;
                else if(cliente == cliente->partida->jogador2)
                    cliente->partida->jogador2 = NULL;

                // Atualiza os dados da struct, dizendo que a partida agora é NULL e que o estado é de esperando nova partida
                cliente->partida = NULL;
                cliente->estado = ESPERANDO_PARTIDA;

                // Caso apos isso, nenhum jogador esteja na partida, ou seja, ambos jogadores sejam NULL
                if(partida->jogador1 == NULL && partida->jogador2 == NULL){
                    // Printa no servidor que a partida foi deletada, exclui da lista e decrementa o contador de partidas
                    printf("PARTIDA DE ID = %d FINALIZADA E EXCLUIDA\n", partida->id);
                    deleta_jogo(partida, jogos);
                    quantidade_jogos--;
                }

            }

            // Libera o mutex
            pthread_mutex_unlock(&mutex);

        }

        // Independente do estado, sempre chama a funcao de atualizar partida, para atualizar o terminal de quem enviou a mensagem
        atualizar_partida(buffer, bytes_recebidos, cliente);

        // Printa no terminal do servidor a mensagem enviada pelo cliente
        printf("Cliente %d: %s", socket_cliente, buffer);

    }

    // Caso saia do loop, o cliente desconectou. Dessa forma, printa no terminal do servidor que o cliente foi desconectado
    printf("Cliente desconectado: %d\n", socket_cliente);

    // Chama a funcao de remover o cliente, que lida com o fato de talvez o cliente estar ou nao numa partida existente
    remover_cliente(cliente);
    
    // Finaliza o socket do cliente
    close(socket_cliente);

    // Retorna NULL e, portanto, finaliza a thread
    return NULL;
}

// Funcao main, principal do programa
int main(int argc, char *argv[]) {

    // Limpa o terminal do servidor
    system("clear");

    // Definicao de algumas das variaveis importantes para a abertura do servidor
    int servidor;
    int cliente;
    int porta;
    CLIENTE *novo_cliente;
    struct sockaddr_in endereco;
    socklen_t tamanho_endereco = sizeof(endereco);
    int opt = 1;

    // Define a semente do random a partir do horario atual do computador
    srand(time(NULL));

    // Atualiza a variavel qtd_pontos de acordo com a quantiadade de navios configuradas em consts.h
    qtd_pontos = 0;
    for(int i = 0; i<TAM_MAXIMO_NAVIO; i++){
        qtd_pontos += i * numero_navios[i];
    }

    // Inicializa ambas as listas de clientes e de jogos
    clientes = cria_lista_clientes();
    jogos = cria_lista_jogos();

    // Garante que, inicialmente, a variavel que guarda o cliente esperando por uma partida seja NULL
    cliente_esperando = NULL;

    // Inicializa o servidor, com ipv4 e TCP
    servidor = socket(AF_INET, SOCK_STREAM, 0);

    // Caso o servidor retorne algo menor que 0, houve erro na funcao socket
    if (servidor < 0) {
        perror("Erro ao criar socket");
        return 1;
    }

    // Permite a reutilizacao imediata do endereco de rede apos o encerramento do servidor
    if (setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        // Entra na condicao caso tenha acontecido algum erro. Encerra o servidor e finaliza o programa
        perror("Erro no setsockopt");
        close(servidor);
        return 1;
    }

    // Define protocolo ipv4 e aceita qualquer interface de rede
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;

    // Caso o programa nao tenha recebido nenhum argumento, utiliza a porta padrao, definida em consts.h
    if(argc == 1)
        porta = PORTA_PADRAO;
    // Caso contrario, obtem a porta pelo segundo argumento recebido
    else if(argc == 2)
        porta = atoi(argv[1]);

    // Define a porta a ser utilizada
    endereco.sin_port = htons(porta);

    // Associa o socket ao endereco de rede. Caso retorne um valor menor que zero: houve erro, fecha o servidor e encerra o programa
    if (bind(servidor, (struct sockaddr *)&endereco, sizeof(endereco)) < 0) {
        perror("Erro no bind");
        close(servidor);
        return 1;
    }

    // Coloca o socket em modo de escuta. Caso retorne um valor menor que zero: houve erro, fecha o servidor e encerra o programa
    if (listen(servidor, MAX_CLIENTES) < 0) {
        perror("Erro no listen");
        close(servidor);
        return 1;
    }

    // Se chegou aqui, o servidor foi inicializado corretamente. Imprime no terminal do servidor a porta do endereco de rede
    printf("Servidor iniciado na porta %d...\n", porta);

    // Loop infinito que aceita clientes
    while (1) {
        // Aguarda um cliente tentar se conectar
        cliente = accept(servidor, (struct sockaddr *)&endereco, &tamanho_endereco);

        // Caso o socket retornado seja menor que zero, houve erro entao reinicia o loop
        if (cliente < 0) {
            perror("Erro no accept");
            continue;
        }

        // Bloqueia o mutex
        pthread_mutex_lock(&mutex);

        // Caso a quantidade de clientes seja igual ou maior que o maximo permitido
        if (quantidade_clientes >= MAX_CLIENTES) {
            // Libera o mutex, envia mensagem ao cliente que ja esta cheio, encerra o cliente e retorna o loop
            pthread_mutex_unlock(&mutex);
            send(cliente, "Servidor cheio.\n", 16, 0);
            close(cliente);
            continue;
        }

        // Aloca e adiciona o cliente na lista de clientes conectados, incrementando o contador
        novo_cliente = adiciona_cliente(cliente, clientes);
        quantidade_clientes++;

        // Libera o mutex
        pthread_mutex_unlock(&mutex);

        // Cria a thread responsavel por atender ao cliente
        pthread_t thread;
        pthread_create(&thread, NULL, atender_cliente, novo_cliente);

        // Avisa ao sistema operacional para liberar automaticamente a thread quando ela encerrar seu processamento
        pthread_detach(thread);

    }

    // O servidor nao possui uma rotina de encerramente, ficando ativo indefinidamente
    // Portanto esse close(servidor) nunca é de fato alcançado
    // Esta aqui apenas por motivos didaticos (caso em algum momento seja implementado uma rotina de encerramento)
    close(servidor);

    return 0;
}