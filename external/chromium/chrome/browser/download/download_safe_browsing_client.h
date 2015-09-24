// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_SAFE_BROWSING_CLIENT_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_SAFE_BROWSING_CLIENT_H_
#pragma once

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/time.h"
#include "chrome/browser/safe_browsing/safe_browsing_service.h"

struct DownloadCreateInfo;

class DownloadSBClient
    : public SafeBrowsingService::Client,
      public base::RefCountedThreadSafe<DownloadSBClient> {
 public:
  typedef Callback2<DownloadCreateInfo*, bool>::Type UrlDoneCallback;
  typedef Callback2<int32, bool>::Type HashDoneCallback;

  DownloadSBClient(int32 download_id,
                   const std::vector<GURL>& url_chain,
                   const GURL& referrer_url);

  
  
  
  
  void CheckDownloadUrl(DownloadCreateInfo* info, UrlDoneCallback* callback);
  void CheckDownloadHash(const std::string& hash, HashDoneCallback* callback);

 private:
  
  
  void CheckDownloadUrlOnIOThread(const std::vector<GURL>& url_chain);
  void CheckDownloadHashOnIOThread(const std::string& hash);

  
  virtual void OnDownloadUrlCheckResult(
      const std::vector<GURL>& url_chain,
      SafeBrowsingService::UrlCheckResult result);
  virtual void OnDownloadHashCheckResult(
      const std::string& hash, SafeBrowsingService::UrlCheckResult result);

  
  
  
  enum SBStatsType {
    DOWNLOAD_URL_CHECKS_TOTAL,
    DOWNLOAD_URL_CHECKS_CANCELED,
    DOWNLOAD_URL_CHECKS_MALWARE,

    DOWNLOAD_HASH_CHECKS_TOTAL,
    DOWNLOAD_HASH_CHECKS_MALWARE,

    
    
    DOWNLOAD_CHECKS_MAX
  };

  friend class base::RefCountedThreadSafe<DownloadSBClient>;
  virtual ~DownloadSBClient();

  
  void SafeBrowsingCheckUrlDone(SafeBrowsingService::UrlCheckResult result);
  void SafeBrowsingCheckHashDone(SafeBrowsingService::UrlCheckResult result);

  
  void ReportMalware(SafeBrowsingService::UrlCheckResult result);

  
  void UpdateDownloadCheckStats(SBStatsType stat_type);

  scoped_ptr<UrlDoneCallback> url_done_callback_;
  scoped_ptr<HashDoneCallback> hash_done_callback_;

  
  DownloadCreateInfo* info_;

  int32 download_id_;
  scoped_refptr<SafeBrowsingService> sb_service_;

  
  std::vector<GURL> url_chain_;
  GURL referrer_url_;

  
  base::TimeTicks start_time_;

  DISALLOW_COPY_AND_ASSIGN(DownloadSBClient);
};

#endif  
