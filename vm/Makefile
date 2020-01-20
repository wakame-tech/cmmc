CC	= cc

pl0vm	: inter.o  common.o
	$(CC) inter.o  common.o -o pl0vm

inter.o	: inter.c
	$(CC) -c inter.c

common.o	: common.c
	$(CC) -c common.c

clean:	
	rm -f *~ *.o vm
