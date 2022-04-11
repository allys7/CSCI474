#!/usr/bin/env python3

md5_1 = 0xe9faddf4d13ff1329e75e2573b86769b
md5_2 = 0x5ac810b7288958194ad3282cfbfc4d8c
bin_md5_1 = bin(md5_1)[2:]
bin_md5_2 = bin(md5_2)[2:]

sha256_1 = 0xdfee893b95f63da38bd4a2e50826bc11cc9c5b90c7db14ec1a5ce83db1e90fea
sha256_2 = 0x2cc68a94df19e743036d5c3f8f88618ef2da49e0c5ac809ac5111ce584e453e6
bin_sha256_1 = bin(sha256_1)[2:]
bin_sha256_2 = bin(sha256_2)[2:]

# pad back to full size
md5_size, sha256_size = 128, 256
bin_md5_1 = "0"*(md5_size - len(bin_md5_1)) + bin_md5_1
bin_md5_2 = "0"*(md5_size - len(bin_md5_2)) + bin_md5_2
bin_sha256_1 = "0"*(sha256_size - len(bin_sha256_1)) + bin_sha256_1
bin_sha256_2 = "0"*(sha256_size - len(bin_sha256_2)) + bin_sha256_2

md5_shared = 0
for i in range(md5_size):
    if bin_md5_1[i] == bin_md5_2[i]:
        md5_shared += 1

sha256_shared = 0
for i in range(sha256_size):
    if bin_sha256_1[i] == bin_sha256_2[i]:
        sha256_shared += 1

print(f'MD5: {md5_shared}')

print(f'SHA256: {sha256_shared}')