// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NET_SDCH_DICTIONARY_FETCHER_H_
#define CHROME_BROWSER_NET_SDCH_DICTIONARY_FETCHER_H_

#include <queue>
#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/sdch_manager.h"
#include "net/url_request/url_fetcher_delegate.h"

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}  

class SdchDictionaryFetcher
    : public net::URLFetcherDelegate,
      public net::SdchFetcher,
      public base::NonThreadSafe {
 public:
  explicit SdchDictionaryFetcher(net::URLRequestContextGetter* context);
  virtual ~SdchDictionaryFetcher();

  
  
  static void Shutdown();

  
  
  
  virtual void Schedule(const GURL& dictionary_url) OVERRIDE;

 private:
  
  
  
  
  static const int kMsDelayFromRequestTillDownload = 100;

  
  void ScheduleDelayedRun();

  
  
  void StartFetching();

  
  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  std::queue<GURL> fetch_queue_;
  
  
  scoped_ptr<net::URLFetcher> current_fetch_;

  
  
  
  base::WeakPtrFactory<SdchDictionaryFetcher> weak_factory_;
  bool task_is_pending_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  std::set<GURL> attempted_load_;

  
  
  scoped_refptr<net::URLRequestContextGetter> context_;

  DISALLOW_COPY_AND_ASSIGN(SdchDictionaryFetcher);
};

#endif  
