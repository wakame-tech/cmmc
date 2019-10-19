CC	= cc

pl0c	: y.tab.c code.o env.o
	$(CC) y.tab.c code.o env.o -ly -ll -o pl0c

y.tab.c	: parser.y lex.yy.c
	yacc -dv -y parser.y

code.o	: code.c code.h
	$(CC) -c code.c

env.o	: env.c env.h
	$(CC) -c env.c

lex.yy.c	: lexer.l
	lex -l lexer.l

clean:
	rm -f *~ *.o pl0c
