// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_UTILS_HUFFMAN_ENCODE_H_
#define WEBP_UTILS_HUFFMAN_ENCODE_H_

#include "../webp/types.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef struct {
  uint8_t code;         
  uint8_t extra_bits;   
} HuffmanTreeToken;

typedef struct {
  int       num_symbols;   
  uint8_t*  code_lengths;  
  uint16_t* codes;         
} HuffmanTreeCode;

int VP8LCreateCompressedHuffmanTree(const HuffmanTreeCode* const tree,
                                    HuffmanTreeToken* tokens, int max_tokens);

int VP8LCreateHuffmanTree(int* const histogram, int tree_depth_limit,
                          HuffmanTreeCode* const tree);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif  
