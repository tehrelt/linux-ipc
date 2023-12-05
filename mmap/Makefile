client: client.c
	gcc -o client.out -I . client.c

server: server.c
	gcc -o server.out -I . server.c

sdebug: 
	gcc -Wall -Wextra -ggdb -O0 -I . -o server.debug.out server.c

sclient:
	gcc -Wall -Wextra -ggdb -O0 -I . -o client.debug.out client.c
