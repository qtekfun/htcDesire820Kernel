// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_PROTOCOL_PARSER_H_
#define CHROME_BROWSER_SAFE_BROWSING_PROTOCOL_PARSER_H_



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
                   int* next_update_sec,
                   bool* reset,
                   std::vector<SBChunkDelete>* chunk_deletes,
                   std::vector<ChunkUrl>* chunk_urls);

  
  
  
  bool ParseChunk(const std::string& list_name,
                  const char* chunk_data,
                  int chunk_len,
                  SBChunkList* chunks);

  
  bool ParseGetHash(const char* chunk_data,
                    int chunk_len,
                    std::vector<SBFullHashResult>* full_hashes);

  
  void FormatGetHash(const std::vector<SBPrefix>& prefixes,
                     std::string* request);

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

  
  static bool ReadHostAndPrefixCount(const char** data,
                                     int* remaining,
                                     SBPrefix* host,
                                     int* count);
  static bool ReadChunkId(const char** data, int* remaining, int* chunk_id);
  static bool ReadPrefixes(
      const char** data, int* remaining, SBEntry* entry, int count);

  
  std::string list_name_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingProtocolParser);
};


#endif  
