// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_UTIL_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_UTIL_H_

#include <cstring>
#include <deque>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/safe_browsing/chunk_range.h"

class GURL;

class SBEntry;

typedef int32 SBPrefix;

struct ChunkUrl {
  std::string url;
  std::string list_name;
};

union SBFullHash {
  char full_hash[32];
  SBPrefix prefix;
};

inline bool operator==(const SBFullHash& lhash, const SBFullHash& rhash) {
  return memcmp(lhash.full_hash, rhash.full_hash, sizeof(SBFullHash)) == 0;
}

inline bool operator<(const SBFullHash& lhash, const SBFullHash& rhash) {
  return memcmp(lhash.full_hash, rhash.full_hash, sizeof(SBFullHash)) < 0;
}

struct SBChunkHost {
  SBPrefix host;
  SBEntry* entry;
};

struct SBChunk {
  SBChunk();
  ~SBChunk();

  int chunk_number;
  int list_id;
  bool is_add;
  std::deque<SBChunkHost> hosts;
};

class SBChunkList {
 public:
  SBChunkList();
  ~SBChunkList();

  
  
  bool empty() const { return chunks_.empty(); }
  size_t size() { return chunks_.size(); }

  void push_back(const SBChunk& chunk) { chunks_.push_back(chunk); }
  SBChunk& back() { return chunks_.back(); }
  SBChunk& front() { return chunks_.front(); }
  const SBChunk& front() const { return chunks_.front(); }

  typedef std::vector<SBChunk>::const_iterator const_iterator;
  const_iterator begin() const { return chunks_.begin(); }
  const_iterator end() const { return chunks_.end(); }

  typedef std::vector<SBChunk>::iterator iterator;
  iterator begin() { return chunks_.begin(); }
  iterator end() { return chunks_.end(); }

  SBChunk& operator[](size_t n) { return chunks_[n]; }
  const SBChunk& operator[](size_t n) const { return chunks_[n]; }

  
  void clear();

 private:
  std::vector<SBChunk> chunks_;

  DISALLOW_COPY_AND_ASSIGN(SBChunkList);
};

struct SBFullHashResult {
  SBFullHash hash;
  std::string list_name;
  int add_chunk_id;
};

struct SBListChunkRanges {
  explicit SBListChunkRanges(const std::string& n);

  std::string name;  
  std::string adds;  
  std::string subs;  
};

struct SBChunkDelete {
  SBChunkDelete();
  ~SBChunkDelete();

  std::string list_name;
  bool is_sub_del;
  std::vector<ChunkRange> chunk_del;
};

enum SBThreatType {
  
  SB_THREAT_TYPE_SAFE,

  
  SB_THREAT_TYPE_URL_PHISHING,

  
  SB_THREAT_TYPE_URL_MALWARE,

  
  SB_THREAT_TYPE_BINARY_MALWARE_URL,

  
  SB_THREAT_TYPE_BINARY_MALWARE_HASH,

  
  
  SB_THREAT_TYPE_CLIENT_SIDE_PHISHING_URL,

  
  SB_THREAT_TYPE_EXTENSION,

  
  
  SB_THREAT_TYPE_CLIENT_SIDE_MALWARE_URL,
};


class SBEntry {
 public:
  enum Type {
    ADD_PREFIX,     
    SUB_PREFIX,     
    ADD_FULL_HASH,  
    SUB_FULL_HASH,  
  };

  
  
  static SBEntry* Create(Type type, int prefix_count);

  
  void Destroy();

