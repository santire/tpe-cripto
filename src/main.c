#include "includes/cipher.h"
#include "includes/embed.h"
#include "includes/extract.h"
#include "includes/utils.h"

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

const char *argp_pogram_bug_address = "sreyes@itba.edu.ar";
const char *argp_program_version = "stegobmp 1.0";

const char *STEG_MODES[] = {"LSB1", "LSB4", "LSBI", 0};
const char *ENCRYPT_MODES[] = {"AES128", "AES192", "AES256", "DES", 0};
const char *BLOCK_MODES[] = {"ECB", "CFB", "OFB", "CBC", 0};

enum t_errors {
  PORTER_NOT_BMP,
  OUTPUT_NOT_BMP,
  PORTER_NO_ACC,
  BOTH_MODES,
  NO_MODE,
  NO_PORTER,
  NO_INPUT,
  NO_OUTPUT,
  NO_STEG,
  MISSING_ARG,
  NO_ERRORS
};

int ERRORS[NO_ERRORS] = {0};
const char *const error_messages[] = {
    [PORTER_NOT_BMP] = "Porter file is not a bitmap",
    [OUTPUT_NOT_BMP] = "Output file is not a bitmap",
    [PORTER_NO_ACC] = "Can't open porter file",
    [BOTH_MODES] = "--extract and --embed are mutually exclusive modes",
    [NO_MODE] = "No mode has been set, use --extract or --embed",
    [NO_PORTER] = "No porter file has been set",
    [NO_INPUT] = "No input file has been set",
    [NO_OUTPUT] = "No output file has been set",
    [NO_STEG] = "Invalid or missing stenography method",
    [MISSING_ARG] = "Missing argument, check if = is present in long option \
(ex: --option=val)",
    [NO_ERRORS] = "",
};

struct arguments {
  int embed_mode;
  int extract_mode;

  // embed mode specific
  const char *secret_file;

  // common options
  const char *porter_file;
  const char *out_file;
  enum t_steg steg_mode;

  // cipher options
  int cypher_mode;
  enum t_encrypt encryption_algorithm;
  enum t_block block_algorithm;
  const char *password;
};
void print_arguments(struct arguments a) {
  printf("###############################\n");
  printf("Arguments: \n");
  printf("Embed mode: %d\n", a.embed_mode);
  printf("Extract mode: %d\n", a.extract_mode);
  printf("Input file: %s\n", a.secret_file);
  printf("Porter file: %s\n", a.porter_file);
  printf("Output file: %s\n", a.out_file);
  printf("Steg mode: %d\n", a.steg_mode);
  printf("cypher mode: %d\n", a.cypher_mode);
  printf("Encryption algorithm: %d\n", a.encryption_algorithm);
  printf("Block algorithm: %d\n", a.block_algorithm);
  printf("Password: %s\n", a.password);
  printf("###############################\n");
}

