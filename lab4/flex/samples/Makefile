CC = gcc

LL = lex

CFLAGS = -w

OBJS = sample1 sample2 sample3 sample4
OBJSC = sample1.c sample2.c sample3.c sample4.c

all: $(OBJS)

sample1: sample1.c
	$(CC) $(CFLAGS) -o sample1 sample1.c

sample2: sample2.c
	$(CC) $(CFLAGS) -o sample2 sample2.c

sample3: sample3.c
	$(CC) $(CFLAGS) -o sample3 sample3.c

sample4: sample4.c
	$(CC) $(CFLAGS) -o sample4 sample4.c

yaccs: $(OBJSC)

sample1.c: sample1.L
	$(LL) -o sample1.c sample1.L

sample2.c: sample2.L
	$(LL) -o sample2.c sample2.L

sample3.c: sample3.L
	$(LL) -o sample3.c sample3.L

sample4.c: sample4.L
	$(LL) -o sample4.c sample4.L 

clean:
	-rm $(OBJS)
	-rm $(OBJSC)

