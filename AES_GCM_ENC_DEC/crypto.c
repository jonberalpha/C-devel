/* ------------------------------------------------------------ *
 * file:        crypto.c                                        *
 * ------------------------------------------------------------ */

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define IV_SIZE 16
#define KEY_SIZE 32
#define SALT_SIZE 8

#define AAD_SIZE 10
#define TAG_SIZE 16

#define CIPHER EVP_aes_256_gcm()
#define HASH_ALG EVP_sha256()


void printHex(unsigned char *text, unsigned int length)
{

  int i = 0;
  printf("value:  ");
  for(i = 0; i < length; i++)
  {
    printf("%02x", text[i]);
  }
  printf("\n");
}

void convertNumber(char *buffer, unsigned char *out, unsigned int size)
{
  for(size_t i = 0, j = 0; j < size; i += 2, j++)
    out[j] = (buffer[i] % 32 + 9) % 25 * 16 + (buffer[i + 1] % 32 + 9) % 25;
}

// get password from user
void getPassword(char *password)
{
  struct termios term, term_orig;

  // turn of echo
  tcgetattr(STDIN_FILENO, &term);
  term_orig = term;
  term.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);

  // get password from user
  printf("Please enter a password: ");
  scanf("%s", password);
  printf("\n");

  // Remember to set back, or your commands won't echo!
  tcsetattr(STDIN_FILENO, TCSANOW, &term_orig);
}

// read the header if there is one, fix IV and salt length for simplicity
int readHeader(FILE *fp, unsigned char *IV, unsigned char *salt, unsigned char *aad, unsigned char *tag)
{
  char buffer[1024] = { 0 };
  int bflen = 1024;

  // Read first line of file
  fgets(buffer, bflen, fp);

  if(strcmp(buffer, "---BEGIN HEAD---\n") == 0)
  { // true if there is an header
    // read IV
    fgets(buffer, bflen, fp);

    convertNumber(buffer, IV, IV_SIZE);
    fgets(buffer, bflen, fp);
    convertNumber(buffer, salt, SALT_SIZE);
    fgets(buffer, bflen, fp);
    convertNumber(buffer, aad, AAD_SIZE);
    fgets(buffer, bflen, fp);
    convertNumber(buffer, tag, TAG_SIZE);
    // read ---END HEAD--- to move the file pointer ahead.
    fgets(buffer, bflen, fp);
    return 1;
  }
  // no header, reset pointer
  rewind(fp);
  return 0;
}

void writeHeader(FILE *fileout, unsigned char *IV, unsigned char *salt, unsigned char *aad, unsigned char *tag)
{
  int i = 0;
  // write header to output file
  fprintf(fileout, "---BEGIN HEAD---\n");
  // write IV
  for(i = 0; i < IV_SIZE; i++)
  {
    fprintf(fileout, "%02x", IV[i]);
  }
  fprintf(fileout, "\n");
  // write salt
  for(i = 0; i < SALT_SIZE; i++)
  {
    fprintf(fileout, "%02x", salt[i]);
  }
  fprintf(fileout, "\n");
  for(i = 0; i < AAD_SIZE; i++)
  {
    fprintf(fileout, "%02x", aad[i]);
  }
  fprintf(fileout, "\n");
  for(i = 0; i < TAG_SIZE; i++)
  {
    fprintf(fileout, "%02x", tag[i]);
  }
  fprintf(fileout, "\n");
  fprintf(fileout, "---END HEAD---\n");
}

// generate random IV and salt
void generateIVandSalt(unsigned char *IV, unsigned char *salt, unsigned char *aad)
{
  // Create random IV (initialization vector)
  RAND_bytes(IV, IV_SIZE);
  // Create random salt
  RAND_bytes(salt, SALT_SIZE);
}

