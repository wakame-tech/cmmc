CC	= cc
OPTS = -std=c99

cmmc	: y.tab.c code.o env.o
	$(CC) $(OPTS) y.tab.c code.o env.o -ly -ll -o cmmc

y.tab.c	: parser.y lex.yy.c
	yacc -dv parser.y

code.o	: code.c code.h
	$(CC) $(OPTS) -c code.c

env.o	: env.c env.h
	$(CC) $(OPTS) -c env.c

lex.yy.c	: lexer.l
	lex -l lexer.l

clean:
	rm -f *~ *.o a.out cmmc lex.yy.c y.*
