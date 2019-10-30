/* -------------------------- inter.c ------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "code.h"

int is_debug = 0;

#define BSIZE 50
/* global variables for interpreter */
#define STACKSIZE 500
int s[STACKSIZE]; /* run-time stack */
int p, b, t;      /* program counter, pointer to stack, top of stack */

int label[BSIZE];		/* label table for backpatching */
int cl = 0;			/* current label number using O_LAB */ 

instruction code[CMAX];		/* code table             */
instruction code2[CMAX];	/* non labeled code table */
int cx  = 1;			/* code index for code    */
int cx2 = 1;			/* code index for code2   */

opecode mnemonic2i(char * f);
instruction getcode(char * buf);

static void wrcode(int c, instruction c2[]);

/* table for mnemonic code */
mnemonic mntbl[] = {
  { "LIT", O_LIT }, { "OPR", O_OPR },
  { "LOD", O_LOD }, { "STO", O_STO },
  { "CAL", O_CAL }, { "INT", O_INT },
  { "JMP", O_JMP }, { "JPC", O_JPC },
  { "CSP", O_CSP }, { "LAB", O_LAB },
  { "   ", O_BAD }, { "RET", O_RET },
  { "DST", O_DST }, { "DLD", O_DLD }
};

/* trace static link l times, where l is the difference */
/* of current and referenced static nesting levels */
int base(l) int l; {
  int b1;
  b1	= b; /* b points to the area of static link */
  while(l > 0) {
	  b1 = s[b1];
	  l--;
  }
  return b1;
}

void stack_dump(int f, int l, int a) {
  char * ope = "   ";
  for (int i = 0; i < 14; i++) {
    if (mntbl[i].cd == f) {
      ope = mntbl[i].sym;
      break;
    }
  }
  printf("@ L%d (%s, %d, %d) t = %d stack [ ", p, ope, l, a, t);
  b = base(l);
  for (int i = 1; i < t; i++) {
    if (b <= i && i < b + 3) {
      printf("_ ");
    } else {
      if (i == b) printf("| ( ");
      printf("%d ", s[i]);
    }
  }
  printf("]\n");
}

/* interpreter */
void interpreter() {
  int tmp;
  int r; // for dynamic load/store
  instruction i;
  opecode f; /* operation code */
  /* l = level difference or 0 a = displacement or sub-operation or number */
  int l, a;
  if (is_debug) printf( "start PL/0\n" );
  t = 0;  b = 1;  p = 1;
  s[1] = s[2] = s[3] = 0; /* static link, dynamic link, ret. addr. of main */

  do {
    i = code[p++]; /* get an instruction */
    f = i.f;  l = i.l;  a = i.a;
    switch(f) {
      case O_LIT : s[++t] = a;  break;
      case O_OPR : 
        switch((oprcode)a) {
          case P_RET: t = b - 1;  p = s[t+3];	 b = s[t+2];  break;
          case P_NEG:	   ;  s[t] = -s[t]		   ;  break;
          case P_ADD: --t;  s[t] = s[t] + s[t+1]	   ;  break; 
          case P_SUB: --t;  s[t] = s[t] - s[t+1]	   ;  break; 
          case P_MUL: --t;  s[t] = s[t] * s[t+1]	   ;  break; 
          case P_DIV: --t;  s[t] = s[t] / s[t+1]	   ;  break; 
          case P_ODD:	   ;  s[t] = s[t] % 2		   ;  break;
          case P_MOD: --t;  s[t] = s[t] % s[t+1];  break;
          case P_EQ:	--t;  s[t] = ( s[t] == s[t+1] )	   ;  break;
          case P_NE:	--t;  s[t] = ( s[t] != s[t+1] )	   ;  break;
          case P_LT:	--t;  s[t] = ( s[t] <  s[t+1] )	   ;  break;
          case P_GE:	--t;  s[t] = ( s[t] >= s[t+1] )	   ;  break;
          case P_GT:	--t;  s[t] = ( s[t] >  s[t+1] )	   ;  break;
          case P_LE:	--t;  s[t] = ( s[t] <= s[t+1] )	   ;  break;
          case P_AND: --t;  s[t] = ( s[t] && s[t+1] )    ;  break;
          case P_OR:  --t;  s[t] = ( s[t] || s[t+1] )    ;  break;
          case P_NOT:    ;  s[t] = s[t] ? 0 : 1       ;  break;
        }
        break;
      case O_LOD:
        s[++t] = s[base(l) + a];
        break;
      case O_DLD:
        // pop stack top
        r = s[t--];
        if (t <= r) {
          printf("warning index out of range %d <= %d\n", t, r);
        }
        if (is_debug) printf("[%d + (3)]\n", r);
        // dynamic load
        s[++t] = s[base(l) + r + 3];

        if (is_debug) stack_dump(f, l, a);

        break;
      case O_STO:
        s[base(l) + a] = s[t--];
        break;
      case O_DST:
        // pop stack top
        r = s[t--];
        if (t <= r) {
          printf("warning index out of range %d <= %d\n", t, r);
        }

        if (is_debug) printf("[%d + (3)]\n", r);
        // dynamic store
        s[base(l) + r + 3] = s[t--];

        if (is_debug) stack_dump(f, l, a);
        break;
      case O_CAL:
        s[t+1] = base(l) /* static link */ ;
        s[t+2] = b /* dynamic link */;
        s[t+3] = p /* ret. addr. */;
        b = t + 1; p = a;	break;
      case O_INT: t += a; break;
      case O_RET:
        tmp = s[t];
        t = b - 1;
        p = s[t+3];
        b = s[t+2];
        t -= a;
        s[++t] = tmp;
        break;
      case O_JMP: p = a;  break;
    case O_JPC: if( s[t--] == 0 ) p = a;  break;
      case O_CSP:
        switch((cspcode)a) {
          case RDI: /* read(var) */
            if(scanf("%d",&s[++t]) != 1) printf( "error read\n" );
            break;
          case WRI: /* write(exp) */
            printf("%d", s[t--]);
            break;
          case WRL: /* writeln */
            putchar('\n');
            break;
          }
        break;
      default: break;
    }

    /* stack dump */
    if (is_debug) {
      stack_dump(f, l, a);
    }
  } while( p != 0 );

  if (is_debug) printf( "end PL/0\n" );
}

