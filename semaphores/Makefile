release:
	make server
	make client

debug:
	make sdebug
	make sclient

client: client.c
	gcc -pthread -o client.out -I . client.c

server: server.c
	gcc -pthread -o server.out -I . server.c

s-pthread debug: 
	gcc -pthread -Wall -Wextra -ggdb -O0 -I . -o server.debug.out server.c

sclient:
	gcc -pthread -Wall -Wextra -ggdb -O0 -I . -o client.debug.out client.c
