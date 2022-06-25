#include "includes/embed.h"
#include "includes/utils.h"

int embed_lsbn(struct t_bmp *bmp, unsigned char *message, int message_size,
               int n);
int embed_lsbi(struct t_bmp *bmp, unsigned char *message, int message_size);

int embed(struct t_embed_params *p, struct t_bmp *bmp) {
  parse_bmp_file(p->porter_file, bmp);
  int error = parse_bmp_file(p->porter_file, bmp);
  if (error < 0) {
    fprintf(stderr, "Error parsing BMP file.\n");
    return error;
  }

  int max_size = 0;

  switch (p->steg_type) {

  case LSB1:
    printf("Embedding with LSB1\n");
    max_size = embed_lsbn(bmp, p->secret_data, p->secret_size, 1);
    break;
  case LSB4:
    printf("Embedding with LSB4\n");
    max_size = embed_lsbn(bmp, p->secret_data, p->secret_size, 4);
    break;
  case LSBI:
    printf("Embedding with LSBI\n");
    max_size = embed_lsbi(bmp, p->secret_data, p->secret_size);
    break;

  default:
    return -1;
  }

  if (max_size != 0) {
    fprintf(stderr, "Error embedding file, max size is %d but file is %d\n",
            max_size, p->secret_size);
    return -1;
  }

  return 0;
}

int embed_lsbn(struct t_bmp *bmp, unsigned char *message, int message_size,
               int n) {

  // Check if bmp is big enough for message
  unsigned int max_size = (bmp->ih.biSizeImage * n) / 8;
  if (message_size > max_size) {
    return max_size;
  }

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

int embed_lsbi(struct t_bmp *bmp, unsigned char *message, int message_size) {

  // Check if bmp is big enough for message
  int max_size = (bmp->ih.biSizeImage - 4) / 8; // First 4 bytes are for pattern
  if (message_size > max_size) {
    return max_size;
  }

  struct t_group groups[4] = {0};
  unsigned char mask = (1 << 1) - 1;
  int img_offset = 4; // First 4 bytes are for pattern

  for (int i = 0; i < message_size; i++) {
    for (int j = 7; j >= 0; j -= 1, img_offset++) {
      int g_i = (bmp->img[img_offset] >> 1) & 0x03;
      struct t_group group = groups[g_i];
      group.count++;
      group.diff += (bmp->img[img_offset] & mask) ^ ((message[i] >> j) & mask);

      // Normal LSB1
      bmp->img[img_offset] ^= bmp->img[img_offset] & mask;
      bmp->img[img_offset] |= (message[i] >> j) & mask;
    }
  }

  // Check invertable patterns
  for (int i = 0; i < 4; i++) {
    struct t_group g = groups[i];
    g.invert = (g.count / 2) < g.diff;
  }

  img_offset = 0;
  for (int i = 0; i < message_size; i++) {
    for (int j = 7; j >= 0; j -= 1, img_offset++) {
      // Setting pattern
      if (img_offset < 4) {
        bmp->img[img_offset] ^= bmp->img[img_offset] & mask;
        bmp->img[img_offset] |= groups[img_offset].invert;
      } else {
        int g_i = (bmp->img[img_offset] >> 1) & 0x03;
        struct t_group group = groups[g_i];
        if (group.invert) {
          bmp->img[img_offset] ^= 0x01;
        }
      }
    }
  }

  return 0;
}
