CC = gcc

CFLAGS = -w

OBJS = main.o util.o scan.o


all: Quary
	@echo "\n-------------------------------------------\nHi Coder! You compiled Quary successfully!\n=> Welcome to Quary! <=\n-------------------------------------------\\n"

Quary: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o Quary

main.o: main.c globals.h util.h scan.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

scan.o: scan.c scan.h util.h globals.h
	$(CC) $(CFLAGS) -c scan.c

clean:
	-rm $(OBJS)
	-rm Quary
	-@echo "\n-------------------------------------------\nHi Coder! You clean Quary successfully!\n-------------------------------------------\\n"


