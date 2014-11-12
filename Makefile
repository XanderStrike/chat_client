all: client server

client: client.c
	gcc -o client -pthread client.c -L/usr/local/lib -I/usr/local/include -lreadline

server: server.c
