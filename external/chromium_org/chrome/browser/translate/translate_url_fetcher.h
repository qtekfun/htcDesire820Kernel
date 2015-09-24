// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_TRANSLATE_URL_FETCHER_H_
#define CHROME_BROWSER_TRANSLATE_TRANSLATE_URL_FETCHER_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

class TranslateURLFetcher : public net::URLFetcherDelegate {
 public:
  
  typedef base::Callback<void(int, bool, const std::string&)> Callback;

  
  enum State {
    IDLE,        
    REQUESTING,  
    COMPLETED,   
    FAILED,      
  };

  explicit TranslateURLFetcher(int id);
  virtual ~TranslateURLFetcher();

  int max_retry_on_5xx() {
    return max_retry_on_5xx_;
  }
  void set_max_retry_on_5xx(int count) {
    max_retry_on_5xx_ = count;
  }

  const std::string& extra_request_header() {
    return extra_request_header_;
  }
  void set_extra_request_header(const std::string& header) {
    extra_request_header_ = header;
  }

  
  
  
  
  bool Request(const GURL& url, const Callback& callback);

  
  State state() { return state_; }

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

 private:
  
  GURL url_;

  
  const int id_;

  
  enum State state_;

  
  scoped_ptr<net::URLFetcher> fetcher_;

  
  
  Callback callback_;

  
  int retry_count_;

  
  int max_retry_on_5xx_;

  
  std::string extra_request_header_;

  DISALLOW_COPY_AND_ASSIGN(TranslateURLFetcher);
};

#endif  
