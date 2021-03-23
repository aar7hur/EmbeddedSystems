#include <string.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/aes.h>



int ctx_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx, 
             EVP_CIPHER_CTX *d_ctx);

unsigned char *ctx_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);
unsigned char *ctx_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);



