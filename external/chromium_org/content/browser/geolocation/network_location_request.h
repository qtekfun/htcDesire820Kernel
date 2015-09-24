// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_NETWORK_LOCATION_REQUEST_H_
#define CONTENT_BROWSER_GEOLOCATION_NETWORK_LOCATION_REQUEST_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/geolocation/wifi_data_provider.h"
#include "content/common/content_export.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}

namespace content {
struct Geoposition;

class NetworkLocationRequest : private net::URLFetcherDelegate {
 public:
  
  CONTENT_EXPORT static int url_fetcher_id_for_tests;

  
  
  
  typedef base::Callback<void(const Geoposition& ,
                              bool ,
                              const base::string16& ,
                              const WifiData& )>
      LocationResponseCallback;

  
  NetworkLocationRequest(net::URLRequestContextGetter* context,
                         const GURL& url,
                         LocationResponseCallback callback);
  virtual ~NetworkLocationRequest();

  
  
  bool MakeRequest(const base::string16& access_token,
                   const WifiData& wifi_data,
                   const base::Time& timestamp);

  bool is_request_pending() const { return url_fetcher_ != NULL; }
  const GURL& url() const { return url_; }

 private:
  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  scoped_refptr<net::URLRequestContextGetter> url_context_;
  LocationResponseCallback callback_;
  const GURL url_;
  scoped_ptr<net::URLFetcher> url_fetcher_;

  
  
  WifiData wifi_data_;
  base::Time timestamp_;  

  
  base::TimeTicks start_time_;

  DISALLOW_COPY_AND_ASSIGN(NetworkLocationRequest);
};

}  

#endif  
