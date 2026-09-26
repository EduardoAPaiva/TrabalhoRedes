#include "../includes/funcoes.h"

// Constante com as opcoes de tamanhos de navios existentes
int numero_navios[TAM_MAXIMO_NAVIO] = NUMERO_NAVIOS_DISPONIVEIS;
int qtd_pontos = 0;

// Funcao que salva a string recebida no nome (nickname) do cliente
void salvar_nome(char *nome, CLIENTE *cliente){
    strcpy(cliente->nickname, nome);        // Copia a string recebida para o nome do cliente
    cliente->cadastrado = 1;                // Altera o booleano do cliente para cadastrado = 1 (True)
}

// Funcao que coloca os caracteres iniciais corretos nos tabuleiros da partida 
void preparar_tabuleiro(JOGO *partida){

    // Para os quatro tabuleiros do jogo, preenche a primeira e ultima linha com os caracteres '='
    // Para os quatro tabuleiros do jogo, preenche a primeira e ultima coluna com os caracteres '|'
    // Para os quatro tabuleiros do jogo, preenche o restante com caractere ' '
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

    // Salva no vetor de qtd da navios disponiveis dos jogadores a quantidade padrao definida
    for(int i = 0; i<TAM_MAXIMO_NAVIO; i++){
        partida->navios_disponiveisJ1[i] = numero_navios[i];
        partida->navios_disponiveisJ2[i] = numero_navios[i];
    }

}

// Funcao que reseta o posicionamento de navios. Ou seja, transforma o tabuleiro no padrão vazio
void resetar_tabuleiro(CLIENTE *cliente){

    // Salva a partida
    JOGO *partida = cliente->partida;

    // Para o tabuleiro de navios posicionados do cliente, reseta o tabuleiro para o formato padrao
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

    // Reseta a quantidade de navios disponiveis para o jogador que resetou o tabuleiro
    for(int i = 0; i<TAM_MAXIMO_NAVIO; i++){

        if(cliente == partida->jogador1)
            partida->navios_disponiveisJ1[i] = numero_navios[i];
        else if(cliente == partida->jogador2)
            partida->navios_disponiveisJ2[i] = numero_navios[i];

    }

}

// Funcao que coloca num buffer um texto, depois um tabuleiro, e depois outro texto. Esse buffer sera enviado para atualizar o terminal do cliente
void* texto_tabuleiro(char buffer[], char antes[], char depois[], char tabuleiro[12][12], int linhatiro, int colunatiro){

    // Variavel que ira sempre salvar a posicao do ultimo caractere adicionado no buffer
    int pos = 0;

    // Adiciona o texto passado antes do tabuleiro
    pos += sprintf(buffer + pos, "%s\n", antes);

    // Adiciona a linha que mostrara a posicao de cada coluna e os caracteres de '=' que ficam acima do tabuleiro
    pos += sprintf(buffer + pos, "      A  B  C  D  E  F  G  H  I  J   \n");
    pos += sprintf(buffer + pos, "   ==================================\n");

    // Para cada posicao do tabuleiro, printa no buffer
    for(int i = 1; i<11; i++){
        for(int j = 0; j<12; j++){
            
            // Caso for a primeira coluna, imprime a linha correspondente
            if(j == 0)
                pos += sprintf(buffer + pos, "%d ", i-1);
            // Caso seja o ultimo tiro, printa o comando de cor para indicar que foi o ultimo tiro
            if(i == linhatiro && j == colunatiro)
                pos += sprintf(buffer + pos, COR_VERMELHO " %c " COR_RESET, tabuleiro[i][j]);
            // Caso nao seja o ultimo tiro, nao printa o codigo de cor
            else
                pos += sprintf(buffer + pos, " %c ", tabuleiro[i][j]);

        }

        pos += sprintf(buffer + pos, "\n");

    }

    // Printa a linha final no buffer
    pos += sprintf(buffer + pos, "   ==================================\n");

    // Printa o texto que ficara apos o tabuleiro
    pos += sprintf(buffer + pos, "\n%s\n", depois);

}

