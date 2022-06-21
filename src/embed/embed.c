#include "embed.h"
#include "../utils/utils.h"

int embed(struct t_embed_params *p) {
  char *porter = NULL;
  // int porter_size = read_file(p->porter_file, &porter);
  struct t_bmp bmp;
  parse_bmp_file(p->porter_file, &bmp);

  printf("fM1 = %c, fM2 = %c, bfS = %u, un1 = %hu, un2 = %hu, iDO = %u\n",
         bmp.fh.fileMarker1, bmp.fh.fileMarker2, bmp.fh.bfSize, bmp.fh.unused1,
         bmp.fh.unused2, bmp.fh.imageDataOffset);
  printf("w = %d, h = %d\n", bmp.ih.width, bmp.ih.height);
  for (int i=0; i<bmp.ih.biSizeImage; i++) {
    putc(bmp.img[i], stdout);
  }
  printf("\n");

  switch (p->steg_type) {

  case LSB1:
    printf("Embedding with LSB1\n");
    // printf("Porter size: %d\n", porter_size);
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

  return 0;
}
