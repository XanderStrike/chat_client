all: client server

client: client.c
	gcc -o client -pthread client.c

server: server.c
