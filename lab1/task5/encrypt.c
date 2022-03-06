#include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void handleErrors(void);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext);

int main(void)
{
    if (0)
    {
        /*
         * Set up the key and iv. Do I need to say to not hard code these in a
         * real application? :-)
         */

        /* A 256 bit key */
        unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

        /* A 128 bit IV */
        unsigned char *iv = (unsigned char *)"0123456789012345";

        /* Message to be encrypted */
        unsigned char *plaintext =
            (unsigned char *)"The quick brown fox jumps over the lazy dog";

        /*
         * Buffer for ciphertext. Ensure the buffer is long enough for the
         * ciphertext which may be longer than the plaintext, depending on the
         * algorithm and mode.
         */
        unsigned char ciphertext[128];

        /* Buffer for the decrypted text */
        unsigned char decryptedtext[128];

        int decryptedtext_len, ciphertext_len;

        /* Encrypt the plaintext */
        ciphertext_len = encrypt(plaintext, strlen((char *)plaintext), key, iv,
                                 ciphertext);

        /* Do something useful with the ciphertext here */
        printf("Initial value is:\n");
        BIO_dump_fp(stdout, (const char *)iv, 17);

        printf("Ciphertext is:\n");
        BIO_dump_fp(stdout, (const char *)ciphertext, ciphertext_len);

        /* Decrypt the ciphertext */
        decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
                                    decryptedtext);

        /* Add a NULL terminator. We are expecting printable text */
        decryptedtext[decryptedtext_len] = '\0';

        /* Show the decrypted text */
        printf("Decrypted text is:\n");
        BIO_dump_fp(stdout, (const char *)decryptedtext, decryptedtext_len);

        return 0;
    }
    else
    {
        // A 128 bit IV, all 0x00 bytes
        unsigned char *iv = (unsigned char *)"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

        // Cipher text from lab document
        unsigned char ciphertext[128] = {0x8d, 0x20, 0xe5, 0x05, 0x6a, 0x8d, 0x24,
                                         0xd0, 0x46, 0x2c, 0xe7, 0x4e, 0x49, 0x04,
                                         0xc1, 0xb5, 0x13, 0xe1, 0x0d, 0x1d, 0xf4,
                                         0xa2, 0xef, 0x2a, 0xd4, 0x54, 0x0f, 0xae,
                                         0x1c, 0xa0, 0xaa, 0xf9};

        // Buffer for the real plaintext once it is discovered
        unsigned char plaintext[128];

        // Buffer for the decrypted text for a given key
        unsigned char decryptedtext[128];

        // A 128 bit key
        unsigned char key[16];

        int decryptedtext_len, ciphertext_len;
        ciphertext_len = 128;

        printf("Initial value is:\n");
        BIO_dump_fp(stdout, (const char *)iv, 17);

        // Do something useful with the ciphertext here
        printf("Ciphertext is:\n");
        BIO_dump_fp(stdout, (const char *)ciphertext, ciphertext_len);

        // open file of words
        FILE *word_file;
        char *line = NULL;
        size_t len = 0;
        ssize_t word_len;
        const char space = ' ';

        if (!(word_file = fopen("./words.txt", "r")))
            fprintf(stderr, "ERROR: unable to open file\n");

        while ((word_len = getline(&line, &len, word_file)) >= 0)
        {
            line[strcspn(line, "\r\n")] = 0; // remove newline
            if (word_len < 16)
            {
                // pad word with spaces
                while (word_len <= 16)
                {
                    strncat(line, &space, 1);
                    word_len++;
                }

                decryptedtext_len = decrypt(ciphertext, ciphertext_len, line, iv,
                                            decryptedtext);

                // Add a NULL terminator. We are expecting printable text
                decryptedtext[decryptedtext_len] = '\0';

                // Show the decrypted text
                printf("Decrypted text is:\n");
                printf("%s:\n%s\n\n", line, decryptedtext);
            }
        }

        /* Provided decryption code
                // Decrypt the ciphertext
                decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
                                            decryptedtext);

                // Add a NULL terminator. We are expecting printable text
                decryptedtext[decryptedtext_len] = '\0';

                // Show the decrypted text
                printf("Decrypted text is:\n");
                printf("%s\n", decryptedtext);
        */

        return 0;
    }
}

void handleErrors(void)
{
    fprintf(stderr, "ERROR!\n");
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    EVP_CIPHER_CTX_set_padding(ctx, 0);

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}
