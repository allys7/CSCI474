#!/usr/bin/env bash

file="words.txt"
out="out.txt"
out2="out2.txt"
plain="plain.txt"

solution="8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9"

echo "" > "$out2"

while read -r key; do
    echo
    echo -e "KEY: $key"
    
    keylen=${#key}
    if [ $keylen -gt 15 ]; then
        echo "key gt 16!"
        continue
    fi
    
    while [ "$keylen" -lt 16 ]; do
        key="$key "
        keylen=${#key}
    done

    hexkey=$(echo -n "$key" | od -A n -t x1 | tr -d '[:space:]')
    openssl enc -aes-128-cbc -in "$plain" -out "$out" -K "$hexkey" -iv 0000000000000000

    outstr=$(cat $out | od -A n -t x1 | tr -d '[:space:]')
    echo "OUT: $outstr"
    echo "$outstr" >> "$out2"
    if [[ $outstr == $solution ]]; then
        echo "$key"
        exit
    fi
done < $file
