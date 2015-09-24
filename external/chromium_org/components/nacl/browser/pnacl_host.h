// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_BROWSER_PNACL_HOST_H_
#define COMPONENTS_NACL_BROWSER_PNACL_HOST_H_

#include <map>

#include "base/callback_forward.h"
#include "base/memory/singleton.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "components/nacl/browser/nacl_file_host.h"
#include "components/nacl/common/pnacl_types.h"
#include "ipc/ipc_platform_file.h"

namespace net {
class DrainableIOBuffer;
}

namespace pnacl {

class PnaclHostTest;
class PnaclTranslationCache;

class PnaclHost {
 public:
  typedef base::Callback<void(base::PlatformFile)> TempFileCallback;
  typedef base::Callback<void(base::PlatformFile, bool is_hit)> NexeFdCallback;

  static PnaclHost* GetInstance();

  PnaclHost();
  ~PnaclHost();

  
  
  void Init();

  
  
  void CreateTemporaryFile(TempFileCallback cb);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void GetNexeFd(int render_process_id,
                 int render_view_id,
                 int pp_instance,
                 bool is_incognito,
                 const nacl::PnaclCacheInfo& cache_info,
                 const NexeFdCallback& cb);

  
  
  
  void TranslationFinished(int render_process_id,
                           int pp_instance,
                           bool success);

  
  
  
  void RendererClosing(int render_process_id);

  
  
  
  
  void ClearTranslationCacheEntriesBetween(base::Time initial_time,
                                           base::Time end_time,
                                           const base::Closure& callback);

  
  size_t pending_translations() { return pending_translations_.size(); }

 private:
  
  
  
  friend struct DefaultSingletonTraits<PnaclHost>;
  friend class pnacl::PnaclHostTest;
  enum CacheState {
    CacheUninitialized,
    CacheInitializing,
    CacheReady
  };
  class PendingTranslation {
   public:
    PendingTranslation();
    ~PendingTranslation();
    base::ProcessHandle process_handle;
    int render_view_id;
    base::PlatformFile nexe_fd;
    bool got_nexe_fd;
    bool got_cache_reply;
    bool got_cache_hit;
    bool is_incognito;
    scoped_refptr<net::DrainableIOBuffer> nexe_read_buffer;
    NexeFdCallback callback;
    std::string cache_key;
    nacl::PnaclCacheInfo cache_info;
  };

  typedef std::pair<int, int> TranslationID;
  typedef std::map<TranslationID, PendingTranslation> PendingTranslationMap;
  static bool TranslationMayBeCached(
      const PendingTranslationMap::iterator& entry);

  void InitForTest(base::FilePath temp_dir);
  void OnCacheInitialized(int net_error);

  static void DoCreateTemporaryFile(base::FilePath temp_dir_,
                                    TempFileCallback cb);

  
  void SendCacheQueryAndTempFileRequest(const std::string& key,
                                        const TranslationID& id);
  void OnCacheQueryReturn(const TranslationID& id,
                          int net_error,
                          scoped_refptr<net::DrainableIOBuffer> buffer);
  void OnTempFileReturn(const TranslationID& id, base::PlatformFile fd);
  void CheckCacheQueryReady(const PendingTranslationMap::iterator& entry);

  
  void ReturnMiss(const PendingTranslationMap::iterator& entry);
  static scoped_refptr<net::DrainableIOBuffer> CopyFileToBuffer(
      base::PlatformFile fd);
  void StoreTranslatedNexe(TranslationID id,
                           scoped_refptr<net::DrainableIOBuffer>);
  void OnTranslatedNexeStored(const TranslationID& id, int net_error);
  void RequeryMatchingTranslations(const std::string& key);

  
  static int CopyBufferToFile(base::PlatformFile fd,
                              scoped_refptr<net::DrainableIOBuffer> buffer);
  void OnBufferCopiedToTempFile(const TranslationID& id, int file_error);

  void OnEntriesDoomed(const base::Closure& callback, int net_error);

  void DeInitIfSafe();

  
  
  int pending_backend_operations_;
  CacheState cache_state_;
  base::FilePath temp_dir_;
  scoped_ptr<pnacl::PnaclTranslationCache> disk_cache_;
  PendingTranslationMap pending_translations_;
  base::ThreadChecker thread_checker_;
  base::WeakPtrFactory<PnaclHost> weak_factory_;
  DISALLOW_COPY_AND_ASSIGN(PnaclHost);
};

}  

#endif  
