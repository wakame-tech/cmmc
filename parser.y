%{
#include <stdio.h>
#include <stdlib.h>
#define YYDEBUG 1
%}

%union {
  char * name;
  int val;
}

%token CONST VAR IF THEN BGN END WHILE READ WRITE WRITELN DO FUN

%token <val> NUMBER
%token IDENT
%token ADD SUB MUL DIV ASN EQL LT GT LEQ GEQ
%token L_PAREN R_PAREN L_BRACKET R_BRACKET
%type <val> expr stmt

%left '*' '/'
%left '+' '-' 
%left '<' '>' '>=' '<='
%left ':='

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
    printf("(%d + %d)\n", $1, $3);
    $$ = $1 + $3;
  }
  | expr SUB expr
  {
    printf("(%d - %d)\n", $1, $3);
    $$ = $1 - $3;
  }
  | expr MUL expr
  {
    printf("(%d * %d)\n", $1, $3);
    $$ = $1 * $3;
  }
  | expr DIV expr
  {
    printf("(%d / %d)\n", $1, $3);
    $$ = $1 / $3;
  }
  | L_PAREN expr R_PAREN
  {
    $$ = $2;
  }
  | IDENT
  | NUMBER
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
