#include <stdio.h>


typedef struct {
  char *name;
  FILE *p;
  char *mode;
  int status;
} File;
