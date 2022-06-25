#include "includes/cipher.h"
#include "includes/types.h"
#include <byteswap.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/evp.h>

const EVP_CIPHER *get_cipher_type(enum t_encrypt enc_algorithm,
                                  enum t_block block_algorithm);
int get_key_size(enum t_encrypt enc_algorithm);

int encrypt(struct t_encrypt_params *p) {
  // TODO: validate input maybe?
  int len;
  int ciphertext_len;
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    printf("Couldn't allocate ctx\n");
    return -1;
  }
  int key_size = get_key_size(p->enc_algorithm);
  if (key_size <= 0) {
    printf("Something went wrong\n");
  }

  unsigned char *key = malloc(key_size + EVP_MAX_IV_LENGTH);
  unsigned char *iv = malloc(key_size);

  if (!key || !iv) {
    printf("Couldn't allocate key or iv\n");
    return -1;
  }


  const EVP_CIPHER *cipher_type =
      get_cipher_type(p->enc_algorithm, p->block_algorithm);

  if (1 != EVP_BytesToKey(cipher_type, EVP_sha256(), NULL,
                          (const unsigned char *)p->password,
                          strlen(p->password), 1, key, iv)) {
    ERR_print_errors_fp(stderr);
  };

  int ciphertext_size = *p->secret_size + AES_BLOCK_SIZE + EVP_MAX_IV_LENGTH;
  unsigned char *new_message =
      (unsigned char *)malloc(ciphertext_size + sizeof(unsigned int));

  if (!new_message) {
    printf("Couldn't allocate memory\n");
    return -1;
  }
  unsigned char *ciphertext = new_message + sizeof(unsigned int);

  // Initialize encription operation
  if (1 != EVP_EncryptInit_ex(ctx, cipher_type, NULL, key, iv)) {
    ERR_print_errors_fp(stderr);
  }
  /*
   * Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, *p->secret_data,
                             (int)*p->secret_size)) {

    ERR_print_errors_fp(stderr);
  }
  ciphertext_len = len;

  /*
   * Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
    ERR_print_errors_fp(stderr);
  }

  ciphertext_len += len;
  EVP_CIPHER_CTX_free(ctx);
  free(key);
  free(iv);

  // Free old plaintext message
  free(*p->secret_data);

  // Assign new encrypted message and size
  unsigned int be_ciphertext_len = 0;
  int num = 1;
  if (*(char *)&num == 1) {
    // Little Endian
    be_ciphertext_len = __bswap_32(ciphertext_len);
  } else {
    // Big Endian
    be_ciphertext_len = ciphertext_len;
  }

  memcpy(new_message, &be_ciphertext_len, sizeof(unsigned int));
  // Assign new encrypted message and size
  *p->secret_data = new_message;
  *p->secret_size = ciphertext_len + sizeof(unsigned int);
  return 0;
}
int decrypt(struct t_encrypt_params *p) {
  unsigned char *ciphertext = *p->secret_data + sizeof(unsigned int);
  int ciphertext_len = *p->secret_size;

  // TODO: validate input maybe?
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    printf("Couldn't allocate ctx\n");
    return -1;
  }
  int key_size = get_key_size(p->enc_algorithm);
  if (key_size <= 0) {
    printf("Something went wrong\n");
  }

  unsigned char *key = malloc(key_size + EVP_MAX_IV_LENGTH);
  unsigned char *iv = malloc(key_size);

  if (!key || !iv) {
    printf("Couldn't allocate key or iv\n");
    return -1;
  }

  const EVP_CIPHER *cipher_type =
      get_cipher_type(p->enc_algorithm, p->block_algorithm);

  if (1 != EVP_BytesToKey(cipher_type, EVP_sha256(), NULL,
                          (const unsigned char *)p->password,
                          strlen(p->password), 1, key, iv)) {
    ERR_print_errors_fp(stderr);
  };

  unsigned char *buffer = malloc(ciphertext_len);
  if (!buffer) {
    printf("Couldn't allocate memory\n");
    return -1;
  }

  int len;
  int decrypted_len = 0;

  if (1 != EVP_DecryptInit_ex(ctx, cipher_type, NULL, key, iv)) {
    ERR_print_errors_fp(stderr);
  }

  if (1 !=
      EVP_DecryptUpdate(ctx, buffer, &len, ciphertext, ciphertext_len)) {
    ERR_print_errors_fp(stderr);
  }
  decrypted_len = len;

  if (1 != EVP_DecryptFinal_ex(ctx, buffer + len, &len)) {
    ERR_print_errors_fp(stderr);
  }

  decrypted_len += len;
  EVP_CIPHER_CTX_free(ctx);
  free(key);
  free(iv);

  // Free old message
  free(*p->secret_data);

  *p->secret_data = buffer;
  *p->secret_size = decrypted_len;

  return 0;
}

const EVP_CIPHER *get_cipher_type(enum t_encrypt enc_algorithm,
                                  enum t_block block_algorithm) {
  switch (block_algorithm) {
  case ECB:
    switch (enc_algorithm) {
    case DES:
      return EVP_des_ecb();
    case AES128:
      return EVP_aes_128_ecb();
    case AES192:
      return EVP_aes_192_ecb();
    case AES256:
      return EVP_aes_256_ecb();
    }

  case CBC:
    switch (enc_algorithm) {
    case DES:
      return EVP_des_cbc();
    case AES128:
      return EVP_aes_128_cbc();
    case AES192:
      return EVP_aes_192_cbc();
    case AES256:
      return EVP_aes_256_cbc();
    }

  case OFB:
    switch (enc_algorithm) {
    case DES:
      return EVP_des_ofb();
    case AES128:
      return EVP_aes_128_ofb();
    case AES192:
      return EVP_aes_192_ofb();
    case AES256:
      return EVP_aes_256_ofb();
    }

  case CFB:
    switch (enc_algorithm) {
    case DES:
      return EVP_des_cfb();
    case AES128:
      return EVP_aes_128_cfb();
    case AES192:
      return EVP_aes_192_cfb();
    case AES256:
      return EVP_aes_256_cfb();
    }
  }

  // TODO: Throw error if enc or block algorithm mismatch
  return NULL;
}
int get_key_size(enum t_encrypt enc_algorithm) {
  switch (enc_algorithm) {
  case AES256:
    return 256 / 8;
  case AES192:
    return 192 / 8;
  case AES128:
  case DES:
    return 128 / 8;
  }

  // TODO: Throw error if enc  algorithm mismatch
  return -1;
}
