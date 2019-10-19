#ifndef _ENV_H_
#define _ENV_H_

#define SYSTEM_AREA 3

typedef
struct LIST {
  char        *name;
  int          kind;
  int          a;
  int          l;
  int          params;
  struct LIST *prev;
} list;


list* search_block(char*);
list* search_all(char*);
list* searchf(int);
void addlist(char*, int, int, int, int);
void delete_block();

list* gettail();
void initialize();

void make_params(int n_of_ids, int label);

void vd_backpatch(int n_of_vars, int offset);

void sem_error1(char* kind);
void sem_error2(char* kind);
void sem_error3(char* s, int n1, int n2);


enum KIND { VARIABLE, BLOCK, FUNC, CONSTANT };

#endif
