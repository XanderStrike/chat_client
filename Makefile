#client: client.c
#	gcc -o client -pthread client.c

server: server.c
	gcc -o server -pthread server.c