// derive AES key from a password
void deriveKey(char *password, int pwdlen, unsigned char *salt, unsigned char *key)
{
  PKCS5_PBKDF2_HMAC_SHA1(password, pwdlen, salt, sizeof(salt), 1, KEY_SIZE, key);
}


void encryptFile(FILE *filein, FILE *fileout, unsigned char *IV,
  unsigned char *salt, unsigned char *key, unsigned char *aad, unsigned char *tag)
{
  // general some good information on https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption
  unsigned char buffer_pt[1024];
  unsigned char buffer_ct[1024];

  EVP_CIPHER_CTX *ctx;

  int len = 0;
  int ciphertext_len = 0;
  int nr_read = 0;

  while(!feof(filein))
  {
    nr_read = fread(buffer_pt, 1, 1024, filein);

    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, IV);
    EVP_EncryptUpdate(ctx, buffer_ct, &len, buffer_pt, nr_read);

    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, buffer_ct + len, &len);

    ciphertext_len += len;
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAG_SIZE, tag);

    EVP_CIPHER_CTX_free(ctx);

    fwrite(buffer_ct, 1, ciphertext_len, fileout);
  }
}

void decryptFile(FILE *filein, FILE *fileout, unsigned char *IV,
  unsigned char *salt, unsigned char *key, unsigned char *aad, unsigned char *tag)
{
  unsigned char buffer_pt[1024];
  unsigned char buffer_ct[1024];

  EVP_CIPHER_CTX *ctx;

  int len = 0;
  int plaintext_len = 0;
  int nr_read = 0;

  while(!feof(filein))
  {
    nr_read = fread(buffer_ct, 1, 1024, filein);

    ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, IV);
    EVP_DecryptUpdate(ctx, buffer_pt, &len, buffer_ct, nr_read);

    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, TAG_SIZE, tag);

    plaintext_len = len;
    EVP_DecryptFinal_ex(ctx, buffer_pt + len, &len);

    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);

    fwrite(buffer_pt, 1, plaintext_len, fileout);
  }
}


int main(int argc, char *argv[])
{

  // ./task5 e|d input output
  // argv[0] = ./task5
  // argv[1] = e or d
  // argv[2] = input
  // argv[3] = output

  unsigned char key[KEY_SIZE], IV[IV_SIZE], salt[SALT_SIZE];
  char password[256];
  unsigned char aad[AAD_SIZE] = { 0 };
  unsigned char tag[TAG_SIZE] = { 0 };
  memset(password, 0, 256);

  if(argc != 4) // argc should be 4 for correct execution
  {
    // We print argv[0], it is the program name
    printf("usage: %s e|d inputfile outputfile \n", argv[0]);
  }
  else
  {
    FILE *filein = fopen(argv[2], "r");
    FILE *fileout = fopen(argv[3], "w");

    // fopen returns 0, the NULL pointer, on failure
    if(filein == 0)
    {
      printf("Error: Could not open input file: %s \n", argv[2]);
      return(-1);
    }
    if(fileout == 0)
    {
      printf("Error: Could not open output file: %s \n", argv[3]);
      return(-2);
    }


    // get password from user
    getPassword(password);

    // encrypt
    if(!strcmp(argv[1], "e"))
    {
      generateIVandSalt(IV, salt, aad);
      writeHeader(fileout, IV, salt, aad, tag);
      deriveKey(password, strlen(password), salt, key);
      encryptFile(filein, fileout, IV, salt, key, aad, tag);
      // decrypt
    }
    else if(!strcmp(argv[1], "d"))
    {
      if(!readHeader(filein, IV, salt, aad, tag))
      {
        printf("Error: Could not read header: %s \n", argv[2]);
        return(-3);
      }

      deriveKey(password, strlen(password), salt, key);
      decryptFile(filein, fileout, IV, salt, key, aad, tag);
    }
    memset(key, 0, KEY_SIZE);
    memset(password, 0, 16);
    fclose(filein);
    fclose(fileout);
  }
  return 0;
}
