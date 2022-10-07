#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./common/memo.c"

#define FILEMODE "r"

typedef char *string;

typedef struct{
   queue_t *queue;
} program_t;