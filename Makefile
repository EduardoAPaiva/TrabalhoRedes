all:
	gcc server.c src/*.c -o server
	gcc client.c src/*.c -o client