opecode mnemonic2i(char * f){
  for(int i = 0; i < 14; i++){
    if (strcmp(mntbl[i].sym, f) == 0) {
      return mntbl[i].cd;
    }
  }

  return O_BAD;
}

// make instruction from a line [improved]
instruction getcode(char * buf) {
  char * stripped = calloc(BSIZE, sizeof(char));
  char * mne = calloc(4, sizeof(char));
  char * level = calloc(BSIZE, sizeof(char));
  char * value = calloc(BSIZE, sizeof(char));

  // strip spaces
  int cnt, pos = 0;
  for (cnt = 0; buf[pos] != '\n'; pos++) {
    if (buf[pos] == ' ' || buf[pos] == '(')
      continue;
    if (buf[pos] == ')')
      break;

    stripped[cnt++] = buf[pos];
  }
  stripped[cnt] = '\0';

  // seek values
  pos = 0;
  for (cnt = 0; stripped[pos] != ',' && stripped[pos] != '\0'; pos++, cnt++) {
    mne[cnt] = stripped[pos];
  }
  mne[cnt] = '\0';

  for (cnt = 0, pos++; stripped[pos] != ',' && stripped[pos] != '\0'; pos++, cnt++) {
    level[cnt] = stripped[pos];
  }
  level[cnt] = '\0';
  cnt = 0;

  for (cnt = 0, pos++; stripped[pos] != '\n' && stripped[pos] != '\0'; pos++, cnt++) {
    value[cnt] = stripped[pos];
  }
  value[cnt] = '\0';

  instruction i = { .f = mnemonic2i(mne), .l = atoi(level), .a = atoi(value) };

  // printf("%d, %d, %d\n", i.f, i.l, i.a);

  free(stripped);
  free(mne);
  free(level);
  free(value);

  return i;
}

/* print code stored in array code[] */
static void wrcode(int c, instruction c2[]) {
  int i, f, l, a;
  
  if (is_debug) printf( "code table\n\n" );

  for(i = 1; i <= c; i++) {
    f = c2[i].f;
    l = c2[i].l;
    a = c2[i].a;
    if (is_debug) printf( "%4d [ %s, %3d, %3d ]\n", i, mntbl[f].sym, l, a );
  }
  if (is_debug) printf("\f");
}

/* transform list structure code cp to array code[] */
/* also do backpatching */

/* codeptr */
void linearize(instruction c[]) {
  codestr *wp;
  opecode f;
  int l, a;
  int c1, c2;
  int tmpcx;

  cleararray(code, sizeof(code));
  cleararray(label, sizeof(label));
  if(cx2 == 1) return;
  for(tmpcx = 1; tmpcx != cx2; tmpcx++) {
    f = code2[tmpcx].f;
    l = code2[tmpcx].l;
    a = code2[tmpcx].a;

    switch(f) {
      case O_LAB:
        /* label 'a' was unused */
        if( label[a] == 0 ) {
          if (is_debug) printf("label %d is %d\n", a, cx);
          /* let it be defined. let label[a] point to its address */
          label[a] = cx;
        } else if (label[a] < 0) {
          /* label 'a' was used before def. */
          /* backpatching */
          if (is_debug) printf("label %d is backpatching\n", a);
          c1 = -label[a];
          /* trace forward reference chain */
          while(c1 != 0) {
            c2 = code[c1].a;
            code[c1].a = cx;
            c1 = c2;
          }
          label[a] = cx;
        } else {
          error("label already defined");
          exit(1);
        }
        /* do not increment cx (delete LAB instruction) */
        break;
      case O_JMP:
      case O_JPC:
      case O_CAL:
        code[cx].f = f;
        code[cx].l = l;
        /* label 'a' was undefined. make forward reference chain */
        if(label[a] <= 0) {
          code[cx].a = -label[a];
          label[a] = -cx;
        } else {
          /* label 'a' was defined */
          code[cx].a = label[a];
        }
        cx++;
        break;
      default:
        code[cx].f = f;
        code[cx].l = l;
        code[cx].a = a;
        cx++;
        break;
    }
    if(cx >= CMAX) {
      error("program is too large");
      exit(1);
    }
  }
  wrcode(cx - 1, code);
}

int main(int argc, char * argv[]) {
  FILE * codef;
  char buf[BSIZE];

  codef = fopen(argv[1], "r");
  if(codef == NULL){
    perror("code.output");
    exit(EXIT_FAILURE);
  }

  if (argc >= 3 && strcmp(argv[2], "-d") == 0) {
    is_debug = 1;
  }

  while(fgets(buf, BSIZE, codef) != 0){
    // for skip empty line
    if (buf[0] == '\n' || buf[0] == '#') continue;
    code2[cx2] = getcode(buf);
    cx2++;
    // printf("%d %s\n", cx2, buf);
  }

  if (ferror(codef) != 0) {
    perror("getcode");
    exit(EXIT_FAILURE);
  }

  if (fclose(codef) != 0) {
    perror("code.output");
    exit(EXIT_FAILURE);
  }

  linearize(code2);

  interpreter();
}