#ifndef EMBED_H
#define EMBED_H
#include "../utils/utils.h"
#include <stdio.h>

enum t_steg { LSB1 = 1, LSB4, LSBI };

struct t_embed_params {
  FILE *porter_file;

  char *secret_data;
  int secret_size;

  enum t_steg steg_type;
};

struct t_extract_params {
  FILE *porter_file;

  unsigned char **output;
  unsigned char **ext;
  
  unsigned int *message_size;

  enum t_steg steg_type;
};

int embed(struct t_embed_params *p, struct t_bmp *bmp);
int extract(struct t_extract_params *p, struct t_bmp *bmp);
#endif