// Funcao que analisa quantos navios uma lista de navios tem de disponibilidade, e cria o texto pra isso
void *texto_navios_disponiveis(int navios[TAM_MAXIMO_NAVIO], char buffer[]){

    // Variavel que ira sempre salvar a posicao do ultimo caractere adicionado no buffer
    int pos = 0;

    // Texto padrao que fica antes de mostrar a quantidade de navios
    pos += sprintf(buffer + pos, "POSICIONE SEUS NAVIOS\nVOCE AINDA TEM:\n");

    // Loop que ira printar quantos navios de cada tamanho tem disponivel, caso tenha pelo menos um
    for(int i = 0; i<TAM_MAXIMO_NAVIO; i++){

        if(navios[i] != 0){
            pos += sprintf(buffer + pos, "%d NAVIO DE TAMANHO %d\n", navios[i], i);
        }

    }

    // Texto padrao que fica apos mostrar a qtd de navios. Indica a formatacao necessaria para o comando
    pos += sprintf(buffer + pos, "\nFORMATACAO:  N P A0\n\nEm N digite o tamanho do navio\nEm P a orientacao ('H' para horizontal e 'V' para vertical)\nEm A0 a casa onde deseja colocar, com coluna e linha juntos\n\nONDE DESEJA POSICIONAR: ");

    // Retorna o buffer
    return buffer;

}

