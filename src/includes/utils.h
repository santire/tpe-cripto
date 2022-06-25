#ifndef UTILS_H
#define UTILS_H
#include "types.h"

// Returns file extension.
// TODO: Probably should change to (const char *filename, char *dest) -> int
const char *get_file_ext(const char *filename);

int parse_bmp_file(FILE *fp, struct t_bmp *bmp);
int write_bmp_file(FILE *fp, struct t_bmp *bmp);

// Reads message to hide and loads it into memory
// as (Size (4 bytes) || message || extension). Returns size.
unsigned int read_secret_message(const char *filename, unsigned char **data);

// Reads message to hide and loads it into memory. Returns size.
unsigned int read_file(FILE *fp, unsigned char **destination);
#endif
