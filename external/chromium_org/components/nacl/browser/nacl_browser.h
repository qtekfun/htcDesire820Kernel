// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_BROWSER_NACL_BROWSER_H_
#define COMPONENTS_NACL_BROWSER_NACL_BROWSER_H_

#include <deque>

#include "base/bind.h"
#include "base/containers/mru_cache.h"
#include "base/files/file_util_proxy.h"
#include "base/memory/singleton.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "base/time/time.h"
#include "components/nacl/browser/nacl_browser_delegate.h"
#include "components/nacl/browser/nacl_validation_cache.h"

class URLPattern;
class GURL;

namespace nacl {

base::PlatformFile OpenNaClExecutableImpl(const base::FilePath& file_path);

class NaClBrowser {
 public:
  static NaClBrowser* GetInstance();

  
  bool IsOk() const;

  
  bool IsReady() const;

  
  
  void EnsureAllResourcesAvailable();

  
  
  void WaitForResources(const base::Closure& reply);

  
  
  
  
  void EnsureIrtAvailable();

  
  const base::FilePath& GetIrtFilePath();

  
  base::PlatformFile IrtFile() const;

  
  
  

  
  void FireGdbDebugStubPortOpened(int port);
  bool HasGdbDebugStubPortListener();
  void SetGdbDebugStubPortListener(base::Callback<void(int)> listener);
  void ClearGdbDebugStubPortListener();

  bool ValidationCacheIsEnabled() const {
    return validation_cache_is_enabled_;
  }

  const std::string& GetValidationCacheKey() const {
    return validation_cache_.GetValidationCacheKey();
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void PutFilePath(const base::FilePath& path, uint64* file_token_lo,
                   uint64* file_token_hi);
  bool GetFilePath(uint64 file_token_lo, uint64 file_token_hi,
                   base::FilePath* path);

  bool QueryKnownToValidate(const std::string& signature, bool off_the_record);
  void SetKnownToValidate(const std::string& signature, bool off_the_record);
  void ClearValidationCache(const base::Closure& callback);
#if defined(OS_WIN)
  
  
  bool GetNaCl64ExePath(base::FilePath* exe_path);
#endif

  void EarlyStartup();
  static void SetDelegate(NaClBrowserDelegate* delegate);
  static NaClBrowserDelegate* GetDelegate();

  
  
  void OnProcessCrashed();
  
  
  bool IsThrottled();

 private:
  friend struct DefaultSingletonTraits<NaClBrowser>;

  enum NaClResourceState {
    NaClResourceUninitialized,
    NaClResourceRequested,
    NaClResourceReady
  };

  NaClBrowser();
  ~NaClBrowser();

  void InitIrtFilePath();

  void OpenIrtLibraryFile();

  void OnIrtOpened(base::PlatformFileError error_code,
                   base::PassPlatformFile file, bool created);

  void InitValidationCacheFilePath();
  void EnsureValidationCacheAvailable();
  void OnValidationCacheLoaded(const std::string* data);
  void RunWithoutValidationCache();

  
  void CheckWaiting();

  
  void MarkAsFailed();

  void MarkValidationCacheAsModified();
  void PersistValidationCache();

  
  base::WeakPtrFactory<NaClBrowser> weak_factory_;

  base::PlatformFile irt_platform_file_;
  base::FilePath irt_filepath_;
  NaClResourceState irt_state_;
  NaClValidationCache validation_cache_;
  NaClValidationCache off_the_record_validation_cache_;
  base::FilePath validation_cache_file_path_;
  bool validation_cache_is_enabled_;
  bool validation_cache_is_modified_;
  NaClResourceState validation_cache_state_;
  base::Callback<void(int)> debug_stub_port_listener_;

  typedef base::HashingMRUCache<std::string, base::FilePath> PathCacheType;
  PathCacheType path_cache_;

  bool ok_;

  
  std::vector<base::Closure> waiting_;

  scoped_ptr<NaClBrowserDelegate> browser_delegate_;

  std::deque<base::Time> crash_times_;

  DISALLOW_COPY_AND_ASSIGN(NaClBrowser);
};

} 

#endif  
