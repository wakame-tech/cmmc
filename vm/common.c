/* ---------------------- common.c ------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef NEED_MALLOC
#include <malloc.h>
#endif

/* library functions for PL/0 compiler */
/* print error message */
void error(s) char *s; {
  printf( "pl0: %s\n", s );
}

/* save string s */
char * strsave(s) char *s; {
  char *p;
    
  if((p = (char *)malloc(strlen(s) + 1)) == NULL) {
    perror("malloc");
    exit(1);
  }
  return strcpy(p, s);
}

/* clear array a */
void cleararray(a, s) char *a; int s; {
  for(int i = 1; i <= s; i++) *a++ = '\0';
}
