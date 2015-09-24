// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_STORE_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_STORE_H_
#pragma once

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/hash_tables.h"
#include "base/task.h"
#include "base/time.h"
#include "chrome/browser/safe_browsing/safe_browsing_util.h"

class FilePath;



struct SBAddPrefix {
  int32 chunk_id;
  SBPrefix prefix;

  SBAddPrefix(int32 id, SBPrefix p) : chunk_id(id), prefix(p) {}
  SBAddPrefix() : chunk_id(), prefix() {}

  int32 GetAddChunkId() const { return chunk_id; }
  SBPrefix GetAddPrefix() const { return prefix; }
};

struct SBSubPrefix {
  int32 chunk_id;
  int32 add_chunk_id;
  SBPrefix add_prefix;

  SBSubPrefix(int32 id, int32 add_id, int prefix)
      : chunk_id(id), add_chunk_id(add_id), add_prefix(prefix) {}
  SBSubPrefix() : chunk_id(), add_chunk_id(), add_prefix() {}

  int32 GetAddChunkId() const { return add_chunk_id; }
  SBPrefix GetAddPrefix() const { return add_prefix; }
};

struct SBAddFullHash {
  int32 chunk_id;
  int32 received;
  SBFullHash full_hash;

  SBAddFullHash(int32 id, base::Time r, const SBFullHash& h)
      : chunk_id(id),
        received(static_cast<int32>(r.ToTimeT())),
        full_hash(h) {
  }

  
  
  SBAddFullHash(int32 id, int32 r, const SBFullHash& h)
      : chunk_id(id), received(r), full_hash(h) {}

  SBAddFullHash() : chunk_id(), received(), full_hash() {}

  int32 GetAddChunkId() const { return chunk_id; }
  SBPrefix GetAddPrefix() const { return full_hash.prefix; }
};

struct SBSubFullHash {
  int32 chunk_id;
  int32 add_chunk_id;
  SBFullHash full_hash;

  SBSubFullHash(int32 id, int32 add_id, const SBFullHash& h)
      : chunk_id(id), add_chunk_id(add_id), full_hash(h) {}
  SBSubFullHash() : chunk_id(), add_chunk_id(), full_hash() {}

  int32 GetAddChunkId() const { return add_chunk_id; }
  SBPrefix GetAddPrefix() const { return full_hash.prefix; }
};

template <class T, class U>
bool SBAddPrefixLess(const T& a, const U& b) {
  if (a.GetAddChunkId() != b.GetAddChunkId())
    return a.GetAddChunkId() < b.GetAddChunkId();

  return a.GetAddPrefix() < b.GetAddPrefix();
}

template <class T, class U>
bool SBAddPrefixHashLess(const T& a, const U& b) {
  if (SBAddPrefixLess(a, b))
    return true;

  if (SBAddPrefixLess(b, a))
    return false;

  return memcmp(a.full_hash.full_hash, b.full_hash.full_hash,
                sizeof(a.full_hash.full_hash)) < 0;
}

void SBProcessSubs(std::vector<SBAddPrefix>* add_prefixes,
                   std::vector<SBSubPrefix>* sub_prefixes,
                   std::vector<SBAddFullHash>* add_full_hashes,
                   std::vector<SBSubFullHash>* sub_full_hashes,
                   const base::hash_set<int32>& add_chunks_deleted,
                   const base::hash_set<int32>& sub_chunks_deleted);

void SBCheckPrefixMisses(const std::vector<SBAddPrefix>& add_prefixes,
                         const std::set<SBPrefix>& prefix_misses);


class SafeBrowsingStore {
 public:
  SafeBrowsingStore() {}
  virtual ~SafeBrowsingStore() {}

  
  
  
  
  
  
  virtual void Init(const FilePath& filename,
                    Callback0::Type* corruption_callback) = 0;

  
  
  virtual bool Delete() = 0;

  
  virtual bool GetAddPrefixes(std::vector<SBAddPrefix>* add_prefixes) = 0;

  
  virtual bool GetAddFullHashes(
      std::vector<SBAddFullHash>* add_full_hashes) = 0;

  
  
  
  virtual bool BeginUpdate() = 0;

  
  
  
  
  
  virtual bool BeginChunk() = 0;

  virtual bool WriteAddPrefix(int32 chunk_id, SBPrefix prefix) = 0;
  virtual bool WriteAddHash(int32 chunk_id,
                            base::Time receive_time,
                            const SBFullHash& full_hash) = 0;
  virtual bool WriteSubPrefix(int32 chunk_id,
                              int32 add_chunk_id, SBPrefix prefix) = 0;
  virtual bool WriteSubHash(int32 chunk_id, int32 add_chunk_id,
                            const SBFullHash& full_hash) = 0;

  
  
  virtual bool FinishChunk() = 0;

  
  virtual void SetAddChunk(int32 chunk_id) = 0;
  virtual bool CheckAddChunk(int32 chunk_id) = 0;
  virtual void GetAddChunks(std::vector<int32>* out) = 0;
  virtual void SetSubChunk(int32 chunk_id) = 0;
  virtual bool CheckSubChunk(int32 chunk_id) = 0;
  virtual void GetSubChunks(std::vector<int32>* out) = 0;

  
  
  virtual void DeleteAddChunk(int32 chunk_id) = 0;
  virtual void DeleteSubChunk(int32 chunk_id) = 0;

  
  
  
  
  
  // (could be written via WriteAddHash(), but that would flush the
  
  
  
  virtual bool FinishUpdate(
      const std::vector<SBAddFullHash>& pending_adds,
      const std::set<SBPrefix>& prefix_misses,
      std::vector<SBAddPrefix>* add_prefixes_result,
      std::vector<SBAddFullHash>* add_full_hashes_result) = 0;

  
  
  virtual bool CancelUpdate() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingStore);
};

#endif  
