#ifndef _CODE_H_
#define _CODE_H_

#include <stdio.h>
#include <stdlib.h>

typedef
struct CODE {
  struct CODE *next;
  int f, l, a;
} code;

typedef struct CPTR {
  code *h;
  code *t;
} cptr;

/* operation codes of PL/0 code */
typedef enum { 
  O_LIT, O_OPR, O_LOD, O_STO, O_CAL, O_INT, 
  O_JMP, O_JPC, O_CSP, O_LAB, O_BAD, O_RET
} opecode;
/* mnemonic code */
typedef struct {
  char *sym; 
  int cd;
} mnemonic;


int makelabel();
cptr* makecode(int f, int l, int a);
cptr* mergecode(cptr* c1, cptr* c2);
void  printcode(FILE* f, cptr* c);

#endif
