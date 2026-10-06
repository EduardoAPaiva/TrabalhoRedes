all:
	mkdir -p bin
	gcc apps/server.c src/*.c -o bin/server
	gcc apps/client.c src/*.c -o bin/client