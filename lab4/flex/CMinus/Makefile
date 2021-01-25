
scanner.exe: main.o lex.yy.o util.o
	gcc main.o lex.yy.o util.o -o scanner -lfl
main.o: main.c globals.h util.h
	gcc main.c -c -w
util.o: util.c util.h globals.h
	gcc util.c -c -w
lex.yy.o: cminus.l
	flex cminus.l
	gcc lex.yy.c -c -w
clean: 
	-rm main.o
	-rm lex.yy.o
	-rm util.o
	-rm scanner
