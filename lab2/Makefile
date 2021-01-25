CC = g++

CFLAGS = 

OBJS = main.o deterNFA.o minDFA.o global.o

all: main
	 @echo "\n-------------------------------------------\nHi Coder! You compiled successfully!\n-------------------------------------------\\n"

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main

global.o: 
	$(CC) $(CFLAGS) -c global.cpp

minDFA.o: 
	$(CC) $(CFLAGS) -c minDFA.cpp

deterNFA.o: 
	$(CC) $(CFLAGS) -c deterNFA.cpp

main.o: 
	$(CC) $(CFLAGS) -c main.cpp

clean:
	-rm $(OBJS)
	-rm main
	-@echo "\n-------------------------------------------\nHi Coder! You clean successfully!\n-------------------------------------------\\n"