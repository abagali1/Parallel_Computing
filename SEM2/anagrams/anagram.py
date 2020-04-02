#!/usr/bin/env python3
words, freq = {x for x in open('words.txt').read().splitlines() if len(x) == 6}, {}

for word in words:
  freq.setdefault(''.join(sorted(word)),[]).append(word)

print(sorted(freq[max(freq, key=lambda x: len(freq[x]))])[0])

