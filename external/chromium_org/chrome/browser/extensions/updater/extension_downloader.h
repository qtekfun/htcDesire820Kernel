// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_UPDATER_EXTENSION_DOWNLOADER_H_
#define CHROME_BROWSER_EXTENSIONS_UPDATER_EXTENSION_DOWNLOADER_H_

#include <deque>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/version.h"
#include "chrome/browser/extensions/updater/extension_downloader_delegate.h"
#include "chrome/browser/extensions/updater/manifest_fetch_data.h"
#include "chrome/browser/extensions/updater/request_queue.h"
#include "chrome/common/extensions/update_manifest.h"
#include "extensions/common/extension.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

namespace net {
class URLFetcher;
class URLRequestContextGetter;
class URLRequestStatus;
}

namespace extensions {

struct UpdateDetails {
  UpdateDetails(const std::string& id, const base::Version& version);
  ~UpdateDetails();

  std::string id;
  base::Version version;
};

class ExtensionUpdaterTest;

class ExtensionDownloader : public net::URLFetcherDelegate {
 public:
  
  
  ExtensionDownloader(ExtensionDownloaderDelegate* delegate,
                      net::URLRequestContextGetter* request_context);
  virtual ~ExtensionDownloader();

  
  
  
  
  
  
  bool AddExtension(const Extension& extension, int request_id);

  
  
  
  
  
  
  bool AddPendingExtension(const std::string& id,
                           const GURL& update_url,
                           int request_id);

  
  
  void StartAllPending();

  
  void StartBlacklistUpdate(const std::string& version,
                            const ManifestFetchData::PingData& ping_data,
                            int request_id);

  
  
  static const int kManifestFetcherId = 1;
  static const int kExtensionFetcherId = 2;

  
  static const char kBlacklistAppID[];

  static const int kMaxRetries = 10;

 private:
  friend class ExtensionUpdaterTest;

  
  
  struct URLStats {
    URLStats()
        : no_url_count(0),
          google_url_count(0),
          other_url_count(0),
          extension_count(0),
          theme_count(0),
          app_count(0),
          platform_app_count(0),
          pending_count(0) {}

    int no_url_count, google_url_count, other_url_count;
    int extension_count, theme_count, app_count, platform_app_count,
        pending_count;
  };

  
  
  struct ExtensionFetch {
    ExtensionFetch();
    ExtensionFetch(const std::string& id, const GURL& url,
                   const std::string& package_hash, const std::string& version,
                   const std::set<int>& request_ids);
    ~ExtensionFetch();

    std::string id;
    GURL url;
    std::string package_hash;
    std::string version;
    std::set<int> request_ids;
  };

  
  bool AddExtensionData(const std::string& id,
                        const base::Version& version,
                        Manifest::Type extension_type,
                        const GURL& extension_update_url,
                        const std::string& update_url_data,
                        int request_id);

  
  void ReportStats() const;

  
  void StartUpdateCheck(scoped_ptr<ManifestFetchData> fetch_data);

  
  void CreateManifestFetcher();

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  void OnManifestFetchComplete(const GURL& url,
                               const net::URLRequestStatus& status,
                               int response_code,
                               const base::TimeDelta& backoff_delay,
                               const std::string& data);

  
  
  void HandleManifestResults(const ManifestFetchData& fetch_data,
                             const UpdateManifest::Results* results);

  
  
  void DetermineUpdates(const ManifestFetchData& fetch_data,
                        const UpdateManifest::Results& possible_updates,
                        std::vector<int>* result);

  
  void FetchUpdatedExtension(scoped_ptr<ExtensionFetch> fetch_data);

  
  void CreateExtensionFetcher();

  
  void OnCRXFetchComplete(const net::URLFetcher* source,
                          const GURL& url,
                          const net::URLRequestStatus& status,
                          int response_code,
                          const base::TimeDelta& backoff_delay);

  
  
  void NotifyExtensionsDownloadFailed(const std::set<std::string>& id_set,
                                      const std::set<int>& request_ids,
                                      ExtensionDownloaderDelegate::Error error);

  
  
  void NotifyUpdateFound(const std::string& id, const std::string& version);

  
  
  ExtensionDownloaderDelegate* delegate_;

  
  scoped_refptr<net::URLRequestContextGetter> request_context_;

  
  base::WeakPtrFactory<ExtensionDownloader> weak_ptr_factory_;

  
  URLStats url_stats_;

  
  
  
  
  typedef std::map<std::pair<int, GURL>,
                   std::vector<linked_ptr<ManifestFetchData> > > FetchMap;
  FetchMap fetches_preparing_;

  
  scoped_ptr<net::URLFetcher> manifest_fetcher_;
  scoped_ptr<net::URLFetcher> extension_fetcher_;

  
  
  RequestQueue<ManifestFetchData> manifests_queue_;
  RequestQueue<ExtensionFetch> extensions_queue_;

  
  std::map<std::string, ExtensionDownloaderDelegate::PingResult> ping_results_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionDownloader);
};

}  

#endif  