// Funcao que analisa se um comando de posicionamento de navio recebido pelo cliente esta valido de acordo com o estado da partida e do tabuleiro
int validar_posicionamento(char buffer[], CLIENTE *cliente){

    // Retorna codigo de resetar tabuleiro caso a string seja apenas "R", que significa que o jogador deseja resetar o tabuleiro
    if(buffer[0] == 'R' && (buffer[1] == '\n' || buffer[1] == '\0'))
        return RESETAR_TABULEIRO;

    // Retorna codigo de erro de tamanho invalido, caso o tamanho de navio recebido esteja fora dos valores possiveis
    if(buffer[0] < '1' || buffer[0] > '0' + TAM_MAXIMO_NAVIO)
        return TAMANHO_INVALIDO;

    // Retorna codigo de erro de formatacao invalida caso o segundo caractere nao seja um espaço vazio
    if(buffer[1] != ' ')
        return FORMATACAO_ERRADA;

    // Retorna codigo de erro de orientacao invalida caso o terceiro caractere nao seja 'H' de horizontal ou 'V' de vertical
    if(buffer[2] != 'H' && buffer[2] !=  'V')
        return ORIENTACAO_INVALIDA;

    // Retorna codigo de erro de formatacao invalida caso o quarto caractere nao seja um espaço vazio
    if(buffer[3] != ' ')
        return FORMATACAO_ERRADA;

    // Retorna codigo de erro de coluna invalida caso o quinto caractere nao esteja entre 'A' e 'J'
    if(buffer[4] < 'A' || buffer[4] > 'J')
        return COLUNA_INVALIDA;

    // Retorna codigo de erro de linha invalida caso o sexto caractere nao esteja entre '0' e '9'
    if(buffer[5] < '0' || buffer[5] > '9')
        return LINHA_INVALIDA;

    // Retorna codigo de erro de formatacao invalida caso o setimo caractere nao seja '\0' ou '\n'
    if(buffer[6] != '\0' && buffer[6] != '\n')
        return FORMATACAO_ERRADA;

    // Define a linha, coluna e o tamanho do navio recebidos
    int linha = buffer[5]-'0'+1;
    int coluna = buffer[4] - 'A'+1;
    int tamanho = buffer[0] - '0';

    // Salva a partida que o cliente esta
    JOGO *partida = cliente->partida;

    // Caso o cliente seja o jogador1 mas o jogador1 nao tenha navios do tamanho solicitado, retorna codigo de erro de tamanho de navio
    if(cliente == partida->jogador1 && partida->navios_disponiveisJ1[tamanho] == 0)
        return TAMANHO_DE_NAVIO_NAO_DISPONIVEL;
    // Caso o cliente seja o jogador2 mas o jogador2 nao tenha navios do tamanho solicitado, retorna codigo de erro de tamanho de navio
    if(cliente == partida->jogador2 && partida->navios_disponiveisJ2[tamanho] == 0)
        return TAMANHO_DE_NAVIO_NAO_DISPONIVEL;

    // Caso a orientacao seja horizontal mas a ultima posicao do navio ultrapasse o fim do tabuleiro, retorna codigo de erro de navio fora do tabuleiro
    if(buffer[2] == 'H' && coluna + tamanho > 11)
        return NAVIO_FORA_DO_TABULEIRO;
    // Caso a orientacao seja vertical mas a ultima posicao do navio ultrapasse o fim do tabuleiro, retorna codigo de erro de navio fora do tabuleiro
    if(buffer[2] == 'V' && linha + tamanho > 11)
        return NAVIO_FORA_DO_TABULEIRO;

    // Define a flag que analisara se todas as posicoes que o navio sera colocado estao livres
    int flag_valido = TRUE;
    // Loop que ira fazer a analise
    for(int i = 0; i<tamanho; i++){

        // Caso o cliente seja o jogador1, orientacao horizontal e a posicao do navio do jogador1 esteja ocupada, define flag_valido como FALSE
        if(cliente == partida->jogador1 && buffer[2] == 'H' && partida->tabuleiro_naviosJ1[linha][coluna+i] != ' ')
            flag_valido = FALSE;
        // Caso o cliente seja o jogador2, orientacao horizontal e a posicao do navio do jogador2 esteja ocupada, define flag_valido como FALSE
        if(cliente == partida->jogador2 && buffer[2] == 'H' && partida->tabuleiro_naviosJ2[linha][coluna+i] != ' ')
            flag_valido = FALSE;
        // Caso o cliente seja o jogador1, orientacao vertical e a posicao do navio do jogador1 esteja ocupada, define flag_valido como FALSE
        if(cliente == partida->jogador1 && buffer[2] == 'V' && partida->tabuleiro_naviosJ1[linha+i][coluna] != ' ')
            flag_valido = FALSE;
        // Caso o cliente seja o jogador2, orientacao vertical e a posicao do navio do jogador2 esteja ocupada, define flag_valido como FALSE
        if(cliente == partida->jogador2 && buffer[2] == 'V' && partida->tabuleiro_naviosJ2[linha+i][coluna] != ' ')
            flag_valido = FALSE;

    }

    // Caso alguma posicao que colocou o navio esteja ocupada, retorna codigo de erro de navio colidindo
    if(flag_valido == FALSE)
        return NAVIO_COLIDINDO;

    // Como o navio foi colocado numa posicao valida, loop que altera todas as posicoes do navio para o caractere 'O'
    for(int i = 0; i<tamanho; i++){

        // Caso o cliente seja o jogador1 e orientacao horizontal, altera a posicao equivalente
        if(cliente == partida->jogador1 && buffer[2] == 'H')
            partida->tabuleiro_naviosJ1[linha][coluna+i] = 'O';
        // Caso o cliente seja o jogador2 e orientacao horizontal, altera a posicao equivalente
        if(cliente == partida->jogador2 && buffer[2] == 'H')
            partida->tabuleiro_naviosJ2[linha][coluna+i] = 'O';
        // Caso o cliente seja o jogador1 e orientacao vertical, altera a posicao equivalente
        if(cliente == partida->jogador1 && buffer[2] == 'V')
            partida->tabuleiro_naviosJ1[linha+i][coluna] = 'O';
        // Caso o cliente seja o jogador2 e orientacao vertical, altera a posicao equivalente
        if(cliente == partida->jogador2 && buffer[2] == 'V')
            partida->tabuleiro_naviosJ2[linha+i][coluna] = 'O';

    }

    // Caso o cliente seja o jogador1, diminui um na quantidade de navios disponiveis do tamanho adicionado no tabuleiro
    if(cliente == partida->jogador1)
        partida->navios_disponiveisJ1[tamanho]--;
    // Caso o cliente seja o jogador2, diminui um na quantidade de navios disponiveis do tamanho adicionado no tabuleiro
    if(cliente == partida->jogador2)
        partida->navios_disponiveisJ2[tamanho]--;


    // Flag que ira avaliar se todos os navios do jogador ja foram posicionados
    int flag_navios_todos_posicionados = TRUE;
    // Loop que ira fazer a analise
    for(int i = 0; i<TAM_MAXIMO_NAVIO; i++){
        
        // Caso o cliente seja o jogador1 e valor da quantidade de navios seja diferente de zero, define a flag como FALSE
        if(cliente == partida->jogador1 && partida->navios_disponiveisJ1[i] != 0)
            flag_navios_todos_posicionados = FALSE;
        // Caso o cliente seja o jogador2 e valor da quantidade de navios seja diferente de zero, define a flag como FALSE
        if(cliente == partida->jogador2 && partida->navios_disponiveisJ2[i] != 0)
            flag_navios_todos_posicionados = FALSE;

    }

    // Caso todos o navios ja estejam posicionados, retorna o codigo equivalente
    if(flag_navios_todos_posicionados)
        return NAVIOS_TODOS_POSICIONADOS;

    // Por fim, retorna sucesso no posicionamento
    return SUCESSO;

}

