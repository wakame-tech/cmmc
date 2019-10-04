%{
#include <stdio.h>
#include <stdlib.h>
// #define YYDEBUG 1

int yylex();
int yyerror(const char * s);
%}

%union {
  char * name;
  int val;
}

%token CONST VAR IF THEN BGN END WHILE READ WRITE WRITELN DO FOR FUN
%token SEMICOLON COMMA DOT
%token NUMBER
%token IDENT
%token ADD SUB MUL DIV ASN EQL NOTEQ LT GT
%token L_PAREN R_PAREN L_BRACKET R_BRACKET L_SQBRACKET R_SQBRACKET

%type <val> code main functions function parameter_list block stmts stmt expr binary_op
%type <val> IDENT READ WRITE WRITELN
%type <val> NUMBER

%left '*' '/' '%'
%left '+' '-' 
%left '<' '>' '==' '!='
%left ':='

%%
code
  : functions main {
    printf("%d %d\n", $1, $2);
    $$ = 0;
  }
  | main {
    printf("OPR 0");
    $$ = 0;
  }

main
  : block DOT { $$ = 0; }

functions
  : function { $$ = 0; }
  | functions function { $$ = 0; }

function
  : FUN IDENT L_PAREN parameter_list R_PAREN block
  {
    printf("%d\n", $2);
    $$ = 0;
  }

parameter_list
  : IDENT { $$ = 0; }
  | parameter_list COMMA IDENT { $$ = 0; }

block
  : BGN stmts END
  {
    $$ = 0;
  }

stmts
  : stmt { $$ = 0; }
  | stmts stmt { $$ = 0; }
  ;

stmt
  : expr SEMICOLON
  { $$ = $1; }

binary_op
  : ADD { $$ = 2; }
  | SUB { $$ = 3; }
  | MUL { $$ = 4; }
  | DIV { $$ = 5; }
  | LT { $$ = 6; }
  | GT { $$ = 7; }
  | LT EQL { $$ = 8; }
  | GT EQL { $$ = 9; }

expr
  : expr binary_op expr
  {
    printf("LIT %d\n", $1);
    printf("LIT %d\n", $3);
    printf("OPR %d\n", $2);
    $$ = 0;
  }
  | L_PAREN expr R_PAREN
  {
    printf("(%d)", $2);
    $$ = 0;
  }
  | READ {
    printf("read\n");
    $$ = 0;
  }
  | WRITE {
    printf("write\n");
    $$ = 0;
  }
  | WRITELN {
    printf("writeln\n");
    $$ = 0;
  }
  | IDENT {
    printf("%d\n", $1);
    $$ = $1;
  }
  | NUMBER
  ;
%%

int yyerror(const char * s) {
  extern char * yytext;
  fprintf(stderr, "parser error near %s\n", yytext);
  return 0;
}

int main(int argc, char * argv[]) {
  extern FILE * yyin;

  printf("[Compile] %s\n", argv[1]);
  if((yyin = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Error!\n");
    exit(1);
  }
  yyparse();
  fclose(yyin);
}
