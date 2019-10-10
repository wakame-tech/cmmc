%token LPAREN RPAREN
%token PLUS MINUS
%token MULT DIV
%token MOD POW
%token NUM NL

%left '*' '/' '%' '^'
%left '+' '-' 
%left '(' ')'
%%

s   : ls
    ;

ls  : ls line
    | line
    ;

line  : e NL { printf("%d\n", $1); }
      ;

e : e PLUS e { $$ = $1 + $3; }
  | e MINUS e { $$ = $1 - $3; }
  | e MULT e { $$ = $1 * $3; }
  | LPAREN e RPAREN { $$ = $2; }
  | e DIV e { $$ = $1 / $3; }
  | e MOD e { $$ = $1 % $3; }
  | e POW e { $$ = (int)pow((double)$1, (double)$3); }
  | e { $$ = $1; }
  | NUM { $$ = $1; }
  ;
%%

#include "lex.yy.c"

main(){
  yyparse();
}
