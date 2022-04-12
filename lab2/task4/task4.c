#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generate_random_string(char *msg) {
  int i;
  for (i = 0; i < 11; i++) {
    int value = rand() % 256 - 128;
    msg[i] = value;
  }
}

int hash_message(char *hashname, char *msg, unsigned char *md_value) {
  OpenSSL_add_all_digests();
  EVP_MD_CTX *ctx = EVP_MD_CTX_create();
  const EVP_MD *md = EVP_get_digestbyname(hashname);
  int len, i;
  EVP_DigestInit_ex(ctx, md, NULL);
  EVP_DigestUpdate(ctx, msg, strlen(msg));
  EVP_DigestFinal_ex(ctx, md_value, &len);
  EVP_MD_CTX_destroy(ctx);
  return len;
}

int crackCollisionHash(char *hashname) {
  char msg1[11], msg2[11];
  unsigned char hash1[EVP_MAX_MD_SIZE], hash2[EVP_MAX_MD_SIZE];
  int count = 0;
  // crack the hash
  int len1, len2, compareLen = 3;
  while (!count || strncmp(hash1, hash2, compareLen) != 0) {
    // sprintf(msg1, "%d", rand());	// convert to string
    generate_random_string(msg1);
    len1 = hash_message(hashname, msg1, hash1);
    // sprintf(msg2, "%d", rand());	// convert to string
    generate_random_string(msg2);
    len2 = hash_message(hashname, msg2, hash2);
    compareLen = (len1 < len2) ? len1 : len2;
    count++;
  }
  // printf("\n cracked after %d tries! %s and %s has same digest ", count,
  // msg1, msg2);
  printf("cracked after %d tries!\r\n", count);
  for (int i = 0; i < compareLen; i++)
    printf("%02x", hash1[i]);
  printf("\r\n");
  return count;
}

int crackOneWayHash(char *hashname) {
  char msg1[11], msg2[11];
  unsigned char hash1[EVP_MAX_MD_SIZE], hash2[EVP_MAX_MD_SIZE];
  int count = 0, i;
  generate_random_string(msg1);
  printf("Generated message 1: %s\r\nHash: ", msg1);
  //  get an initial message
  int len1 = hash_message(hashname, msg1, hash1);
  for (int i = 0; i < len1; i++)
    printf("%02x", hash1[i]);
  printf("\n");

  // loop to crack hash
  int len2, compareLen = len1;
  while (!count || strncmp(hash1, hash2, compareLen) != 0) {
    generate_random_string(msg2);
    len2 = hash_message(hashname, msg2, hash2);
    printf("%d %02x%02x%02x%02x\n", count, hash2[0], hash2[1], hash2[2],
           hash2[3]);
    count++;
    compareLen = (len1 < len2) ? len1 : len2;
  }
  printf("cracked after %d tries! \r\n", count);
  for (i = 0; i < compareLen; i++)
    printf("%02x", hash1[i]);
  printf("\r\n");
  return count;
}

int main(int argc, char *argv[]) {
  char *hashname = "md5";
  srand((int)time(0)); // init random seed
  int i, count;

  for (i = 0, count = 0; i < 15; i++)
    count += crackCollisionHash(hashname);
  printf("average time cracking collision-free: %d \r\n", count / 15);
  printf("One way \r\n");
  for (i = 0, count = 0; i < 5; i++)
    count += crackOneWayHash(hashname);
  printf("average time cracking one-way: %d \r\n", count / 5);
}