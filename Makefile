all: client server

client: client.c
	gcc -o client client.c -pthread -L/usr/local/lib -I/usr/local/include -lreadline -w

server: server.c
