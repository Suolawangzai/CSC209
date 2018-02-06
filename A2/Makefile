all: boggle clean

test: board user
	gcc test.c board.o user.o -o test -g

boggle: board user dic
	gcc boggle.c board.o user.o dictionary.o -o boggle -g

board: board.c board.h
	gcc board.c -c -g

user: user.c user.h
	gcc user.c -c -g

dic: dictionary.c dictionary.c
	gcc dictionary.c -c -g

clean:
	rm *.o 
	rm -r *.dSYM