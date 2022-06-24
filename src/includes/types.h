#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum t_steg { LSB1 = 1, LSB4, LSBI };
enum t_encrypt { AES128 = 1, AES192, AES256, DES };
enum t_block { ECB = 1, CFB, OFB, CBC };

// https://stackoverflow.com/questions/17967480/parse-read-bitmap-file-in-c
// https://stackoverflow.com/questions/14279242/read-bitmap-file-into-structure

typedef struct __attribute__((__packed__)) {
  unsigned char fileMarker1;
  unsigned char fileMarker2;
  unsigned int bfSize;
  uint16_t unused1;
  uint16_t unused2;
  unsigned int imageDataOffset;
} FILEHEADER;

typedef struct __attribute__((__packed__)) {
  unsigned int biSize;
  int width;
  int height;
  uint16_t planes;
  uint16_t bitPix;
  unsigned int biCompression;
  unsigned int biSizeImage;
  int biXPelsPerMeter;
  int biYPelsPerMeter;
  unsigned int biClrUsed;
  unsigned int biClrImportant;
} INFOHEADER;

typedef struct __attribute__((__packed__)) {
  unsigned char b;
  unsigned char g;
  unsigned char r;
} IMAGE;

struct t_bmp {
  FILEHEADER fh;
  INFOHEADER ih;
  unsigned char *img;
};
#endif
