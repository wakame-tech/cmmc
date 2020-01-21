CC	= cc
OPTS = -std=c99 -lm

pl0vm	: inter.o  common.o
	$(CC) $(OPTS) inter.o  common.o -o pl0vm

inter.o	: inter.c
	$(CC) $(OPTS) -c inter.c

common.o	: common.c
	$(CC) $(OPTS) -c common.c

clean:	
	rm -f *~ *.o vm pl0vm
