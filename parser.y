%{
#include <stdio.h>
#include <stdlib.h>
#define YYDEBUG 1
%}

%union {
  int val;
}

%token <val> INT_LITERAL
%token ADD SUB
%type <val> expr stmt

%left '+' '-' 
%left '*' '/'

%%
stmts
  : stmt
  | stmts stmt
  ;

stmt
  : expr

expr
  : expr ADD expr
  {
    printf("ADD %d %d", $1, $3);
    $$ = $1 + $3;
  }
  | expr SUB expr
  {
    printf("SUB %d, %d", $1, $3);
    $$ = $1 - $3;
  }
  | INT_LITERAL
  ;                 
%%

int yyerror(char const *str) {
  extern char *yytext;
  fprintf(stderr, "parser error near %s\n", yytext);
  return 0;
}

int main(int argc, char * argv[]) {
  extern FILE *yyin;

  printf("[Compile] %s\n", argv[1]);
  if((yyin = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Error ! Error ! Error !\n");
    exit(1);
  }
  yyparse();
  fclose(yyin);
}
