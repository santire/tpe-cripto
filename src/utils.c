#include "includes/byteswap.h"
#include "includes/utils.h"

const char *get_file_ext(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return "";
  return dot + 1;
}

unsigned int read_file(FILE *fp, unsigned char **destination) {
  unsigned int buff_size = -1;

  if (fp != NULL) {
    // Go to the end of the file
    if (fseek(fp, 0L, SEEK_END) == 0) {
      // Get the size of the file
      buff_size = ftell(fp);
      if (buff_size == -1) {
        fprintf(stderr, "Error reading file\n");
        return 0;
      }

      // Allocate our buffer to that size
      *destination = malloc(sizeof(char) * (buff_size));

      // Go back to the beginning of the file
      if (fseek(fp, 0L, SEEK_SET) != 0) {
        return -1;
      }

      if (ferror(fp) != 0) {
        fprintf(stderr, "Error reading file\n");
      }

      if (*destination) {
        fread(*destination, 1, buff_size, fp);
      }
    }
  }

  return buff_size;
}

unsigned int read_secret_message(const char *filename, unsigned char **data) {
  const char *ext = strrchr(filename, '.');
  unsigned char *buffer = NULL;
  unsigned int buff_size = 0;
  unsigned int be_buff_size = 0;
  FILE *fp = fopen(filename, "rb");
  int byte_size = sizeof(unsigned int);

  buff_size = read_file(fp, &buffer);
  if (buff_size == 0) {
    free(buffer);
    fclose(fp);
    return 0;
  }

  int num = 1; // 00 01
  if (*(char *)&num == 1) {
    // Little Endian
    be_buff_size = bswap_32(buff_size);
  } else {
    // Big Endian
    be_buff_size = buff_size;
  }

  int ext_size = strlen(ext) + 1; // one for \0
  int message_size = byte_size + buff_size + ext_size;

  *data = (unsigned char *)calloc(message_size, sizeof(char));

  memcpy(*data, &be_buff_size, byte_size);
  memcpy((*data) + byte_size, buffer, buff_size);
  memcpy((*data) + byte_size + buff_size, ext, ext_size);

  free(buffer);
  fclose(fp);

  return message_size;
}

int parse_bmp_file(FILE *fp, struct t_bmp *bmp) {

  fread(&(bmp->fh), sizeof(unsigned char), sizeof(FILEHEADER), fp);

  // Verify that this is a .BMP file
  if (bmp->fh.fileMarker1 != 'B' || bmp->fh.fileMarker2 != 'M') {
    fprintf(stderr, "File is not a valid BMP\n");
    return -1;
  }

  fread(&(bmp->ih), sizeof(unsigned char), sizeof(INFOHEADER), fp);

  // Verify no compression
  if (bmp->ih.biCompression != 0) {
    fprintf(stderr, "Porter BMP file must not be compressed\n");
    return -1;
  }

  // Move fp to the beginning of bitmap data
  fseek(fp, bmp->fh.imageDataOffset, SEEK_SET);

  // Allocate enough memory for the bitmap image data
  bmp->img = (unsigned char *)malloc(bmp->ih.biSizeImage);

  // Verify memory allocation
  if (!bmp->img) {
    free(bmp->img);
    fprintf(stderr, "Couldn't allocate memory to load porter bmp\n");
    return -1;
  }

  // Read image into the bitmap image data
  fread(bmp->img, bmp->ih.biSizeImage, 1, fp);

  return 0;
}

int write_bmp_file(FILE *fp, struct t_bmp *bmp) {
  fwrite(&bmp->fh, 1, sizeof(FILEHEADER), fp);
  fwrite(&bmp->ih, 1, sizeof(INFOHEADER), fp);
  fwrite(bmp->img, bmp->ih.biSizeImage, sizeof(unsigned char), fp);

  return 0;
}
