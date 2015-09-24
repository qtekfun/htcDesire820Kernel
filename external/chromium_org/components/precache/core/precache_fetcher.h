// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_PRECACHE_CORE_PRECACHE_FETCHER_H_
#define COMPONENTS_PRECACHE_CORE_PRECACHE_FETCHER_H_

#include <list>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "url/gurl.h"

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}

namespace precache {

class PrecacheFetcher {
 public:
  class PrecacheDelegate {
   public:
    
    
    
    virtual void OnDone() = 0;
  };

  
  
  
  
  PrecacheFetcher(const std::list<GURL>& starting_urls,
                  net::URLRequestContextGetter* request_context,
                  PrecacheDelegate* precache_delegate);

  virtual ~PrecacheFetcher();

  
  
  
  void Start();

 private:
  class Fetcher;

  
  
  
  
  void StartNextFetch();

  
  
  
  
  void OnConfigFetchComplete(const net::URLFetcher& source);

  
  
  
  void OnManifestFetchComplete(const net::URLFetcher& source);

  
  void OnResourceFetchComplete(const net::URLFetcher& source);

  
  
  const std::list<GURL> starting_urls_;

  
  scoped_refptr<net::URLRequestContextGetter> request_context_;

  
  PrecacheDelegate* precache_delegate_;

  scoped_ptr<Fetcher> fetcher_;

  std::list<GURL> manifest_urls_to_fetch_;
  std::list<GURL> resource_urls_to_fetch_;

  DISALLOW_COPY_AND_ASSIGN(PrecacheFetcher);
};

}  

#endif  
