all:
	bison calculator.Y -o calculator.tab.c
	gcc -g calculator.tab.c -o calculator -w
clean:
	-rm calculator.tab.c
	-rm calculator