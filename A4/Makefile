CFLAGS= -DPORT=\$(PORT) -g -std=c99 -Werror
PORT=51352

game_local: game_local.o game.o 
	gcc $(CFLAGS) -o game_local game_local.o game.o

game_local.o: game_local.c game.h
	gcc $(CFLAGS) -c game_local.c

game.o: game.c game.h
	gcc $(CFLAGS) -c game.c

clean: 
	rm game_local *.o
