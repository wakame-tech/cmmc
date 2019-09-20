%{
#include <stdio.h>
#include "y.tab.h"

int yywrap() {
  return 1;
}
%}

%%
"+" { return ADD; }
"-"	{ return SUB; }
[\n]

[1-9][0-9]* {
    yylval.val = atoi(yytext);
    return INT_LITERAL;
}

%%
