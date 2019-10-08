CC	= cc

pl0c	: y.tab.c lex.yy.c
	$(CC) lex.yy.c y.tab.c -ly -o pl0c

y.tab.c	: parser.y
	yacc -dv -y parser.y

lex.yy.c	: lexer.l
	lex -l lexer.l

clean:
	rm -f *~ *.o pl0c
