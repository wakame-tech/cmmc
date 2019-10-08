%union {
  char * name;
  int val;
  /* enum op_type_e */ int optype;
}

%{
#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"
// #define YYDEBUG 1

int yylex();
int yyerror(const char * s);

int seek = 0;

%}

%token CONST VAR IF THEN BGN END WHILE READ WRITE WRITELN DO FOR FUN
%token SEMICOLON COMMA DOT
%token NUMBER
%token IDENT
%token ADD SUB MUL DIV ASN EQL NOTEQ LT GT
%token L_PAREN R_PAREN L_BRACKET R_BRACKET L_SQBRACKET R_SQBRACKET

%type <val> code blocks block parameter_list stmts stmt expr
%type <optype> binary_op
%type <val> READ WRITE WRITELN
%type <val> NUMBER
%type <name> IDENT

%left '*' '/' '%'
%left '+' '-' 
%left '<' '>' '==' '!='
%left ':='

%%
code
  : blocks DOT {
    printf("end\n");
    $$ = 0;
  }

blocks
  : block
  | blocks block

block
  : FUN IDENT L_PAREN parameter_list R_PAREN stmts
  {
    printf("fun : %s\n", $2);

    $$ = 0;
  }
  | stmts {
    printf("> block\n");
  }

parameter_list
  : IDENT { $$ = 0; }
  | parameter_list COMMA IDENT { $$ = 0; }

stmts
  : stmt
  | stmts stmt
  ;

stmt
  : expr SEMICOLON { $$ = $1; }
  | IDENT ASN expr { $$ = 0; }
  | BGN stmts END { printf("> begin\n"); $$ = $2; }
  | IF expr THEN { printf("if"); $$ = ++seek; } stmt {}
  | WRITE expr { printf("write\n"); $$ = 0; }

binary_op
  : ADD { $$ = Add; }
  | SUB { $$ = Sub; }
  | MUL { $$ = Mul; }
  | DIV { $$ = Div; }
  | LT { $$ = Lt; }
  | GT { $$ = Gt; }
  | LT EQL { $$ = Leq; }
  | GT EQL { $$ = Geq; }

expr
  : expr binary_op expr
  {
    printf("op %d\n", $2);
    $$ = ++seek;
  }
  | L_PAREN expr R_PAREN { $$ = $2; }
  | READ {
    printf("read\n");
    $$ = ++seek;
  }
  | WRITELN {
    printf("writeln\n");
    $$ = ++seek;
  }
  | IDENT { printf("%s\n", $1); $$ = ++seek; }
  | NUMBER {
    printf("lit %d\n", $1);
  }
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
  return 0;
}
