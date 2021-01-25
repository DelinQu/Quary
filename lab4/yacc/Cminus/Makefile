BISON = bison
LEX = flex
CC = gcc -w

TARGET = myMinusComplier
OBJS = main.o util.o cminus.tab.c lex.yy.c analyze.o symtab.o

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)  -ll

main.o: main.c globals.h util.h scan.h cminus.tab.h analyze.h
	$(CC) -o $@ -c main.c

util.o: util.c util.h globals.h cminus.tab.h
	$(CC) -o $@ -c util.c

analyze.o: analyze.c analyze.h globals.h symtab.h
	$(CC) -o $@ -c analyze.c

symtab.o: symtab.c symtab.h globals.h
	$(CC) -o $@ -c symtab.c

lex.yy.c: cminus.l globals.h util.h scan.h cminus.tab.h
	$(LEX) -w cminus.l

cminus.tab.h cminus.tab.c: cminus.y globals.h util.h scan.h
	$(BISON) -d -v cminus.y

clean:
	-rm -rf *.o lex.yy.c myMinusComplier cminus.tab.h cminus.tab.c cminus.output
