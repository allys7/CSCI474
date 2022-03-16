from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import Cipher
from cryptography.hazmat.primitives.ciphers.algorithms import AES
from cryptography.hazmat.primitives.ciphers.modes import CBC
from cryptography.hazmat.primitives.padding import PKCS7

iv = b'0000000000000000'

solution = "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9"
plain = 'This is a top secret.'

if __name__ == '__main__':
    
    words_file = open('words.txt', 'r')
    # words = words_file.readlines()
    words = ['median']

    backend = default_backend()
    padder = PKCS7(128).padder()

    for word in words:
        print(f'KEY: {word}')
        keystr = word
        if (len(keystr) >= 16):
            continue # skip words that are too long

        # pad key with spaces
        while (len(keystr) < 16):
            keystr += ' '

        key = bytes(keystr, 'ascii')
        print(f'BYTES:')
        print(key.hex(' '))

        aes = AES(key)
        cbc = CBC(iv)

        cipher = Cipher(aes, cbc, backend)
        enc = cipher.encryptor()
        cipher_text = enc.update(bytes(plain, 'ascii')) + enc.finalize()
        print(cipher_text)

        print('CIPHER TEXT:')
        print(cipher_text.hex(' '))

        if (cipher_text.hex(' ') == solution):
            print(f"FOUND SOLUTION! {word}")
            break
