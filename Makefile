#client: client.c
#	gcc -o client -pthread client.c
client: server.c
	gcc -o server -pthread server.c
