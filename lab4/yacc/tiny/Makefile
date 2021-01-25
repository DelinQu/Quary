#
# Makefile for yacc
# created by qdl in 2020/10/30
# 

CC = gcc

CFLAGS = -w

OBJS = main.o util.o scan.o parse.o


bison: main.o util.o scan.o parse.o
	$(CC) $(CFLAGS) main.o util.o scan.o parse.o -o bison -lfl
	
main.o: main.c globals.h util.h scan.h
	$(CC) $(CFLAGS) -c main.c
	
util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

scan.o: lex.yy.c scan.h util.h globals.h
	$(CC) $(CFLAGS) -c lex.yy.c -o scan.o

parse.o: tiny.tab.c parse.h scan.h globals.h util.h
	$(CC) $(CFLAGS) -c tiny.tab.c -o parse.o

clean:
	-rm bison
	-rm main.o
	-rm util.o
	-rm scan.o
	-rm parse.o