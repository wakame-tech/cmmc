/* ---------------------- common.c ------------------------- */
#include <stdio.h>
#include <string.h>
#ifdef NEED_MALLOC
#include <malloc.h>
#endif

	/* library functions for PL/0 compiler */
void error( s )  /* print error message */
  char *s;
{
    printf( "pl0: %s\n", s );
}

char *strsave( s )  /* save string s */
  char *s;
{
    char *p;
    
    if(( p = (char *)malloc( strlen( s ) + 1 )) == NULL ) {
	perror( "malloc" );
	exit( 1 );
    }
    return strcpy( p, s );
}

void cleararray( a, s )  /* clear array a */
  char *a;
  int s;
{
    int i;
    
    for( i=1; i<=s; i++ ) *a++ = '\0';
}
