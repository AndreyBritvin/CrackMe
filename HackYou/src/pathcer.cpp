#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utils.h"
#include "patcher.h"

err_code_t patch(char *filename)
{
    assert(filename);

    FILE* SAFE_OPEN_FILE(to_crack, filename, "rb+");

    fseek(to_crack, 0, SEEK_END);
    unsigned long filesize = ftell(to_crack);
    rewind(to_crack);

    SAFE_CALLOC(bin_code, char, filesize);
    fread(bin_code, filesize, sizeof(char), to_crack);
    fseek(to_crack, 0, 0);

    hash_t file_hash = hash(bin_code, filesize);
    printf("Hash of this code is %lld\n", file_hash);

    if (file_hash == original_hash)
    {
        printf("Well, now Im gonna crack all your files...\n");
        bin_code[0x1f] = 0xEB;
        fwrite(bin_code, sizeof(char), filesize, to_crack);
    }
    else if (file_hash == modified_hash)
    {
        printf("This file has been already patched\n");
    }
    else
    {
        printf("This is not file that I need to crack\n");
    }

    free(bin_code);
    fclose(to_crack);

    return EXIT_SUCCESS;
}

hash_t hash(void *void_arr_to_hash, size_t arr_size)
{
    assert(void_arr_to_hash != NULL);

    hash_t hash = 8765;
    byte *arr_to_hash = (byte *) void_arr_to_hash;

    for(size_t i = 0; i < arr_size; i++)
    {
        hash = ((hash << 5) + hash) + (hash_t) arr_to_hash[i]; /* hash * 33 + c */
    }

    return hash;
}
