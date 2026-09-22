# Batalha Naval — Cliente/Servidor

## 1. Descrição

Este projeto consiste na implementação de uma aplicação cliente-servidor utilizando sockets TCP em C, desenvolvida como trabalho de implementação de comunicação entre processos através de uma rede.

A aplicação implementa o jogo Batalha Naval, permitindo que diversas partidas ocorram simultaneamente no mesmo servidor. Os clientes se conectam ao servidor e podem entrar em uma partida com outro jogador, enquanto o servidor é responsável pelo gerenciamento das conexões, partidas e comunicação entre os jogadores.

A arquitetura foi desenvolvida para suportar múltiplas conexões simultâneas por meio do uso de threads, permitindo que diferentes partidas sejam executadas de forma independente.

## 2. Funcionalidades

- Comunicação entre cliente e servidor utilizando socket TCP;
- Conexão simultânea de vários clientes;
- Criação e gerenciamento de múltiplas partidas simultâneas;
- Associação de dois jogadores em cada partida;
- Posicionamento das embarcações no tabuleiro;
- Realização de jogadas entre os jogadores;
- Verificação dos disparos realizados;
- Atualização do estado da partida;
- Identificação do vencedor;
- Encerramento das partidas;
- Tratamento do encerramento e desconexão dos clientes;
- Gerenciamento das conexões através de threads.

## 3. Arquitetura da aplicação

A aplicação é dividida em duas partes principais:

### Servidor

O servidor é responsável por:

1. Criar e configurar o socket;
2. Aguardar novas conexões;
3. Aceitar múltiplos clientes;
4. Criar uma thread para o gerenciamento de cada cliente;
5. Organizar os clientes em partidas;
6. Intermediar a comunicação entre os jogadores;
7. Gerenciar o estado de cada partida;
8. Encerrar corretamente as conexões e threads.

### Cliente

O cliente é responsável por:

1. Conectar-se ao servidor;
2. Enviar informações e comandos referentes à partida;
3. Receber as respostas do servidor;
4. Exibir o estado do jogo;
5. Realizar as jogadas;
6. Informar o encerramento da conexão.

## 4. Múltiplas partidas simultâneas

Um dos principais objetivos da implementação é permitir que o servidor gerencie mais de uma partida simultaneamente.

Cada cliente conectado é associado a uma thread responsável pelo seu atendimento. Quando dois jogadores são associados a uma mesma partida, o servidor mantém as informações necessárias para que eles possam jogar de maneira independente das demais partidas.

Dessa forma, é possível ter, por exemplo:

                    SERVIDOR
                       |
          +------------+------------+
          |            |            |
       Partida 1    Partida 2    Partida 3
          |            |            |
        +---+        +---+        +---+
        |   |        |   |        |   |
       C1  C2       C3  C4       C5  C6

Assim, enquanto dois jogadores estão disputando uma partida, outros jogadores podem se conectar ao servidor e iniciar outras partidas.

## 5. Comunicação

A comunicação entre os clientes e o servidor é realizada utilizando sockets TCP.

O TCP foi utilizado por fornecer uma comunicação orientada à conexão, adequada para o envio das informações necessárias para o funcionamento da partida.

As mensagens trocadas são utilizadas para controlar eventos como:

- Conexão do cliente;
- Entrada em uma partida;
- Posicionamento das embarcações;
- Realização de jogadas;
- Resposta aos disparos;
- Atualização do estado da partida;
- Vitória ou derrota;
- Encerramento da conexão.

## 6. Tratamento de conexões

O servidor permanece em execução aguardando novas conexões.

Quando um cliente se conecta, o servidor aceita a conexão e cria uma nova thread para realizar o seu atendimento. Dessa maneira, o servidor não precisa interromper o atendimento dos clientes que já estão conectados para aceitar novas conexões.

O encerramento de uma conexão também é tratado pelo servidor, permitindo liberar os recursos associados ao cliente e atualizar o estado da partida quando necessário.

## 7. Estrutura do projeto

A estrutura do projeto é organizada da seguinte forma:

```text
.
├── server.c
├── client.c
├── includes/
│   ├── func.c
│   ├── func.h
│   └── struct_cliente.c
│   └── struct_cliente.h
│   └── struct_jogo.c
│   └── struct_jogo.h
├── run.sh
└── README.md
```

## 8. Requisitos

Para compilar e executar o projeto, é necessário:

- Sistema operacional Linux;
- Compilador GCC;
- Bibliotecas padrão de sockets e threads disponíveis no sistema;

O projeto utiliza apenas recursos disponíveis na linguagem C e nas bibliotecas padrão do sistema, conforme especificado no trabalho.

## 9. Compilação

Para compilar o projeto, execute:

```bash
./run.sh
```

## 10. Execução

### Servidor

Primeiramente, execute o servidor:

```bash
./server
```

O servidor ficará aguardando novas conexões de clientes.

### Cliente

Em outro terminal, execute:

```bash
./client
```

Em caso de conexão em outro computador conectado a mesma rede LAN, execute:

```bash
./client 192.168.X.X
```

Onde o endereço correponde ao IP do computador executando o servidor.

Para testar várias conexões simultaneamente, podem ser abertas várias instâncias do cliente em diferentes terminais.

Por exemplo:
```text

Terminal 1 → ./server

Terminal 2 → ./client
Terminal 3 → ./client
Terminal 4 → ./client
Terminal 5 → ./client
```

Nesse cenário, os clientes podem ser organizados em diferentes partidas simultaneamente.

## 11. Tecnologias utilizadas

- Linguagem: C
- Comunicação: Socket TCP
- Concorrência: POSIX Threads (pthread)
- Compilador: GCC
- Sistema operacional: Linux - Ubuntu

## 12. Integrantes

- Caio Cesar Trentin de Assis — 15674233
- Eduardo Alves Paiva — 15448481
- João Pedro Biazus Fagá  — 15483280
- Mariana do Nascimento Ferreira — 15582241

## 13. Considerações

O projeto demonstra a utilização de sockets para comunicação entre processos e o gerenciamento de múltiplas conexões simultâneas por meio de threads.

A aplicação escolhida foi uma Batalha Naval multiplayer, permitindo demonstrar de forma prática o estabelecimento de conexões, troca de mensagens, gerenciamento de jogadores e execução de diversas partidas independentes simultaneamente em um único servidor.