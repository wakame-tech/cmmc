%{

#define ECHO printf("%s", yytext);

%}

%%
"var" return VAR;
"main" return MAIN;
"if" return IF;
"then" return THEN;
"else" return ELSE;
"begin" return BGN;
"while" return WHILE;
"do" return DO;
"for" return FOR;
"end" return END;
"read" return READ;
"write" return WRITE;
"writeln" return WRITELN;
"return" return RETURN;
"goto" return GOTO;
"label" return LABEL;

";" return SEMICOLON;
"," return COMMA;

"++" return INC;
"--" return DEC;
"+" return PLUS;
"-"	return MINUS;
"*" return MULT;
"/" return DIV;
":=" return ASN;
"%" return MOD;
"^" return POW;
"==" return EQ;
"!=" return NE;
"<" return LT;
">" return GT;
"<=" return LE;
">=" return GE;
"&&" return AND;
"||" return OR;
"!" return NOT;

"(" return L_PAREN;
")" return R_PAREN;
"{" return L_BRACKET;
"}" return R_BRACKET;
"[" return L_SQBRACKET;
"]" return R_SQBRACKET;
[ \n\t]+

#(.*)$

0|[1-9][0-9]* {
  yylval.val = atoi(yytext);
  return NUMBER;
}

[a-z][a-z0-9]* {
  char * n = calloc(strlen(yytext) + 1, sizeof(char));
  strcpy(n, yytext);
  // printf("var %s\n", n);
  yylval.name = n;
  return IDENT;
}

. return yytext[0];
