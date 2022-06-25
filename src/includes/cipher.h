#ifndef CIPHER_H
#define CIPHER_H
#include "types.h"
#include "utils.h"

struct t_encrypt_params {

  unsigned char **secret_data;
  unsigned int *secret_size;

  enum t_encrypt enc_algorithm;
  enum t_block block_algorithm;
  const char *password;
};

int encrypt(struct t_encrypt_params *p);
int decrypt(struct t_encrypt_params *p);
#endif
