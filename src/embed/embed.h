#ifndef EMBED_H
#define EMBED_H
#include <stdio.h>

enum t_steg { LSB1 = 1, LSB4, LSBI };

struct t_embed_params {
  FILE *porter_file;
  FILE *output_file;

  const char *secret_data;
  const int secret_size;

  enum t_steg steg_type;
};

int embed(struct t_embed_params *params);
#endif
