// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_COMMON_JSON_RESPONSE_FETCHER_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_COMMON_JSON_RESPONSE_FETCHER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/url_request/url_fetcher_delegate.h"

class GURL;

namespace base {
class DictionaryValue;
class Value;
}

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}

namespace app_list {

class JSONResponseFetcher : public net::URLFetcherDelegate {
 public:
  
  
  typedef base::Callback<void(scoped_ptr<base::DictionaryValue>)> Callback;

  JSONResponseFetcher(const Callback& callback,
                      net::URLRequestContextGetter* context_getter);
  virtual ~JSONResponseFetcher();

  
  void Start(const GURL& query_url);
  void Stop();

 private:
  
  void OnJsonParseSuccess(scoped_ptr<base::Value> parsed_json);
  void OnJsonParseError(const std::string& error);

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  Callback callback_;
  net::URLRequestContextGetter* context_getter_;

  scoped_ptr<net::URLFetcher> fetcher_;
  base::WeakPtrFactory<JSONResponseFetcher> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(JSONResponseFetcher);
};

}  

#endif  
