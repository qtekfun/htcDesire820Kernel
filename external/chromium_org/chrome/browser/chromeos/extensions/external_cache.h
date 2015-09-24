// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_EXTERNAL_CACHE_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_EXTERNAL_CACHE_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner.h"
#include "chrome/browser/extensions/updater/extension_downloader_delegate.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace base {
class DictionaryValue;
}

namespace extensions {
class ExtensionDownloader;
}

namespace net {
class URLRequestContextGetter;
}

namespace chromeos {

class ExternalCache : public content::NotificationObserver,
                      public extensions::ExtensionDownloaderDelegate {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}
    
    virtual void OnExtensionListsUpdated(
        const base::DictionaryValue* prefs) = 0;

    
    
    
    virtual std::string GetInstalledExtensionVersion(const std::string& id);
  };

  
  
  
  
  
  
  ExternalCache(const base::FilePath& cache_dir,
                net::URLRequestContextGetter* request_context,
                const scoped_refptr<base::SequencedTaskRunner>&
                    backend_task_runner,
                Delegate* delegate,
                bool always_check_updates,
                bool wait_for_cache_initialization);
  virtual ~ExternalCache();

  
  static const char kCacheReadyFlagFileName[];

  
  const base::DictionaryValue* cached_extensions() {
    return cached_extensions_.get();
  }

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnExtensionDownloadFailed(
      const std::string& id,
      Error error,
      const PingResult& ping_result,
      const std::set<int>& request_ids) OVERRIDE;

  virtual void OnExtensionDownloadFinished(
      const std::string& id,
      const base::FilePath& path,
      const GURL& download_url,
      const std::string& version,
      const PingResult& ping_result,
      const std::set<int>& request_ids) OVERRIDE;

  virtual bool IsExtensionPending(const std::string& id) OVERRIDE;

  virtual bool GetExtensionExistingVersion(const std::string& id,
                                           std::string* version) OVERRIDE;

  
  
  void Shutdown(const base::Closure& callback);

  
  
  void UpdateExtensionsList(scoped_ptr<base::DictionaryValue> prefs);

  
  
  
  void OnDamagedFileDetected(const base::FilePath& path);

 private:
  
  
  void UpdateExtensionLoader();

  
  
  
  void CheckCache();

  
  
  
  static void BackendCheckCacheStatus(
      base::WeakPtr<ExternalCache> external_cache,
      const base::FilePath& cache_dir);

  
  
  
  void OnCacheStatusChecked(bool ready);

  
  
  void CheckCacheContents();

  
  
  
  static void BackendCheckCacheContents(
      base::WeakPtr<ExternalCache> external_cache,
      const base::FilePath& cache_dir,
      scoped_ptr<base::DictionaryValue> prefs);

  
  static void BackendCheckCacheContentsInternal(
      const base::FilePath& cache_dir,
      base::DictionaryValue* prefs);

  
  
  void OnCacheUpdated(scoped_ptr<base::DictionaryValue> prefs);

  
  
  static void BackendInstallCacheEntry(
      base::WeakPtr<ExternalCache> external_cache,
      const base::FilePath& cache_dir,
      const std::string& id,
      const base::FilePath& path,
      const std::string& version);

  
  void OnCacheEntryInstalled(const std::string& id,
                             const base::FilePath& path,
                             const std::string& version);

  
  
  
  static void BackendShudown(const base::Closure& callback);

  
  base::FilePath cache_dir_;

  
  net::URLRequestContextGetter* request_context_;

  
  Delegate* delegate_;

  
  bool shutdown_;

  
  bool always_check_updates_;

  
  bool wait_for_cache_initialization_;

  
  scoped_ptr<base::DictionaryValue> extensions_;

  
  
  scoped_ptr<base::DictionaryValue> cached_extensions_;

  
  scoped_ptr<extensions::ExtensionDownloader> downloader_;

  
  scoped_refptr<base::SequencedTaskRunner> backend_task_runner_;

  
  content::NotificationRegistrar notification_registrar_;

  
  base::WeakPtrFactory<ExternalCache> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ExternalCache);
};

}  

#endif  
