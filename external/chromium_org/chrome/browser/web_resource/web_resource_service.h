// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_RESOURCE_WEB_RESOURCE_SERVICE_H_
#define CHROME_BROWSER_WEB_RESOURCE_WEB_RESOURCE_SERVICE_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/web_resource/json_asynchronous_unpacker.h"
#include "chrome/browser/web_resource/resource_request_allowed_notifier.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

class PrefService;

namespace base {
class DictionaryValue;
}

namespace net {
class URLFetcher;
}

class WebResourceService
    : public net::URLFetcherDelegate,
      public JSONAsynchronousUnpackerDelegate,
      public base::RefCountedThreadSafe<WebResourceService>,
      public ResourceRequestAllowedNotifier::Observer {
 public:
  WebResourceService(PrefService* prefs,
                     const GURL& web_resource_server,
                     bool apply_locale_to_url_,
                     const char* last_update_time_pref_name,
                     int start_fetch_delay_ms,
                     int cache_update_delay_ms);

  
  
  
  void StartAfterDelay();

  
  virtual void OnUnpackFinished(const DictionaryValue& parsed_json) OVERRIDE;
  virtual void OnUnpackError(const std::string& error_message) OVERRIDE;

 protected:
  virtual ~WebResourceService();

  
  virtual void Unpack(const base::DictionaryValue& parsed_json) = 0;

  PrefService* prefs_;

 private:
  class UnpackerClient;
  friend class base::RefCountedThreadSafe<WebResourceService>;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  void ScheduleFetch(int64 delay_ms);

  
  void StartFetch();

  
  void EndFetch();

  
  virtual void OnResourceRequestsAllowed() OVERRIDE;

  
  
  ResourceRequestAllowedNotifier resource_request_allowed_notifier_;

  
  scoped_ptr<net::URLFetcher> url_fetcher_;

  
  
  JSONAsynchronousUnpacker* json_unpacker_;

  
  
  
  bool in_fetch_;

  
  GURL web_resource_server_;

  
  bool apply_locale_to_url_;

  
  const char* last_update_time_pref_name_;

  
  int start_fetch_delay_ms_;

  
  
  int cache_update_delay_ms_;

  
  
  base::WeakPtrFactory<WebResourceService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(WebResourceService);
};

#endif  
