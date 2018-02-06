all: phist clean

phist: wordcount dirdivider
	gcc phist.c dirdivider.o wordcount.o -o phist -g

dirdivider: dirdivider.c dirdivider.h
	gcc dirdivider.c -c -g

wordcount: wordcount.c wordcount.h
	gcc wordcount.c -c -g

clean:
	rm *.o
