CC = gcc

CFLAGS = -w

OBJS = main.o util.o scan.o parse.o

all: QuaryParse
	@echo "\n-----------------------------------------------"
	@echo "Hi Coder! You compiled QuaryParse successfully!"
	@echo "=> Welcome to QuaryParse! <="
	@echo "-----------------------------------------------"

QuaryParse: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o QuaryParse

main.o: main.c globals.h util.h scan.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

scan.o: scan.c scan.h util.h globals.h
	$(CC) $(CFLAGS) -c scan.c

parse.o: parse.c parse.h scan.h globals.h util.h
	gcc -c parse.c -o parse.o

clean:
	-rm $(OBJS)
	-rm QuaryParse
	-@echo "\n-----------------------------------------------"
	-@echo "Hi Coder! You clean QuaryParse successfully!"
	-@echo "-----------------------------------------------"


