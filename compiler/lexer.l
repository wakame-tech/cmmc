%{
#include <stdio.h>
#include "y.tab.h"

int yywrap() {
  return 1;
}
%}

%%
"const" return CONST;
"var" return VAR;
"if" return IF;
"then" return THEN;
"begin" return BGN;
"end" return END;
"while" return WHILE;
"read" return READ;
"write" return WRITE;
"writeln" return WRITELN;
"do" return DO;
"for" return FOR;
"function" return FUN;

";" return SEMICOLON;
"," return COMMA;
"." return DOT;

"+" return ADD;
"-"	return SUB;
"*" return MUL;
"/" return DIV;
":=" return ASN;
"=" return EQL;
"!=" return NOTEQ;
"<" return LT;
">" return GT;

"(" return L_PAREN;
")" return R_PAREN;
"{" return L_BRACKET;
"}" return R_BRACKET;
"[" return L_SQBRACKET;
"]" return R_SQBRACKET;
[ \n\t]

0|[1-9][0-9]* {
  yylval.val = atoi(yytext);
  return NUMBER;
}

[a-zA-Z]([a-zA-Z0-9])* {
  yylval.name = yytext;
  return IDENT;
}

%%
