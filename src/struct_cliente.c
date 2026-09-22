#include "../includes/struct_cliente.h"

LISTA_CLIENTES* cria_lista_clientes(){

    LISTA_CLIENTES *lista = (LISTA_CLIENTES *)malloc(sizeof(LISTA_CLIENTES));

    if(lista == NULL)
        printf("ERRO AO ALOCAR LISTA DE CLIENTES\n");

    return lista;

}

CLIENTE *adiciona_cliente(int socket, LISTA_CLIENTES *lista){
    
    CLIENTE *novo_cliente = (CLIENTE *)malloc(sizeof(CLIENTE));

    novo_cliente->socket = socket;
    novo_cliente->prox = NULL;
    novo_cliente->cadastrado = 0;
    novo_cliente->estado = ESPERANDO_PARTIDA;
    novo_cliente->partida = NULL;

    if(*lista == NULL){
        *lista = novo_cliente;
        novo_cliente->ant = NULL;

        return novo_cliente;
    }

    CLIENTE *aux = *lista;

    while(aux->prox != NULL){
        aux = aux->prox;
    }

    aux->prox = novo_cliente;
    novo_cliente->ant = aux;

    return novo_cliente;

}

void deleta_cliente(CLIENTE *cliente, LISTA_CLIENTES *lista){

    if(*lista == cliente){
        *lista = cliente->prox;
        if(*lista != NULL) (*lista)->ant = NULL;
    }

    else{
        (cliente->ant)->prox = cliente->prox;
        if(cliente->prox != NULL) (cliente->prox)->ant = cliente->ant;
    }

    free(cliente);

}