static int parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *a = state->input;

  switch (key) {
  case ARGP_KEY_INIT: {
    // Default values
    a->embed_mode = 0;
    a->extract_mode = 0;
    a->steg_mode = 0;
    a->cypher_mode = 0;
    a->secret_file = "";
    a->porter_file = "";
    a->out_file = "";
    a->password = "";
    a->encryption_algorithm = AES128;
    a->block_algorithm = CBC;
    break;
  }
  case 991: {
    a->embed_mode = 1;
    break;
  }
  case 992: {
    a->extract_mode = 1;
    break;
  }
  case 'i': {
    if (arg == NULL) {
      ERRORS[MISSING_ARG] = 1;
    } else {
      // TODO: Check if file exists
      a->secret_file = arg;
    }
    break;
  }
  case 'p': {
    if (arg == NULL) {
      ERRORS[MISSING_ARG] = 1;
    } else {
      const char *ext = get_file_ext(arg);
      if (strcmp(ext, "bmp") == 0) {
        FILE *file;
        // Check if file is readable
        if ((file = fopen(arg, "rb"))) {
          fclose(file);
          a->porter_file = arg;
        } else {
          ERRORS[PORTER_NO_ACC] = 1;
        }
      } else {
        ERRORS[PORTER_NOT_BMP] = 1;
      }
    }
    break;
  }
  case 'o': {
    if (arg == NULL) {
      ERRORS[MISSING_ARG] = 1;
    } else {
      // TODO: Check if not exists || is overridable
      a->out_file = arg;
    }
    break;
  }
  case 's': {
    if (arg == NULL) {
      ERRORS[MISSING_ARG] = 1;
    } else {
      int i = 0;
      while (STEG_MODES[i]) {
        if (strcasecmp(STEG_MODES[i], arg) == 0) {
          a->steg_mode = i + 1;
          break;
        }
        i++;
      }
    }
    break;
  }

  case 'a': {
    if (arg == NULL) {
      ERRORS[MISSING_ARG] = 1;
    } else {
      arg = *arg == '=' ? arg + 1 : arg;
      int i = 0;
      while (ENCRYPT_MODES[i]) {
        printf("%s\n", arg);
        if (strcasecmp(ENCRYPT_MODES[i], arg) == 0) {
          printf("%s\n", arg);
          a->encryption_algorithm = i + 1;
          break;
        }
        i++;
      }
    }
    break;
  }

  case 'm': {
    if (arg == NULL) {
      ERRORS[MISSING_ARG] = 1;
    } else {
      arg = *arg == '=' ? arg + 1 : arg;
      int i = 0;
      while (BLOCK_MODES[i]) {
        if (strcasecmp(BLOCK_MODES[i], arg) == 0) {
          a->block_algorithm = i + 1;
          break;
        }
        i++;
      }
    }
    break;
  }

  case 993: {
    if (arg == NULL) {
      ERRORS[MISSING_ARG] = 1;
    } else {
      a->password = arg;
    }
    break;
  }

  case ARGP_KEY_ARG: {
    ERRORS[MISSING_ARG] = 1;
    break;
  }

  case ARGP_KEY_END: {
    int arg_errors = 0;
    // Check all required args have been set
    if (a->embed_mode && a->extract_mode) {
      ERRORS[BOTH_MODES] = 1;
    }
    if (!a->embed_mode && !a->extract_mode) {
      ERRORS[NO_MODE] = 1;
    }
    if (strcmp(a->porter_file, "") == 0) {
      ERRORS[NO_PORTER] = 1;
    }
    if (strcmp(a->out_file, "") == 0) {
      ERRORS[NO_OUTPUT] = 1;
    }
    if (a->steg_mode == 0) {
      ERRORS[NO_STEG] = 1;
    }
    if (a->embed_mode && (strcmp(a->secret_file, "") == 0)) {
      ERRORS[NO_INPUT] = 1;
    }

    if (a->embed_mode && (strcmp(get_file_ext(a->out_file), "bmp") != 0)) {
      ERRORS[OUTPUT_NOT_BMP] = 1;
    }

    if (strcmp(a->password, "") != 0) {
      a->cypher_mode = 1;
    }

    // validate inputs
    for (int i = 0; i < NO_ERRORS; i++) {
      if (ERRORS[i]) {
        arg_errors = 1;
        printf("ERROR: %s\n", error_messages[i]);
      }
    }
    if (arg_errors) {
      argp_usage(state);
    }
    break;
  }

  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

int main(int argc, char **argv) {

  struct argp_option options[] = {
      {0, 0, 0, 0, "Mode Options  (Mutually exclusive):", 1},
      {"embed", 991, 0, 0, "Use program to embed a message in bmp file."},
      {"extract", 992, 0, 0,
       "Use program to extract a message from a bmp file."},
      {0, 0, 0, 0, "Embed Specific Options", 2},
      {"in", 'i', "FILE", 0, "File to embed."},
      {0, 0, 0, 0, "General Options:", 3},
      {"bmp", 'p', "FILE", 0, "Bitmap porter file."},
      {"out", 'o', "FILE", 0,
       "Output file. Bitmap with hidden file if embed mode, extracted file "
       "otherwise."},
      {"steg", 's', "<LSB1 | LSB4 | LSBI>", 0, "Stenography algorithm."},
      {0, 0, 0, 0, "Optional Cypher Options:", 4},
      {0, 0, 0, 0, "Optional arguments require = (ex: -m=ECB)", 5},
      {"cypher", 'a', "AES128 | AES192 | AES256 | DES", OPTION_ARG_OPTIONAL,
       "Cypher algorithm."},
      {"block-mode", 'm', "ECB | CBF | OFB | CBC", OPTION_ARG_OPTIONAL,
       "Block mode."},
      {"pass", 993, "TEXT", OPTION_ARG_OPTIONAL,
       "Cypher password. If not present, other cypher parameters are ignored."},
      {0}};

  struct argp argp = {options, parse_opt, 0,
                      "Hide and extract files from bitmap files using "
                      "different stenography techniques.\v",
                      0};

  struct arguments arguments;

  int retval = argp_parse(&argp, argc, argv, 0, 0, &arguments);
  if (retval != 0) {
    printf("%d\n", retval);
    return retval;
  }

  FILE *porter_file = fopen(arguments.porter_file, "rb");

  if (arguments.embed_mode) {

    FILE *output_file = fopen(arguments.out_file, "wb+");
    unsigned char *secret_message = NULL;
    unsigned int secret_size =
        read_secret_message(arguments.secret_file, &secret_message);

    if (arguments.cypher_mode) {
      printf("Encrypting message...\n");
      // Encrypts message leaving the encrypted message in secret_message
      // and the encrypted size in secret_size
      struct t_encrypt_params encrypt_params = {
          &secret_message, &secret_size, arguments.encryption_algorithm,
          arguments.block_algorithm, arguments.password};
      if (encrypt(&encrypt_params) != 0) {
        // TODO: Error handling
      }
    }

    struct t_embed_params embed_params = {porter_file, secret_message,
                                          secret_size, arguments.steg_mode};
    printf("Embedding file...\n");
    struct t_bmp out_bmp;
    embed(&embed_params, &out_bmp);

    write_bmp_file(output_file, &out_bmp);
    printf("Wrote file %s succesfully\n", arguments.out_file);
    fclose(output_file);

    free(out_bmp.img);
    free(secret_message);
  }

  if (arguments.extract_mode) {
    printf("Extracting file...\n");
    unsigned char *message = NULL;
    unsigned char *output = NULL;
    unsigned char *ext = NULL;
    unsigned int message_size = 0;
    struct t_extract_params extract_params = {
        porter_file, &output, &message_size, arguments.steg_mode};
    struct t_bmp in_bmp;
    extract(&extract_params, &in_bmp);

    if (arguments.cypher_mode) {
      printf("Decrypting message...\n");
      struct t_encrypt_params decrypt_params = {
          &output, &message_size, arguments.encryption_algorithm,
          arguments.block_algorithm, arguments.password};
      if (decrypt(&decrypt_params) != 0) {
        // TODO: Error handling
      }

      message_size = 0;
      unsigned char *o = output;
      message_size |= (o[0] << 24); 
      message_size |= (o[1] << 16);
      message_size |= (o[2] << 8);
      message_size |= (o[3]);
    }

    message = output + sizeof(unsigned int);
    ext = message + message_size;
    char *filename =
        (char *)malloc(strlen(arguments.out_file) + strlen((char *)ext) + 1);
    strcpy(filename, arguments.out_file);
    strcat(filename, (char *)ext);

    FILE *output_file = fopen(filename, "wb+");

    fwrite(message, sizeof(char), message_size, output_file);
    printf("Wrote file %s succesfully\n", filename);
    fclose(output_file);

    free(output);
    free(in_bmp.img);
    free(filename);
  }

  fclose(porter_file);

  return 0;
}
