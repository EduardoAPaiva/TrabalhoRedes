all:
	mkdir -p bin
	gcc apps/server.c src/*.c -o bin/server
	gcc apps/client.c -o bin/client