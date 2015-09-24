// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_BROWSER_PNACL_TRANSLATION_CACHE_H_
#define COMPONENTS_NACL_BROWSER_PNACL_TRANSLATION_CACHE_H_

#include <map>

#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "net/base/cache_type.h"

namespace base {
class MessageLoopProxy;
}

namespace disk_cache {
class Backend;
}

namespace nacl {
struct PnaclCacheInfo;
}

namespace net {
class DrainableIOBuffer;
}

namespace pnacl {
typedef base::Callback<void(int)> CompletionCallback;
typedef base::Callback<void(int, scoped_refptr<net::DrainableIOBuffer>)>
    GetNexeCallback;
class PnaclTranslationCacheEntry;
extern const int kMaxMemCacheSize;

class PnaclTranslationCache
    : public base::SupportsWeakPtr<PnaclTranslationCache> {
 public:
  PnaclTranslationCache();
  virtual ~PnaclTranslationCache();

  
  
  
  int InitOnDisk(const base::FilePath& cache_dir,
                 const CompletionCallback& callback);

  
  
  
  int InitInMemory(const CompletionCallback& callback);

  
  
  
  
  void StoreNexe(const std::string& key,
                 net::DrainableIOBuffer* nexe_data,
                 const CompletionCallback& callback);

  
  
  
  void GetNexe(const std::string& key, const GetNexeCallback& callback);

  
  int Size();

  
  static std::string GetKey(const nacl::PnaclCacheInfo& info);

  
  
  
  int DoomEntriesBetween(base::Time initial, base::Time end,
                         const CompletionCallback& callback);

 private:
  friend class PnaclTranslationCacheEntry;
  friend class PnaclTranslationCacheTest;
  
  
  void OpComplete(PnaclTranslationCacheEntry* entry);
  disk_cache::Backend* backend() { return disk_cache_.get(); }

  int Init(net::CacheType,
           const base::FilePath& directory,
           int cache_size,
           const CompletionCallback& callback);

  void OnCreateBackendComplete(int rv);

  scoped_ptr<disk_cache::Backend> disk_cache_;
  CompletionCallback init_callback_;
  bool in_memory_;
  std::map<void*, scoped_refptr<PnaclTranslationCacheEntry> > open_entries_;

  DISALLOW_COPY_AND_ASSIGN(PnaclTranslationCache);
};

}  

#endif  
