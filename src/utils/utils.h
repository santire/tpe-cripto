#ifndef UTILS_H
#define UTILS_H
#include <string.h>
const char *get_file_ext(const char *filename);

// Reads message to hide and loads it into memory. Returns size.
int read_secret_message(const char *filename, char **data);
#endif