// Funcao que analisa se um comando de ataque recebido pelo cliente esta valido de acordo com o estado da partida e do tabuleiro
int validar_ataque(char buffer[], CLIENTE *cliente){

    // Retorna codigo de erro de coluna invalida caso o quinto caractere nao esteja entre 'A' e 'J'
    if(buffer[0] < 'A' || buffer[0] > 'J')
        return COLUNA_INVALIDA;

    // Retorna codigo de erro de linha invalida caso o sexto caractere nao esteja entre '0' e '9'
    if(buffer[1] < '0' || buffer[1] > '9')
        return LINHA_INVALIDA;

    // Retorna codigo de erro de formatacao invalida caso o terceiro caractere nao seja '\0' ou '\n'
    if(buffer[2] != '\0' && buffer[2] != '\n')
        return FORMATACAO_ERRADA;

    // Salva a partida do cliente
    JOGO *partida = cliente->partida;

    // Obtem a linha e a coluna do comando de ataque
    int linha = buffer[1]-'0'+1;
    int coluna = buffer[0] - 'A'+1;

    // Caso o cliente seja o jogador1 e a casa do comando ja tenha sido atacada antes, retorna codigo de erro de casa ja atacada
    if(cliente == partida->jogador1 && partida->tabuleiro_tirosJ1[linha][coluna] != ' ')
        return CASA_JA_ATACADA;
    // Caso o cliente seja o jogador2 e a casa do comando ja tenha sido atacada antes, retorna codigo de erro de casa ja atacada
    else if(cliente == partida->jogador2 && partida->tabuleiro_tirosJ2[linha][coluna] != ' ')
        return CASA_JA_ATACADA;


    // Caso o cliente seja o jogador1
    if(cliente == partida->jogador1){

        // Atualiza o local do ultimo tiro do jogador 1
        partida->ultimo_tiroJ1[0] = linha;
        partida->ultimo_tiroJ1[1] = coluna;

        // Caso a casa atacada pelo jogador1 seja um navio do jogador2
        if(partida->tabuleiro_naviosJ2[linha][coluna] == 'O'){
            // Diminui a quantidade de vidas do jogador2 e altera o tabuleiro de tiros do jogador1 para 'O'
            partida->pontos_restantesJ2--;
            partida->tabuleiro_tirosJ1[linha][coluna] = 'O';

            // Atualiza a casa atacada pelo jogador1 para 'X' no tabuleiro de navios do jogador2
            partida->tabuleiro_naviosJ2[linha][coluna] = 'X';

            // Caso o jogador2 nao tenha mais vidas restantes
            if(partida->pontos_restantesJ2 == 0){
                // Define o jogador1 como vitorioso e atualiza o terminal dele
                partida->jogador1->estado = VITORIA;
                terminal_vitoria(partida, partida->jogador1);

                // Define o jogador2 como derrotado e atualiza o terminal dele
                partida->jogador2->estado = DERROTA;
                terminal_derrota(partida, partida->jogador2);

                // Retorna codigo de partida finalizada
                return PARTIDA_FINALIZADA;
            }

        }
        // Caso a casa atacada pelo jogador1 nao seja um navio do jogador2, apenas atualiza o tabuleiro de tiros do jogador1 para 'X'
        else{
            partida->tabuleiro_tirosJ1[linha][coluna] = 'X';

            // Atualiza a casa atacada pelo jogador1 para 'X' no tabuleiro de navios do jogador2
            partida->tabuleiro_naviosJ2[linha][coluna] = 'X';
        }

    }
    // Caso o cliente seja o jogador2
    if(cliente == partida->jogador2){

        // Atualiza o local do ultimo tiro do jogador 2
        partida->ultimo_tiroJ2[0] = linha;
        partida->ultimo_tiroJ2[1] = coluna;

        // Caso a casa atacada pelo jogador1 seja um navio do jogador1
        if(partida->tabuleiro_naviosJ1[linha][coluna] == 'O'){
            // Diminui a quantidade de vidas do jogador1 e altera o tabuleiro de tiros do jogador2 para 'O'
            partida->pontos_restantesJ1--;
            partida->tabuleiro_tirosJ2[linha][coluna] = 'O';
            
            // Atualiza a casa atacada pelo jogador2 para 'X' no tabuleiro de navios do jogador1
            partida->tabuleiro_naviosJ1[linha][coluna] = 'X';

            // Caso o jogador1 nao tenha mais vidas restantes
            if(partida->pontos_restantesJ1 == 0){
                // Define o jogador2 como vitorioso e atualiza o terminal dele
                partida->jogador2->estado = VITORIA;
                terminal_vitoria(partida, partida->jogador2);

                // Define o jogador1 como derrotado e atualiza o terminal dele
                partida->jogador1->estado = DERROTA;
                terminal_derrota(partida, partida->jogador1);

                // Retorna codigo de partida finalizada
                return PARTIDA_FINALIZADA;
            }

        }
        // Caso a casa atacada pelo jogador2 nao seja um navio do jogador1, apenas atualiza o tabuleiro de tiros do jogador2 para 'X'
        else{
            partida->tabuleiro_tirosJ2[linha][coluna] = 'X';

            // Atualiza a casa atacada pelo jogador2 para 'X' no tabuleiro de navios do jogador1
            partida->tabuleiro_naviosJ1[linha][coluna] = 'X';
        }

    }

    // Retorna o codigo de lance executado com sucesso
    return SUCESSO;

}

