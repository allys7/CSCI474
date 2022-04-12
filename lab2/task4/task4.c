#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void generate_random_string(char *msg) {
  int i;
  for (i = 0; i < 11; i++)
    msg[i] = rand() % 256 - 128;
}

int crackOneWayHash(char *hashname) {
  char msg1[11], msg2[11];
  unsigned char hash1[EVP_MAX_MD_SIZE], hash2[EVP_MAX_MD_SIZE];
  int count = 0, i;
  generate_random_string(msg1);
  //  get an initial message
  int len1 = hash_message(hashname, msg1, hash1);

  // loop to crack hash
  int len2, compareLen = len1;
  while (!count || strncmp(hash1, hash2, compareLen) != 0) {
    generate_random_string(msg2);
    len2 = hash_message(hashname, msg2, hash2);
    count++;
    compareLen = (len1 < len2) ? len1 : len2;
  }
  printf("cracked after %d tries!", count);
  for (i = 0; i < 3; i++)
    printf("%02x", hash1[i]);
  printf("\r\n");
  return count;
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
  printf("cracked after %d tries! same digest ", count);
  for (int i = 0; i < 3; i++)
    printf("%02x", hash1[i]);
  printf("\n");
  return count;
}

int main(int argc, char *argv[]) {
  char *hashname;
  if (!argv[1])
    // set to md5 by default
    hashname = "md5";
  else
    hashname = argv[1];
  srand((int)time(0)); // init random seed
  int i, count;
  for (i = 0, count = 0; i < 15; i++)
    count += crackCollisionHash(hashname);
  printf("average time cracking collision-free: %d \n", count / 15);
  for (i = 0, count = 0; i < 5; i++)
    count += crackOneWayHash(hashname);
  printf("average time cracking one-way: %d \n", count / 5);
}