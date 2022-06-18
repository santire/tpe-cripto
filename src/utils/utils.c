#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *get_file_ext(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return "";
  return dot + 1;
}

int read_secret_message(const char *filename, char **data) {
  const char *ext = strrchr(filename, '.');
  char *source = NULL;
  int buff_size = -1;
  FILE *fp = fopen(filename, "r");

  if (fp != NULL) {
    // Go to the end of the file
    if (fseek(fp, 0L, SEEK_END) == 0) {
      // Get the size of the file
      buff_size = ftell(fp);
      if (buff_size == -1) {
        // TODO: Better error handling
        fputs("Error reading file", stderr);
        return -1;
      }

      // Allocate our buffer to that size
      source = malloc(sizeof(char) * (buff_size));

      // Go back to the beginning of the file
      if (fseek(fp, 0L, SEEK_SET) != 0) {
        // TODO: Better error handling
        fputs("Error reading file", stderr);
        free(source);
        return -1;
      }

      if (ferror(fp) != 0) {
        fputs("Error reading file", stderr);
      }
    }
  }

  int ext_size = strlen(ext) + 1; // one for \0
  int message_size = sizeof(int) + buff_size + ext_size;

  *data = malloc(sizeof(char) * message_size);

  memcpy(*data, &buff_size, sizeof(int));
  memcpy((*data)+sizeof(int), source, buff_size);
  memcpy((*data)+sizeof(int)+buff_size, ext, ext_size);

  free(source);
  return message_size;
}
