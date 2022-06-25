#include "includes/embed.h"
#include "includes/utils.h"

int embed_lsbn(struct t_bmp *bmp, unsigned char *message, int message_size, int n);
int embed_lsbi(struct t_bmp *bmp, unsigned char *message, int message_size);

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

int embed_lsbn(struct t_bmp *bmp, unsigned char *message, int message_size, int n) {
  unsigned char mask = (1 << n) - 1;
  int img_offset = 0;

  for (int i = 0; i < message_size; i++) {
    for (int j = 8 - n; j >= 0; j -= n, img_offset++) {
      int combination = (bmp->img[img_offset] >> 1) & 0x03;

      bmp->img[img_offset] ^= bmp->img[img_offset] & mask;
      bmp->img[img_offset] |= (message[i] >> j) & mask;
    }
  }

  return 0;
}

int embed_lsbi(struct t_bmp *bmp, unsigned char *message, int message_size) {
  unsigned char mask = (1 << 1) - 1;
  int img_offset = 0;

  for (int i = 0; i < message_size; i++) {
    for (int j = 7; j >= 0; j -= 1, img_offset++) {
      bmp->img[img_offset] ^= bmp->img[img_offset] & mask;
      bmp->img[img_offset] |= (message[i] >> j) & mask;
    }
  }

  return 0;
}
