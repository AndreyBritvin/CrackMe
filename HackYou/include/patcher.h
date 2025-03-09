#ifndef PATCHER_H_
#define PATCHER_H_

#include "utils.h"
#include "stdlib.h"

typedef unsigned long long hash_t;
typedef char byte;

static hash_t original_hash = -7977139799940870391;
static hash_t modified_hash = -8804680756655524609;

typedef err_code_t (*patcher_t)(char *);

hash_t hash(void *void_arr_to_hash, size_t arr_size);
err_code_t patch(char *filename);

#endif // PATCHER_H_
