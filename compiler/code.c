#include <stdio.h>
#include <stdlib.h>

#include "code.h"

/* table for mnemonic code */
mnemonic mntbl[] = {
  { "LIT", O_LIT }, { "OPR", O_OPR },
  { "LOD", O_LOD }, { "STO", O_STO },
  { "CAL", O_CAL }, { "INT", O_INT },
  { "JMP", O_JMP }, { "JPC", O_JPC },
  { "CSP", O_CSP }, { "LAB", O_LAB },
  { "   ", O_BAD }, { "RET", O_RET }
};


cptr *makecode(int f, int l, int a){
  code *tmp;
  cptr *tmp2;

  tmp = (code*)malloc(sizeof(code));
  if (tmp == NULL){
    perror("memory allocation");
    exit(EXIT_FAILURE);
  }

  tmp->f = f;
  tmp->l = l;
  tmp->a = a;
  tmp->next = NULL;

  tmp2 = (cptr*)malloc(sizeof(cptr));
  if (tmp == NULL){
    perror("memory allocation");
    exit(EXIT_FAILURE);
  }
  
  tmp2->h = tmp2->t = tmp;

  return tmp2;
}


cptr* mergecode(cptr* c1, cptr* c2){

  if (c1 ==  NULL){
    return c2;
  }

  if (c2 == NULL){
    return c1;
  }
  
  c1->t->next = c2->h;
  c1->t = c2->t;
    
  free(c2);
    
  return c1;
}

void printcode(FILE* f, cptr* c){
  code* tmp;

  for(tmp=c->h; tmp != NULL; tmp = tmp->next){
    fprintf(f, "( %s, %4d, %4d )\n",
	    mntbl[tmp->f].sym, tmp->l, tmp->a);
  }
}

int makelabel(){
  static int x = 0;

  x++;
  return x;
}
