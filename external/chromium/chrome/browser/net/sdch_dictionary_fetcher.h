// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NET_SDCH_DICTIONARY_FETCHER_H_
#define CHROME_BROWSER_NET_SDCH_DICTIONARY_FETCHER_H_
#pragma once

#include <queue>
#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "chrome/common/net/url_fetcher.h"
#include "net/base/sdch_manager.h"

class SdchDictionaryFetcher : public URLFetcher::Delegate,
                              public net::SdchFetcher {
 public:
  SdchDictionaryFetcher();
  virtual ~SdchDictionaryFetcher();

  
  
  static void Shutdown();

  
  
  
  virtual void Schedule(const GURL& dictionary_url);

 private:
  
  
  
  
  static const int kMsDelayFromRequestTillDownload = 100;

  
  void ScheduleDelayedRun();

  
  
  void StartFetching();

  
  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  std::queue<GURL> fetch_queue_;
  
  
  scoped_ptr<URLFetcher> current_fetch_;

  
  
  
  ScopedRunnableMethodFactory<SdchDictionaryFetcher> method_factory_;
  bool task_is_pending_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  std::set<GURL> attempted_load_;

  DISALLOW_COPY_AND_ASSIGN(SdchDictionaryFetcher);
};

#endif  
