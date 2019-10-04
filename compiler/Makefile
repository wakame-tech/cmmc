CC	= cc

pl0c	: y.tab.c lex.yy.c
	$(CC) y.tab.c lex.yy.c -ly -o pl0c

y.tab.c	: parser.y
	yacc -dv -y parser.y

lex.yy.c	: lexer.l
	lex -l lexer.l

clean:
	rm -f *~ *.o pl0c
