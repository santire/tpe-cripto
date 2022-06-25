#include "includes/extract.h"
#include "includes/utils.h"

int extract_lsbn(struct t_bmp *bmp, unsigned char **output, int n);
int extract_lsbi(struct t_bmp *bmp, unsigned char **output);

int extract(struct t_extract_params *p, struct t_bmp *bmp) {
  int error = parse_bmp_file(p->porter_file, bmp);
  if (error < 0) {
    fprintf(stderr, "Error parsing BMP file.\n");
    return error;
  }

  switch (p->steg_type) {

  case LSB1:
    printf("Extracting with LSB1\n");
    extract_lsbn(bmp, p->output, 1);
    break;
  case LSB4:
    printf("Extracting with LSB4\n");
    extract_lsbn(bmp, p->output, 4);
    break;
  case LSBI:
    printf("Extracting with LSBI\n");
    extract_lsbi(bmp, p->output);
    break;

  default:
    return -1;
  }

  unsigned int message_size = 0;
  unsigned char *o = *p->output;

  message_size |= (o[0] << 24);
  message_size |= (o[1] << 16);
  message_size |= (o[2] << 8);
  message_size |= (o[3]);

  *p->message_size = message_size;
  return 0;
}

int extract_lsbn(struct t_bmp *bmp, unsigned char **output, int n) {
  // Calculate max possible output size
  int max_size = (bmp->ih.biSizeImage * n) / 8;
  *output = (unsigned char *)calloc(max_size, sizeof(unsigned char));

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

int extract_lsbi(struct t_bmp *bmp, unsigned char **output) {

  // Calculate max possible output size
  int max_size = (bmp->ih.biSizeImage) / 8;
  *output = (unsigned char *)calloc(max_size, sizeof(unsigned char));

  unsigned char mask = (1 << 1) - 1;
  int byte = 0;
  int bit_position = 7;

  int invert[4] = {0};
  for (int i = 0; i < bmp->ih.biSizeImage; i++) {
    if (i < 4) {
      invert[i] |= bmp->img[i] & mask;
    } else {
      int g_i = (bmp->img[i] >> 1) & 0x03;
      if (invert[g_i]) {
        (*output)[byte] |= (0x01 ^ (bmp->img[i] & mask)) << bit_position;
      } else {
        (*output)[byte] |= (bmp->img[i] & mask) << bit_position;
      }
      bit_position -= 1;
      if (bit_position < 0) {
        byte++;
        bit_position = 7;
      }
    }
  }

  return 0;
}
