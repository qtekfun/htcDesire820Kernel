// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_UPDATER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_UPDATER_H_
#pragma once

#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_temp_dir.h"
#include "base/memory/weak_ptr.h"
#include "base/task.h"
#include "base/time.h"
#include "base/timer.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/common/extensions/update_manifest.h"
#include "chrome/common/net/url_fetcher.h"
#include "googleurl/src/gurl.h"

class Extension;
class ExtensionPrefs;
class ExtensionUpdaterTest;
class ExtensionUpdaterFileHandler;
class PrefService;
class Profile;
class SafeManifestParser;

class ManifestFetchData {
 public:
  static const int kNeverPinged = -1;

  
  enum PingType {
    
    ROLLCALL,

    
    
    ACTIVE
  };

  struct PingData {
    
    
    
    int rollcall_days;
    int active_days;

    PingData() : rollcall_days(0), active_days(0) {}
    PingData(int rollcall, int active)
        : rollcall_days(rollcall), active_days(active) {}
  };

  explicit ManifestFetchData(const GURL& update_url);
  ~ManifestFetchData();

  
  
  
  bool AddExtension(std::string id, std::string version,
                    const PingData& ping_data,
                    const std::string& update_url_data);

  const GURL& base_url() const { return base_url_; }
  const GURL& full_url() const { return full_url_; }
  int extension_count() { return extension_ids_.size(); }
  const std::set<std::string>& extension_ids() const { return extension_ids_; }

  
  bool Includes(const std::string& extension_id) const;

  
  
  bool DidPing(std::string extension_id, PingType type) const;

 private:
  
  std::set<std::string> extension_ids_;

  
  std::map<std::string, PingData> pings_;

  
  GURL base_url_;

  
  
  GURL full_url_;

  DISALLOW_COPY_AND_ASSIGN(ManifestFetchData);
};

class ManifestFetchesBuilder {
 public:
  ManifestFetchesBuilder(ExtensionServiceInterface* service,
                         ExtensionPrefs* prefs);
  ~ManifestFetchesBuilder();

  void AddExtension(const Extension& extension);

  void AddPendingExtension(const std::string& id,
                           const PendingExtensionInfo& info);

  
  void ReportStats() const;

  
  
  std::vector<ManifestFetchData*> GetFetches();

 private:
  struct URLStats {
    URLStats()
        : no_url_count(0),
          google_url_count(0),
          other_url_count(0),
          extension_count(0),
          theme_count(0),
          app_count(0),
          pending_count(0) {}

    int no_url_count, google_url_count, other_url_count;
    int extension_count, theme_count, app_count, pending_count;
  };

  void AddExtensionData(Extension::Location location,
                        const std::string& id,
                        const Version& version,
                        Extension::Type extension_type,
                        GURL update_url,
                        const std::string& update_url_data);
  ExtensionServiceInterface* const service_;
  ExtensionPrefs* const prefs_;

  
  
  
  
  std::multimap<GURL, ManifestFetchData*> fetches_;

  URLStats url_stats_;

  DISALLOW_COPY_AND_ASSIGN(ManifestFetchesBuilder);
};

class ExtensionUpdater : public URLFetcher::Delegate {
 public:
  
  
  
  ExtensionUpdater(ExtensionServiceInterface* service,
                   ExtensionPrefs* extension_prefs,
                   PrefService* prefs,
                   Profile* profile,
                   int frequency_seconds);

  virtual ~ExtensionUpdater();

  
  void Start();

  
  
  void Stop();

  
  
  void CheckSoon();

  
  
  void CheckNow();

  
  void set_blacklist_checks_enabled(bool enabled) {
    blacklist_checks_enabled_ = enabled;
  }

  
  
  
  bool WillCheckSoon() const;

 private:
  friend class ExtensionUpdaterTest;
  friend class ExtensionUpdaterFileHandler;
  friend class SafeManifestParser;

  
  
  struct ExtensionFetch {
    ExtensionFetch();
    ExtensionFetch(const std::string& i, const GURL& u,
                   const std::string& h, const std::string& v);
    ~ExtensionFetch();

    std::string id;
    GURL url;
    std::string package_hash;
    std::string version;
  };

  
  
  static const int kManifestFetcherId = 1;
  static const int kExtensionFetcherId = 2;

  static const char* kBlacklistAppID;

  
  void Init();

  
  base::TimeDelta DetermineFirstCheckDelay();

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  virtual void OnManifestFetchComplete(const GURL& url,
                                       const net::URLRequestStatus& status,
                                       int response_code,
                                       const std::string& data);
  virtual void OnCRXFetchComplete(const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const std::string& data);

  // Called when a crx file has been written into a temp file, and is ready
  
  void OnCRXFileWritten(const std::string& id,
                        const FilePath& path,
                        const GURL& download_url);

  
  void OnCRXFileWriteError(const std::string& id);

  
  
  void ProcessBlacklist(const std::string& data);

  
  
  
  
  void ScheduleNextCheck(const base::TimeDelta& target_delay);

  
  void TimerFired();

  
  void DoCheckSoon();

  
  void StartUpdateCheck(ManifestFetchData* fetch_data);

  
  void FetchUpdatedExtension(const std::string& id, const GURL& url,
    const std::string& hash, const std::string& version);

  
  
  void HandleManifestResults(const ManifestFetchData& fetch_data,
                             const UpdateManifest::Results* results);

  
  
  bool GetExistingVersion(const std::string& id, std::string* version);

  
  
  std::vector<int> DetermineUpdates(const ManifestFetchData& fetch_data,
      const UpdateManifest::Results& possible_updates);

  
  void NotifyStarted();

  
  
  void NotifyUpdateFound(const std::string& extension_id);

  
  void NotifyIfFinished();

  
  void AddToInProgress(const std::set<std::string>& ids);

  
  void RemoveFromInProgress(const std::set<std::string>& ids);

  
  bool alive_;

  base::WeakPtrFactory<ExtensionUpdater> weak_ptr_factory_;

  
  scoped_ptr<URLFetcher> manifest_fetcher_;
  scoped_ptr<URLFetcher> extension_fetcher_;

  
  
  std::deque<ManifestFetchData*> manifests_pending_;
  std::deque<ExtensionFetch> extensions_pending_;

  
  scoped_ptr<ManifestFetchData> current_manifest_fetch_;

  
  ExtensionFetch current_extension_fetch_;

  
  ExtensionServiceInterface* service_;

  base::OneShotTimer<ExtensionUpdater> timer_;
  int frequency_seconds_;

  ScopedRunnableMethodFactory<ExtensionUpdater> method_factory_;

  bool will_check_soon_;

  ExtensionPrefs* extension_prefs_;
  PrefService* prefs_;
  Profile* profile_;

  scoped_refptr<ExtensionUpdaterFileHandler> file_handler_;
  bool blacklist_checks_enabled_;

  
  std::set<std::string> in_progress_ids_;

  FRIEND_TEST(ExtensionUpdaterTest, TestStartUpdateCheckMemory);
  FRIEND_TEST(ExtensionUpdaterTest, TestAfterStopBehavior);

  DISALLOW_COPY_AND_ASSIGN(ExtensionUpdater);
};

#endif  
