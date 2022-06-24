#ifndef EMBED_H
#define EMBED_H
#include "../types.h"
#include "../utils/utils.h"

struct t_embed_params {
  FILE *porter_file;

  char *secret_data;
  int secret_size;

  enum t_steg steg_type;
};

int embed(struct t_embed_params *p, struct t_bmp *bmp);
#endif
