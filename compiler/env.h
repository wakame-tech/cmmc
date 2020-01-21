#ifndef _ENV_H_
#define _ENV_H_

#define SYSTEM_AREA 3

typedef
struct LIST {
  // variable name
  char        *name;
  // VARIABLE
  int          kind;
  // offset
  int          a;
  // level
  int          l;
  // ?
  int          params;
  // length if var is array, must be positive
  int length;

  struct LIST *prev;
} list;


list* search_block(char*);
list* search_all(char*);
list* searchf(int);
void add_label(int n, char * label);
int search_label(char * label);
void addlist(char*, int, int, int, int, int);
void delete_block();

list* gettail();
void initialize();

void make_params(int n_of_ids, int label);

int vd_backpatch(int n_of_vars, int offset);

void sem_error1(char* kind);
void sem_error2(char* kind);
void sem_error3(char* s, int n1, int n2);


enum KIND { VARIABLE, BLOCK, FUNC, CONSTANT };

#endif
