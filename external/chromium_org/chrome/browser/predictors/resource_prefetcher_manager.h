// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCHER_MANAGER_H_
#define CHROME_BROWSER_PREDICTORS_RESOURCE_PREFETCHER_MANAGER_H_

#include <map>

#include "base/memory/ref_counted.h"
#include "chrome/browser/predictors/resource_prefetcher.h"
#include "chrome/browser/predictors/resource_prefetch_common.h"

namespace net {
class URLRequestContextGetter;
}

namespace predictors {

struct NavigationID;
class ResourcePrefetchPredictor;

class ResourcePrefetcherManager
    :  public ResourcePrefetcher::Delegate,
       public base::RefCountedThreadSafe<ResourcePrefetcherManager> {
 public:
  
  ResourcePrefetcherManager(ResourcePrefetchPredictor* predictor,
                            const ResourcePrefetchPredictorConfig& config,
                            net::URLRequestContextGetter* getter);

  
  void ShutdownOnUIThread();

  

  
  void ShutdownOnIOThread();

  
  
  
  void MaybeAddPrefetch(const NavigationID& navigation_id,
                        PrefetchKeyType key_type,
                        scoped_ptr<ResourcePrefetcher::RequestVector> requests);

  
  
  void MaybeRemovePrefetch(const NavigationID& navigation_id);

  
  virtual void ResourcePrefetcherFinished(
      ResourcePrefetcher* prefetcher,
      ResourcePrefetcher::RequestVector* requests) OVERRIDE;
  virtual net::URLRequestContext* GetURLRequestContext() OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<ResourcePrefetcherManager>;
  friend class MockResourcePrefetcherManager;

  typedef std::map<std::string, ResourcePrefetcher*> PrefetcherMap;

  virtual ~ResourcePrefetcherManager();

  
  void ResourcePrefetcherFinishedOnUI(
      const NavigationID& navigation_id,
      PrefetchKeyType key_type,
      scoped_ptr<ResourcePrefetcher::RequestVector> requests);

  ResourcePrefetchPredictor* predictor_;
  const ResourcePrefetchPredictorConfig config_;
  net::URLRequestContextGetter* const context_getter_;

  PrefetcherMap prefetcher_map_;  

  DISALLOW_COPY_AND_ASSIGN(ResourcePrefetcherManager);
};

}  

#endif  
