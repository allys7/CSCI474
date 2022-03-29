def square_and_multiply(a: int, b: int, n: int):
    c = 0
    f = 1

    # convert key to binary string and remove '0b' prefix
    binkey = str(bin(b))[2::]

    # due to python indexing being left to right, looping from 0 over the length
    # is equivalent to looping from k down to 0 like in the slides
    for i in range(0, len(binkey)):
        c *= 2
        f = (f * f) % n
        if (binkey[i] == '1'):
            c += 1
            f = (f * a) % n
    return f, c

if __name__ == '__main__':
    f, c = square_and_multiply(a=5, b=596, n=1234)
    print(f'f: {f}')
    print(f'c: {c}')




