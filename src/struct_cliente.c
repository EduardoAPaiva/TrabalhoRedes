#include "../includes/struct_cliente.h"

// Funcao que aloca uma lista de clientes e retorna a lista vazia
LISTA_CLIENTES* cria_lista_clientes(){

    // Aloca a lista
    LISTA_CLIENTES *lista = (LISTA_CLIENTES *)malloc(sizeof(LISTA_CLIENTES));

    // Printa um erro caso a lista alocada ainda seja NULL
    if(lista == NULL)
        printf("ERRO AO ALOCAR LISTA DE CLIENTES\n");

    // Retorna a lista alocada vazia
    return lista;

}

// Funcao que adiciona um cliente no final de uma lista encadeada
CLIENTE *adiciona_cliente(int socket, LISTA_CLIENTES *lista){
    
    // Aloca o novo cliente 
    CLIENTE *novo_cliente = (CLIENTE *)malloc(sizeof(CLIENTE));

    // Printa um erro caso o cliente alocado ainda seja NULL e retorna NULL
    if(novo_cliente == NULL){
        printf("ERRO NA ALOCACAO DO CLIENTE\n");
        return NULL;
    }

    // Salva o socket passado para a funcao no cliente
    novo_cliente->socket = socket;

    // Define-se alguns valores padrao para determinadas variaveis do cliente
    novo_cliente->prox = NULL;
    novo_cliente->cadastrado = 0;
    novo_cliente->estado = ESPERANDO_PARTIDA;
    novo_cliente->partida = NULL;

    // Caso a lista esteja vazia, adiciona o cliente no inicio e retorna o ponteiro do novo cliente
    if(*lista == NULL){
        *lista = novo_cliente;
        novo_cliente->ant = NULL;

        return novo_cliente;
    }

    // Cria um cliente auxiliar para percorrer a lista
    CLIENTE *aux = *lista;

    // Loop que caminha ate o final da lista encadeada
    while(aux->prox != NULL){
        aux = aux->prox;
    }

    // Adiciona o novo cliente ao final da lista
    aux->prox = novo_cliente;
    novo_cliente->ant = aux;

    // Retorna o ponteiro do novo cliente
    return novo_cliente;

}

// Funcao que remove um cliente especificado da lista encadeada
void deleta_cliente(CLIENTE *cliente, LISTA_CLIENTES *lista){

    // Caso o cliente seja o primeiro da lista
    if(*lista == cliente){
        // Atualiza os ponteiros para remover o cliente
        *lista = cliente->prox;
        if(*lista != NULL) (*lista)->ant = NULL;
    }
    // Caso o cliente nao seja o primeiro da lista
    else{
        // Atualiza os ponteiros para remover o cliente
        (cliente->ant)->prox = cliente->prox;
        if(cliente->prox != NULL) (cliente->prox)->ant = cliente->ant;
    }

    // Libera a memoria alocada para o cliente
    free(cliente);

}