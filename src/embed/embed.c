#include "embed.h"
#include "../utils/utils.h"

// TODO: Error handling

int embed_lsb1(struct t_bmp *bmp, FILE *out_file, char *message,
               int message_size) {

  // In LSB1 I can only change 1 bit per byte
  printf("Max hideable bits: %d\n", bmp->ih.biSizeImage);
  printf("Max hideable bytes: %d\n", bmp->ih.biSizeImage / 8);

  if (message_size > bmp->ih.biSizeImage / 8) {
    printf("CANT HIDE THIS FILE IN THIS PORTER");
    return -1;
  }

  for (int i=0; i < message_size; i++) {
    // bit = agarro el i-esimo bit de *message
    // reemplazo el ultimo bit del i-esimo byte de bmp->img por bit
  }

  return 0;
}

int embed(struct t_embed_params *p) {
  struct t_bmp bmp;
  parse_bmp_file(p->porter_file, &bmp);

  switch (p->steg_type) {

  case LSB1:
    printf("Embedding with LSB1\n");
    embed_lsb1(&bmp, p->output_file, p->secret_data, p->secret_size);
    break;
  case LSB4:
    printf("Embedding with LSB4\n");
    break;
  case LSBI:
    printf("Embedding with LSBI\n");
    break;

  default:
    return -1;
  }

  free(bmp.img);
  return 0;
}