// Funcao que analisa se o comando recebido é exatamente a string "BUSCAR"
int validar_busca_partida(char buffer[], CLIENTE *cliente){

    // Compara o texto recebido com a string "BUSCAR", retorna sucesso caso positivo
    if(strcmp(buffer, "BUSCAR") == 0 || strcmp(buffer, "BUSCAR\n") == 0)
        return SUCESSO;
    
    // Retorna codigo de formatacao errada caso nao entre na condicao
    return FORMATACAO_ERRADA;

}

// Funcao que altera o estado do cliente para aguardando posicionar. Analisa se o outro jogador ja terminou de posicionar e inicia a partida
void aguardar_adversario_posicionar(CLIENTE *cliente){

    // Salva a partida do cliente
    JOGO *partida = cliente->partida;

    // Caso o cliente seja o jogador1 e o jogador2 ainda esteja posicionando navios, apenas altera o estado do cliente para esperando adversario
    if(cliente == partida->jogador1 && partida->jogador2->estado == POSICIONANDO_NAVIOS)
        cliente->estado = AGUARDANDO_ADVERSARIO_POSICIONAR;
    // Caso o cliente seja o jogador2 e o jogador1 ainda esteja posicionando navios, apenas altera o estado do cliente para esperando adversario
    else if(cliente == partida->jogador2 && partida->jogador1->estado == POSICIONANDO_NAVIOS)
        cliente->estado = AGUARDANDO_ADVERSARIO_POSICIONAR;

    // Caso o cliente seja o jogador1 e o jogador2 ja esteja aguardando o adversario
    else if(cliente == partida->jogador1 && partida->jogador2->estado == AGUARDANDO_ADVERSARIO_POSICIONAR){

        // Randomiza quem ira comecar
        int op = rand() % 2;

        // Caso op for zero, define jogador1 em turno e jogador2 esperando turno e atualiza o terminal de ambos jogadores
        if(op == 0){
            partida->jogador1->estado = EM_TURNO;
            partida->jogador2->estado = ESPERANDO_TURNO;
            terminal_em_turno(partida, partida->jogador1);
            terminal_esperando_turno(partida, partida->jogador2);
        }
        // Caso op for um, define jogador2 em turno e jogador1 esperando turno e atualiza o terminal de ambos jogadores
        else if(op == 1){
            partida->jogador2->estado = EM_TURNO;
            partida->jogador1->estado = ESPERANDO_TURNO;
            terminal_em_turno(partida, partida->jogador2);
            terminal_esperando_turno(partida, partida->jogador1);
        }
    }
    // Caso o cliente seja o jogador2 e o jogador1 ja esteja aguardando o adversario
    else if(cliente == partida->jogador2 && partida->jogador1->estado == AGUARDANDO_ADVERSARIO_POSICIONAR){

        // Randomiza quem ira comecar
        int op = rand() % 2;

        // Caso op for zero, define jogador1 em turno e jogador2 esperando turno e atualiza o terminal de ambos jogadores
        if(op == 0){
            partida->jogador1->estado = EM_TURNO;
            partida->jogador2->estado = ESPERANDO_TURNO;
            terminal_em_turno(partida, partida->jogador1);
            terminal_esperando_turno(partida, partida->jogador2);
        }
        // Caso op for um, define jogador2 em turno e jogador1 esperando turno e atualiza o terminal de ambos jogadores
        else if(op == 1){
            partida->jogador2->estado = EM_TURNO;
            partida->jogador1->estado = ESPERANDO_TURNO;
            terminal_em_turno(partida, partida->jogador2);
            terminal_esperando_turno(partida, partida->jogador1);
        }

    }

}