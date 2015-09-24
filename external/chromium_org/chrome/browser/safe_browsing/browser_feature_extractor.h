// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_BROWSER_FEATURE_EXTRACTOR_H_
#define CHROME_BROWSER_SAFE_BROWSING_BROWSER_FEATURE_EXTRACTOR_H_

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "base/time/time.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/safe_browsing/safe_browsing_service.h"
#include "chrome/browser/safe_browsing/ui_manager.h"
#include "url/gurl.h"
#include "webkit/common/resource_type.h"


class HistoryService;

namespace content {
class WebContents;
}

namespace safe_browsing {
class ClientMalwareRequest;
class ClientPhishingRequest;
class ClientSideDetectionHost;

struct IPUrlInfo {
  
  std::string url;
  std::string method;
  std::string referrer;
  ResourceType::Type resource_type;

  IPUrlInfo(const std::string& url,
            const std::string& method,
            const std::string& referrer,
            const ResourceType::Type& resource_type);
  ~IPUrlInfo();
};

typedef std::map<std::string, std::vector<IPUrlInfo> > IPUrlMap;

struct BrowseInfo {
  
  
  IPUrlMap ips;

  
  
  scoped_ptr<SafeBrowsingUIManager::UnsafeResource> unsafe_resource;

  
  
  
  std::vector<GURL> host_redirects;
  std::vector<GURL> url_redirects;

  
  GURL referrer;

  
  int http_status_code;

  BrowseInfo();
  ~BrowseInfo();
};

class BrowserFeatureExtractor {
 public:
  
  
  
  
  typedef base::Callback<void(bool, ClientPhishingRequest*)> DoneCallback;
  typedef base::Callback<void(bool, scoped_ptr<ClientMalwareRequest>)>
      MalwareDoneCallback;

  
  
  
  BrowserFeatureExtractor(content::WebContents* tab,
                          ClientSideDetectionHost* host);

  
  virtual ~BrowserFeatureExtractor();

  
  
  
  
  
  
  virtual void ExtractFeatures(const BrowseInfo* info,
                               ClientPhishingRequest* request,
                               const DoneCallback& callback);

  
  
  
  
  
  virtual void ExtractMalwareFeatures(BrowseInfo* info,
                                      ClientMalwareRequest* request,
                                      const MalwareDoneCallback& callback);

 private:
  friend class base::DeleteHelper<BrowserFeatureExtractor>;
  typedef std::pair<ClientPhishingRequest*, DoneCallback> ExtractionData;
  typedef std::map<CancelableRequestProvider::Handle,
                   ExtractionData> PendingQueriesMap;

  
  void ExtractBrowseInfoFeatures(const BrowseInfo& info,
                                 ClientPhishingRequest* request);

  
  void StartExtractFeatures(ClientPhishingRequest* request,
                            const DoneCallback& callback);

  
  
  void QueryUrlHistoryDone(CancelableRequestProvider::Handle handle,
                           bool success,
                           const history::URLRow* row,
                           history::VisitVector* visits);

  
  
  void QueryHttpHostVisitsDone(CancelableRequestProvider::Handle handle,
                               bool success,
                               int num_visits,
                               base::Time first_visit);

  
  
  void QueryHttpsHostVisitsDone(CancelableRequestProvider::Handle handle,
                                bool success,
                                int num_visits,
                                base::Time first_visit);

  
  
  
  
  void SetHostVisitsFeatures(int num_visits,
                             base::Time first_visit,
                             bool is_http_query,
                             ClientPhishingRequest* request);

  
  
  void StorePendingQuery(CancelableRequestProvider::Handle handle,
                         ClientPhishingRequest* request,
                         const DoneCallback& callback);

  
  
  
  bool GetPendingQuery(CancelableRequestProvider::Handle handle,
                       ClientPhishingRequest** request,
                       DoneCallback* callback);

  
  
  bool GetHistoryService(HistoryService** history);

  
  
  void FinishExtractMalwareFeatures(scoped_ptr<IPUrlMap> bad_ips,
                                    MalwareDoneCallback callback,
                                    scoped_ptr<ClientMalwareRequest> request);

  content::WebContents* tab_;
  ClientSideDetectionHost* host_;
  CancelableRequestConsumer request_consumer_;
  base::WeakPtrFactory<BrowserFeatureExtractor> weak_factory_;

  
  
  std::map<ClientPhishingRequest*, DoneCallback> pending_extractions_;

  
  
  PendingQueriesMap pending_queries_;

  DISALLOW_COPY_AND_ASSIGN(BrowserFeatureExtractor);
};

}  
#endif  
