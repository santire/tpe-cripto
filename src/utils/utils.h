#ifndef UTILS_H
#define UTILS_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// https://stackoverflow.com/questions/17967480/parse-read-bitmap-file-in-c

typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned char fileMarker1;                                                                                                                                                                                              
    unsigned char fileMarker2;                                                                                                                                                                                               
    unsigned int   bfSize;                                                                                                                                                                                                                   
    uint16_t unused1;                                                                                                                                                                                                                        
    uint16_t unused2;                                                                                                                                                                                                                        
    unsigned int   imageDataOffset;                                                                                                                                                            
} FILEHEADER;                                                                                                                                                                                                                                

typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned int   biSize;                                                                                                                                                                                                                   
    int            width;                                                                                                                                                                
    int            height;                                                                                                                                                                     
    uint16_t planes;                                                                                                                                                                                                                         
    uint16_t bitPix;                                                                                                                                                                                                                         
    unsigned int   biCompression;                                                                                                                                                                                                            
    unsigned int   biSizeImage;                                                                                                                                                                                                              
    int            biXPelsPerMeter;                                                                                                                                                                                                          
    int            biYPelsPerMeter;                                                                                                                                                                                                          
    unsigned int   biClrUsed;                                                                                                                                                                                                                
    unsigned int   biClrImportant;                                                                                                                                                                                                           
} INFOHEADER;                                                                                                                                                                                                                                

typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned char  b;                                                                                                                                                                                                                        
    unsigned char  g;                                                                                                                                                                                                                        
    unsigned char  r;                                                                                                                                                                                                                        
} IMAGE;

struct t_bmp {
  FILEHEADER fh;
  INFOHEADER ih;
  unsigned char *img;
};

// Returns file extension.
// TODO: Probably should change to (const char *filename, char *dest) -> int
const char *get_file_ext(const char *filename);

int parse_bmp_file(FILE *fp, struct t_bmp *bmp);
// Reads message to hide and loads it into memory
// as (Size (4 bytes) || message || extension). Returns size.
int read_secret_message(const char *filename, char **data);

// Reads message to hide and loads it into memory. Returns size.
int read_file(FILE *fp, char **destination);
#endif
