#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>

#define SAFE_OPEN_FILE(file_ptr, filename, mode)                                   \
                        file_ptr = fopen(filename, mode);                    \
                        if (file_ptr == NULL)                                      \
                        {                                                          \
                            printf("Unable open output file '%s'\n", filename);    \
                            return ERROR_FILE;                                     \
                        }
#define SAFE_CALLOC(var_name, type, size) type *var_name = (type *) calloc(size, sizeof(type)); \
                                          if (var_name == NULL)                                 \
                                          {                                                     \
                                            return ERROR_CALLOC_IS_NULL;                        \
                                          }

// #define PRINT_ERROR(...) fprintf(stderr, __VA_ARGS__);

#define LOG(...) if (LOG_FILE != NULL) fprintf(LOG_FILE, __VA_ARGS__)

// header utils
typedef int err_code_t;

enum errors
{
    OK,
    ERROR_FILE,
    ERROR_TREE_IS_NULL,
    ERROR_TREE_LOOPED,
    ERROR_PARENT_DONT_MATCH,
    ERROR_CALLOC_IS_NULL,
    ERROR_CANT_GET_NEXT_NODE,
    ERROR_FILENAME_IS_EMPTY,
    PATCHED,
    WRONG_FILE,
    ALREADY_PATCHED,
};

#endif //UTILS_H_
