all: bwtsearch

bwtsearch: bwtsearch.c
	gcc -o bwtsearch bwtsearch.c -lm


