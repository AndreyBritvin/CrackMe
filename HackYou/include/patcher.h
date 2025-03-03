#ifndef PATCHER_H_
#define PATCHER_H_

#include "utils.h"
#include "stdlib.h"
typedef unsigned long long hash_t;
typedef char byte;

static hash_t original_hash = 8758719067478870283;
static hash_t modified_hash =  758545229006759617;

hash_t hash(void *void_arr_to_hash, size_t arr_size);
err_code_t patch(char *filename);

#endif // PATCHER_H_
