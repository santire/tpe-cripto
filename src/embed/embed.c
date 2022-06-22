#include "embed.h"
#include "../utils/utils.h"

int embed_lsbn(struct t_bmp *bmp, char *message, int message_size, int n);
int extract_lsbn(struct t_bmp *bmp, char **output, int n);

// int embed_lsb1(struct t_bmp *bmp, FILE *out_file, char *message,
//                int message_size) {

//   // In LSB1 I can only change 1 bit per byte
//   printf("Max hideable bits: %d\n", bmp->ih.biSizeImage);
//   printf("Max hideable bytes: %d\n", bmp->ih.biSizeImage / 8);

//   if (message_size > bmp->ih.biSizeImage / 8) {
//     printf("CANT HIDE THIS FILE IN THIS PORTER");
//     return -1;
//   }

//   return 0;
// }

int embed(struct t_embed_params *p, struct t_bmp *bmp) {
  parse_bmp_file(p->porter_file, bmp);

  switch (p->steg_type) {

  case LSB1:
    printf("Embedding with LSB1\n");
    embed_lsbn(bmp, p->secret_data, p->secret_size, 1);
    break;
  case LSB4:
    printf("Embedding with LSB4\n");
    embed_lsbn(bmp, p->secret_data, p->secret_size, 4);
    break;
  case LSBI:
    printf("Embedding with LSBI\n");
    break;

  default:
    return -1;
  }

  return 0;
}

int extract(struct t_extract_params *p, struct t_bmp *bmp) {
  parse_bmp_file(p->porter_file, bmp);

  switch (p->steg_type) {

  case LSB1:
    printf("Extracting with LSB1\n");
    extract_lsbn(bmp, p->output, 1);
    break;
  case LSB4:
    printf("Embedding with LSB4\n");
    extract_lsbn(bmp, p->output, 4);
    break;
  case LSBI:
    printf("Embedding with LSBI\n");
    break;

  default:
    return -1;
  }

  unsigned int message_size = (unsigned int)(**p->output);
  *p->ext = *p->output + message_size + sizeof(unsigned int);
  printf("Extension? %s\n", *p->ext);

  return 0;
}

int embed_lsbn(struct t_bmp *bmp, char *message, int message_size, int n) {
  unsigned char mask = (1 << n) - 1;
  int img_offset = 0;

  for (int i = 0; i < message_size; i++) {
    for (int j = 8 - n; j >= 0; j -= n, img_offset++) {
      bmp->img[img_offset] ^= bmp->img[img_offset] & mask;
      bmp->img[img_offset] |= (message[i] >> j) & mask;
    }
  }

  return 0;
}

int extract_lsbn(struct t_bmp *bmp, char **output, int n) {
  // Calculate max possible output size
  int max_size = (bmp->ih.biSizeImage * n) / 8;
  printf("max size: %d\n", max_size);
  *output = (char *)calloc(max_size, sizeof(unsigned char));

  unsigned char mask = (1 << n) - 1;
  int byte = 0;
  int bit_position = 8 - n;

  for (int i = 0; i < bmp->ih.biSizeImage; i++) {
    (*output)[byte] |= (bmp->img[i] & mask) << bit_position;
    bit_position -= n;
    if (bit_position < 0) {
      byte++;
      bit_position = 8 - n;
    }
  }

  return 0;
}