  void set_list_id(int list_id) { data_.list_id = list_id; }
  int list_id() const { return data_.list_id; }
  void set_chunk_id(int chunk_id) { data_.chunk_id = chunk_id; }
  int chunk_id() const { return data_.chunk_id; }
  int prefix_count() const { return data_.prefix_count; }

  
  bool IsPrefix() const {
    return type() == ADD_PREFIX || type() == SUB_PREFIX;
  }

  
  bool IsAdd() const {
    return type() == ADD_PREFIX || type() == ADD_FULL_HASH;
  }

  
  bool IsSub() const {
    return type() == SUB_PREFIX || type() == SUB_FULL_HASH;
  }

  
  int HashLen() const {
    return IsPrefix() ? sizeof(SBPrefix) : sizeof(SBFullHash);
  }

  
  
  int ChunkIdAtPrefix(int index) const;

  
  void SetChunkIdAtPrefix(int index, int chunk_id);

  
  
  const SBPrefix& PrefixAt(int index) const;
  const SBFullHash& FullHashAt(int index) const;

  
  
  void SetPrefixAt(int index, const SBPrefix& prefix);
  void SetFullHashAt(int index, const SBFullHash& full_hash);

 private:
  
  struct SBSubPrefix {
    int add_chunk;
    SBPrefix prefix;
  };

  
  struct SBSubFullHash {
    int add_chunk;
    SBFullHash prefix;
  };

  
  
  struct Data {
    int list_id;
    
    
    
    
    int chunk_id;
    Type type;
    int prefix_count;
  };

  SBEntry();
  ~SBEntry();

  
  
  static int PrefixSize(Type type);

  
  static int Size(Type type, int prefix_count);

  
  int Size() const;

  Type type() const { return data_.type; }

  void set_prefix_count(int count) { data_.prefix_count = count; }
  void set_type(Type type) { data_.type = type; }

  
  
  Data data_;
  union {
    SBPrefix add_prefixes_[1];
    SBSubPrefix sub_prefixes_[1];
    SBFullHash add_full_hashes_[1];
    SBSubFullHash sub_full_hashes_[1];
  };
};



namespace safe_browsing_util {

extern const char kMalwareList[];
extern const char kPhishingList[];
extern const char kBinUrlList[];
extern const char kBinHashList[];
extern const char kCsdWhiteList[];
extern const char kDownloadWhiteList[];
extern const char kExtensionBlacklist[];
extern const char kSideEffectFreeWhitelist[];
extern const char kIPBlacklist[];

extern const char* kAllLists[10];

enum ListType {
  INVALID = -1,
  MALWARE = 0,
  PHISH = 1,
  BINURL = 2,
  BINHASH = 3,
  CSDWHITELIST = 4,
  
  
  
  DOWNLOADWHITELIST = 6,
  
  EXTENSIONBLACKLIST = 8,
  
  SIDEEFFECTFREEWHITELIST = 10,
  
  IPBLACKLIST = 12,
  
};

ListType GetListId(const std::string& name);

bool GetListName(ListType list_id, std::string* list);

void CanonicalizeUrl(const GURL& url, std::string* canonicalized_hostname,
                     std::string* canonicalized_path,
                     std::string* canonicalized_query);

void GenerateHostsToCheck(const GURL& url, std::vector<std::string>* hosts);

void GeneratePathsToCheck(const GURL& url, std::vector<std::string>* paths);

void GeneratePatternsToCheck(const GURL& url, std::vector<std::string>* urls);

int GetHashIndex(const SBFullHash& hash,
                 const std::vector<SBFullHashResult>& full_hashes);

int GetUrlHashIndex(const GURL& url,
                    const std::vector<SBFullHashResult>& full_hashes);

bool IsPhishingList(const std::string& list_name);
bool IsMalwareList(const std::string& list_name);
bool IsBadbinurlList(const std::string& list_name);
bool IsBadbinhashList(const std::string& list_name);
bool IsExtensionList(const std::string& list_name);

GURL GeneratePhishingReportUrl(const std::string& report_page,
                               const std::string& url_to_report,
                               bool is_client_side_detection);

SBFullHash StringToSBFullHash(const std::string& hash_in);
std::string SBFullHashToString(const SBFullHash& hash_out);

}  

#endif  
