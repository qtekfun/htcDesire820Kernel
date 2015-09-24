// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_UPDATER_EXTENSION_UPDATER_H_
#define CHROME_BROWSER_EXTENSIONS_UPDATER_EXTENSION_UPDATER_H_

#include <list>
#include <map>
#include <set>
#include <stack>
#include <string>

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/extensions/updater/extension_downloader_delegate.h"
#include "chrome/browser/extensions/updater/manifest_fetch_data.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "url/gurl.h"

class ExtensionServiceInterface;
class ExtensionSet;
class PrefService;
class Profile;

namespace extensions {

class ExtensionDownloader;
class ExtensionPrefs;
class ExtensionUpdaterTest;

class ExtensionUpdater : public ExtensionDownloaderDelegate,
                         public content::NotificationObserver {
 public:
  typedef base::Closure FinishedCallback;

  struct CheckParams {
    
    CheckParams();
    ~CheckParams();

    
    
    std::list<std::string> ids;

    
    
    
    bool install_immediately;

    
    
    FinishedCallback callback;
  };

  
  
  
  ExtensionUpdater(ExtensionServiceInterface* service,
                   ExtensionPrefs* extension_prefs,
                   PrefService* prefs,
                   Profile* profile,
                   int frequency_seconds);

  virtual ~ExtensionUpdater();

  
  void Start();

  
  
  void Stop();

  
  
  void CheckSoon();

  
  
  
  bool CheckExtensionSoon(const std::string& extension_id,
                          const FinishedCallback& callback);

  
  
  void CheckNow(const CheckParams& params);

  
  
  
  bool WillCheckSoon() const;

  
  
  void set_default_check_params(const CheckParams& params) {
    default_params_ = params;
  }

 private:
  friend class ExtensionUpdaterTest;
  friend class ExtensionUpdaterFileHandler;

  
  
  struct FetchedCRXFile {
    FetchedCRXFile();
    FetchedCRXFile(const std::string& id,
                   const base::FilePath& path,
                   const GURL& download_url,
                   const std::set<int>& request_ids);
    ~FetchedCRXFile();

    std::string extension_id;
    base::FilePath path;
    GURL download_url;
    std::set<int> request_ids;
  };

  struct InProgressCheck {
    InProgressCheck();
    ~InProgressCheck();

    bool install_immediately;
    FinishedCallback callback;
    
    std::list<std::string> in_progress_ids_;
  };

  struct ThrottleInfo;

  
  base::TimeDelta DetermineFirstCheckDelay();

  
  
  
  
  void ScheduleNextCheck(const base::TimeDelta& target_delay);

  
  
  void AddToDownloader(const ExtensionSet* extensions,
                       const std::list<std::string>& pending_ids,
                       int request_id);

  
  void TimerFired();

  
  void DoCheckSoon();

  
  virtual void OnExtensionDownloadFailed(
      const std::string& id,
      Error error,
      const PingResult& ping,
      const std::set<int>& request_ids) OVERRIDE;

  virtual void OnExtensionDownloadFinished(
      const std::string& id,
      const base::FilePath& path,
      const GURL& download_url,
      const std::string& version,
      const PingResult& ping,
      const std::set<int>& request_id) OVERRIDE;

  virtual bool GetPingDataForExtension(
      const std::string& id,
      ManifestFetchData::PingData* ping_data) OVERRIDE;

  virtual std::string GetUpdateUrlData(const std::string& id) OVERRIDE;

  virtual bool IsExtensionPending(const std::string& id) OVERRIDE;

  virtual bool GetExtensionExistingVersion(const std::string& id,
                                           std::string* version) OVERRIDE;

  void UpdatePingData(const std::string& id, const PingResult& ping_result);

  
  void MaybeInstallCRXFile();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void NotifyStarted();

  
  void NotifyIfFinished(int request_id);

  void ExtensionCheckFinished(const std::string& extension_id,
                              const FinishedCallback& callback);

  
  bool alive_;

  base::WeakPtrFactory<ExtensionUpdater> weak_ptr_factory_;

  
  ExtensionServiceInterface* service_;

  
  scoped_ptr<ExtensionDownloader> downloader_;

  base::OneShotTimer<ExtensionUpdater> timer_;
  int frequency_seconds_;
  bool will_check_soon_;

  ExtensionPrefs* extension_prefs_;
  PrefService* prefs_;
  Profile* profile_;

  std::map<int, InProgressCheck> requests_in_progress_;
  int next_request_id_;

  
  content::NotificationRegistrar registrar_;

  
  
  bool crx_install_is_running_;

  
  std::stack<FetchedCRXFile> fetched_crx_files_;
  FetchedCRXFile current_crx_file_;

  CheckParams default_params_;

  
  
  std::map<std::string, ThrottleInfo> throttle_info_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionUpdater);
};

}  

#endif  
