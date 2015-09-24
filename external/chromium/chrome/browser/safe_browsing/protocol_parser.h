// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_PROTOCOL_PARSER_H_
#define CHROME_BROWSER_SAFE_BROWSING_PROTOCOL_PARSER_H_
#pragma once



#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/safe_browsing/chunk_range.h"
#include "chrome/browser/safe_browsing/safe_browsing_util.h"


class SafeBrowsingProtocolParser {
 public:
  SafeBrowsingProtocolParser();

  
  
  
  
  
  
  
  
  
  
  bool ParseUpdate(const char* chunk_data,
                   int chunk_len,
                   const std::string& key,
                   int* next_update_sec,
                   bool* re_key,
                   bool* reset,
                   std::vector<SBChunkDelete>* chunk_deletes,
                   std::vector<ChunkUrl>* chunk_urls);

  
  
  
  bool ParseChunk(const std::string& list_name,
                  const char* chunk_data,
                  int chunk_len,
                  const std::string& key,
                  const std::string& mac,
                  bool* re_key,
                  SBChunkList* chunks);

  
  
  bool ParseGetHash(const char* chunk_data,
                    int chunk_len,
                    const std::string& key,
                    bool* re_key,
                    std::vector<SBFullHashResult>* full_hashes);

  
  void FormatGetHash(const std::vector<SBPrefix>& prefixes,
                     std::string* request);

  
  
  bool ParseNewKey(const char* chunk_data,
                   int chunk_length,
                   std::string* client_key,
                   std::string* wrapped_key);

 private:
  bool ParseAddChunk(const std::string& list_name,
                     const char* data,
                     int data_len,
                     int hash_len,
                     std::deque<SBChunkHost>* hosts);
  bool ParseSubChunk(const std::string& list_name,
                     const char* data,
                     int data_len,
                     int hash_len,
                     std::deque<SBChunkHost>* hosts);

  
  static void ReadHostAndPrefixCount(const char** data,
                                     int* remaining,
                                     SBPrefix* host,
                                     int* count);
  static int ReadChunkId(const char** data, int* remaining);
  static bool ReadPrefixes(
      const char** data, int* remaining, SBEntry* entry, int count);

  
  std::string list_name_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingProtocolParser);
};


#endif  
