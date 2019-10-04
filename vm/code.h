/* ---------------------- code.h ------------------------ */

/* data types of code */

#define CMAX 10000

#define concat3(t1,t2,t3) concat2(t1, concat2(t2,t3))
#define concat4(t1,t2,t3,t4) concat2(t1, concat3(t2,t3,t4))
#define concat5(t1,t2,t3,t4,t5) concat2(t1, concat4(t2,t3,t4,t5))
#define concat6(t1,t2,t3,t4,t5,t6) concat2(t1, concat5(t2,t3,t4,t5,t6))

/* operation codes of PL/0 code */
typedef enum {
  O_LIT, O_OPR, O_LOD,
  O_STO, O_CAL, O_INT,
  O_JMP, O_JPC, O_CSP,
  O_LAB, O_BAD, O_RET
} opecode;

/* sub-operations of arithmetic operation etc. */
typedef enum {
  P_RET, P_NEG, P_ADD, P_SUB, P_MUL, P_DIV, P_ODD, P_MOD,
  P_EQ, P_NE, P_LT, P_GE, P_GT, P_LE
} oprcode;

/* sub-operations of I/O operation */
typedef enum { RDI, WRI, WRL } cspcode;

typedef enum { ADD, SUB, MUL, DIV } mathope;

/* an instruction of PL/0 code */
typedef struct {
  opecode f; /* operation code */
  int l; /* difference of static nesting levels, or 0 */
  int a; /* displacement (offset), or address, or sub-operation, or number */
} instruction;

/* the PL/0 code, represented as a list structure */
typedef struct codelist {
  instruction inst;      /* an instruction */
  struct codelist *next; /* pointer to the next cell */
} codestr; 

/* type of attribute code */
typedef struct {
  codestr *head; /* beginning of code */
  codestr *tail; /* end of code */
} codeptr; 

/* mnemonic code */
typedef struct {
  char *sym; 
  int cd;
} mnemonic; 

extern int label[];
extern int cl;
extern instruction code[];
extern int cx;

extern int genlabel();
extern codeptr nullcode();
extern codeptr gen();
extern codeptr concat2();
extern codeptr printcode();
extern void linearize(instruction c[]);
