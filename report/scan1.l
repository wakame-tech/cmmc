%%
[\t ]+ {}
"+"|"-"|"*"|"/"|"=" {
  printf("%s (operator) ", yytext);
}

";" {
  printf("%s (separator)", yytext);
}

[a-z][a-z0-9]* {
  printf("%s (identifier) ", yytext);
}

-?([1-9][0-9]*|0)(\.[0-9]+)?(e[+-]?[1-9][0-9]*)? {
  printf("%s (number) ", yytext);
}

0[xX][A-Fa-f0-9]+(\.[A-Fa-f0-9]+)? {
  printf("%s (hex) ", yytext);
}

. {
  printf("%s (don't know) ", yytext);
}
%%
