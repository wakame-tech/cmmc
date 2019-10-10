CC	= cc

a.out	: y.tab.c lex.yy.c
	$(CC) y.tab.c -ll -ly

y.tab.c	: ex1.y
	yacc -dv -y ex1.y

lex.yy.c	: ex1.l
	lex -l ex1.l

clean:
	rm -f lex.yy.c y.tab.c *~ *.o a.out
