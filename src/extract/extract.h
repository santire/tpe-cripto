#ifndef EXTRACT_H
#define EXTRACT_H
#include "../types.h"
#include "../utils/utils.h"

struct t_extract_params {
  FILE *porter_file;

  unsigned char **output;
  unsigned char **ext;

  unsigned int *message_size;

  enum t_steg steg_type;
};

int extract(struct t_extract_params *p, struct t_bmp *bmp);
#endif
