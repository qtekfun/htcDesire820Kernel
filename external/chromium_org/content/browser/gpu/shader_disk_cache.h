// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GPU_SHADER_DISK_CACHE_H_
#define CONTENT_BROWSER_GPU_SHADER_DISK_CACHE_H_

#include <map>
#include <queue>
#include <string>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "content/common/content_export.h"
#include "net/disk_cache/disk_cache.h"

namespace content {

class ShaderDiskCacheEntry;
class ShaderDiskReadHelper;
class ShaderClearHelper;

class CONTENT_EXPORT ShaderDiskCache
    : public base::RefCounted<ShaderDiskCache>,
      public base::SupportsWeakPtr<ShaderDiskCache> {
 public:
  void Init();

  void set_host_id(int host_id) { host_id_ = host_id; }

  
  void Cache(const std::string& key, const std::string& shader);

  
  
  
  
  
  int Clear(
      const base::Time begin_time,
      const base::Time end_time,
      const net::CompletionCallback& completion_callback);

  
  
  
  
  int SetAvailableCallback(const net::CompletionCallback& callback);

  
  int32 Size();

  
  // written to the cache.
  
  
  // been written to the cache.
  int SetCacheCompleteCallback(const net::CompletionCallback& callback);

 private:
  friend class base::RefCounted<ShaderDiskCache>;
  friend class ShaderDiskCacheEntry;
  friend class ShaderDiskReadHelper;
  friend class ShaderCacheFactory;

  explicit ShaderDiskCache(const base::FilePath& cache_path);
  ~ShaderDiskCache();

  void CacheCreatedCallback(int rv);

  disk_cache::Backend* backend() { return backend_.get(); }

  void EntryComplete(void* entry);
  void ReadComplete();

  bool cache_available_;
  int host_id_;
  base::FilePath cache_path_;
  bool is_initialized_;
  net::CompletionCallback available_callback_;
  net::CompletionCallback cache_complete_callback_;

  scoped_ptr<disk_cache::Backend> backend_;

  scoped_refptr<ShaderDiskReadHelper> helper_;
  std::map<void*, scoped_refptr<ShaderDiskCacheEntry> > entry_map_;

  DISALLOW_COPY_AND_ASSIGN(ShaderDiskCache);
};

class CONTENT_EXPORT ShaderCacheFactory {
 public:
  static ShaderCacheFactory* GetInstance();

  
  
  
  
  void ClearByPath(const base::FilePath& path,
                   const base::Time& begin_time,
                   const base::Time& end_time,
                   const base::Closure& callback);

  
  scoped_refptr<ShaderDiskCache> Get(int32 client_id);

  
  void SetCacheInfo(int32 client_id, const base::FilePath& path);

  
  void RemoveCacheInfo(int32 client_id);

  
  void AddToCache(const base::FilePath& path, ShaderDiskCache* cache);

  
  void RemoveFromCache(const base::FilePath& path);

 private:
  friend struct DefaultSingletonTraits<ShaderCacheFactory>;
  friend class ShaderClearHelper;

  ShaderCacheFactory();
  ~ShaderCacheFactory();

  scoped_refptr<ShaderDiskCache> GetByPath(const base::FilePath& path);
  void CacheCleared(const base::FilePath& path);

  typedef std::map<base::FilePath, ShaderDiskCache*> ShaderCacheMap;
  ShaderCacheMap shader_cache_map_;

  typedef std::map<int32, base::FilePath> ClientIdToPathMap;
  ClientIdToPathMap client_id_to_path_map_;

  typedef std::queue<scoped_refptr<ShaderClearHelper> > ShaderClearQueue;
  typedef std::map<base::FilePath, ShaderClearQueue> ShaderClearMap;
  ShaderClearMap shader_clear_map_;

  DISALLOW_COPY_AND_ASSIGN(ShaderCacheFactory);
};

}  

#endif  

