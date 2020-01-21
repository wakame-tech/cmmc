#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"

list *h, *t;

void addlist(char* name, int kind, int offset, int level, int fparam, int length){
  list *tmp;

  tmp = (list*)malloc(sizeof(list));
  if (tmp == NULL){
    perror("memory allocation");
    exit(EXIT_FAILURE);
  }

  printf("%s kind = %d length = %d\n", name, kind, length);

  tmp->name   = name;
  tmp->kind   = kind;
  tmp->a      = offset;
  tmp->l      = level;
  tmp->params = fparam;
  tmp->length = length;

  tmp->prev = t;
  t = tmp;
}

list *search_all(char* name){
  list *tmp;

  for(tmp=t;
      tmp != h;
      tmp = tmp->prev){
    if (strcmp(name, tmp->name) == 0){
      return tmp;
    }
  }

  return NULL;
}

list *search_block(char* name){
  list *tmp;

  for(tmp=t;
      tmp != h && tmp->kind != BLOCK;
      tmp = tmp->prev){
    if (strcmp(name, tmp->name)==0){
      return tmp;
    }
  }
  
  return NULL;
}

// label and index dict
struct label_t { int k; char * v; };
struct label_t labels[100];
int labels_i = 0;

void add_label(int n, char * label) {
  struct label_t l = { .k = n, .v = label };
  labels[labels_i++] = l;
}

int search_label(char * label) {
  for (int i = 0; i < labels_i; i++) {
    if (strcmp(labels[i].v, label) == 0)  {
      return labels[i].k;
    }
  }
  return -1;
}

void delete_block(){
  list *tmp;

  for(tmp=t;
      tmp != h && tmp->kind != BLOCK;
      tmp = tmp->prev){
    free(tmp->name);
    free(tmp);
  }
  
  free(tmp);
  
  t = tmp->prev;
}

list* searchf(int l){
  list *tmp;

  for(tmp = t;
      tmp != NULL;
      tmp = tmp->prev){
    if (tmp->kind == FUNC && tmp->l == l){
      return tmp;
    }
  }
  return tmp;
}

void initialize(){

  /* initialize the list */
  h = (list*)malloc(sizeof(list));
  if (h == NULL){
    perror("memory allocation");
    exit(EXIT_FAILURE);
  }


  t = h;
  h->prev = NULL;
  h->name = "";
}

list* gettail(){
  return t;
}


void make_params(int n_of_ids, int label){
  list* tmp = gettail();
  int i;
  
  for(i = 1; i<n_of_ids; i++){
    tmp->a = 0 - i ;
    tmp = tmp->prev;
  }

  for(;tmp->kind != FUNC; tmp = tmp->prev){
  }
  tmp->params = n_of_ids - 1;
  tmp->a = label;
}

// return total memory size
int vd_backpatch(int n_of_vars, int offset){
  list * tmp = gettail();
  
  int cnt = 0;
  for(int i = 0; i < n_of_vars; i++) {
    if (tmp == NULL) {
      printf("[Internal Compile Error] backpaching failure\n");
      return -1;
    }
    printf("%s { .a = %d .length = %d }\n", tmp->name, SYSTEM_AREA + offset + cnt, tmp->length);
    tmp->a = SYSTEM_AREA + offset + cnt;
    // for array
    cnt += tmp->length;
    tmp = tmp->prev;
  }

  return  cnt;
}

void sem_error1(char* kind){
  fprintf(stderr,
	  "this identifier has"
	  " been already declared(%s)!\n", kind);
  exit(EXIT_FAILURE);
}

void sem_error2(char* kind){
  fprintf(stderr,
	  "this identifier(%s) has not been declared!\n",
	  kind);
  exit(EXIT_FAILURE);
}

void sem_error3(char* s, int n1, int n2){
  fprintf(stderr,
	  "the number of parameters does not match "
	  "as the declaration(%s : %d -> %d)\n",
	  s, n1, n2);
  exit(EXIT_FAILURE);
}
